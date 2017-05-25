/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resources/dye/dyepalette.h"

#include "logger.h"

#ifndef DYECMD
#include "resources/db/palettedb.h"
#endif  // DYECMD

#include "utils/stringutils.h"

#ifndef USE_SDL2
#include <cmath>
#endif  // USE_SDL2

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#ifdef SIMD_SUPPORTED
#include "utils/cpu.h"
#endif  // SIMD_SUPPORTED

#include "debug.h"

#ifdef SIMD_SUPPORTED
DyeFunctionPtr DyePalette::funcReplaceSColor = nullptr;
DyeFunctionPtr DyePalette::funcReplaceSColorSse2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceSColorAvx2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAColor = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAColorSse2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAColorAvx2 = nullptr;

#ifdef USE_OPENGL
DyeFunctionPtr DyePalette::funcReplaceSOGLColor = nullptr;
DyeFunctionPtr DyePalette::funcReplaceSOGLColorSse2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceSOGLColorAvx2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAOGLColor = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAOGLColorSse2 = nullptr;
DyeFunctionPtr DyePalette::funcReplaceAOGLColorAvx2 = nullptr;
#endif  // USE_OPENGL
#endif  // SIMD_SUPPORTED

DyePalette::DyePalette(const std::string &restrict description,
                       const uint8_t blockSize) :
    mColors()
{
    const size_t size = CAST_SIZE(description.length());
    if (size == 0)
        return;

    StringVect parts;
    splitToStringVector(parts, description.substr(1), ',');
    if (description[0] == '#')
    {
        FOR_EACH (StringVectCIter, it, parts)
        {
            DyeColor color(0, 0, 0, 0);
            hexToColor(*it, blockSize, color);
            mColors.push_back(color);
        }
        return;
    }
#ifndef DYECMD
    else if (description[0] == '@')
    {
        uint8_t alpha = 255;
        FOR_EACH (StringVectCIter, it, parts)
        {
            const std::string str = *it;
            if (str.empty())
                continue;
            if (str[0] == '+')
            {
                if (str.size() != 3)
                    continue;
                alpha = CAST_U8((hexDecode(str[1]) << 4) + hexDecode(str[2]));
                continue;
            }
            const DyeColor *const color = PaletteDB::getColor(str);
            if (color)
            {
                DyeColor color2 = *color;
                color2.value[3] = alpha;
                mColors.push_back(color2);
            }
            else
            {
                DyeColor color2(0, 0, 0, 0);
                hexToColor(str, blockSize, color2);
                mColors.push_back(color2);
            }
        }
        return;
    }
#endif  // DYECMD

    logger->log("Error, invalid embedded palette: %s", description.c_str());
}

void DyePalette::hexToColor(const std::string &restrict hexStr,
                            const uint8_t blockSize,
                            DyeColor &color) noexcept2
{
    for (size_t i = 0, colorIdx = 0;
         i < blockSize && colorIdx < 4;
         i += 2, colorIdx ++)
    {
        color.value[colorIdx] = CAST_U8((
            hexDecode(hexStr[i]) << 4)
            + hexDecode(hexStr[i + 1]));
    }
    color.update();
}

unsigned int DyePalette::hexDecode(const signed char c) noexcept2
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else
        return 0;
}

void DyePalette::getColor(const unsigned int intensity,
                          unsigned int (&restrict color)[3]) const restrict2
{
    if (intensity == 0)
    {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        return;
    }

    const int last = CAST_S32(mColors.size());
    if (last == 0)
        return;

    const int intLast = intensity * last;
    const int i = intLast / 255;
    const int t = intLast % 255;

    int j = t != 0 ? i : i - 1;

    if (j >= last)
        j = 0;

    // Get the exact color if any, the next color otherwise.
    const DyeColor &colorJ = mColors[j];
    const int r2 = colorJ.value[0];
    const int g2 = colorJ.value[1];
    const int b2 = colorJ.value[2];

    if (t == 0)
    {
        // Exact color.
        color[0] = r2;
        color[1] = g2;
        color[2] = b2;
        return;
    }

    // Get the previous color. First color is implicitly black.
    if (i > 0 && i < last + 1)
    {
        const DyeColor &colorI = mColors[i - 1];
        const int t2 = 255 - t;
        // Perform a linear interpolation.
        color[0] = (t2 * colorI.value[0] + t * r2) / 255;
        color[1] = (t2 * colorI.value[1] + t * g2) / 255;
        color[2] = (t2 * colorI.value[2] + t * b2) / 255;
    }
    else
    {
        // Perform a linear interpolation.
        color[0] = (t * r2) / 255;
        color[1] = (t * g2) / 255;
        color[2] = (t * b2) / 255;
    }
}

void DyePalette::getColor(double intensity,
                          int (&restrict color)[3]) const restrict2
{
    // Nothing to do here
    if (mColors.empty())
        return;

    // Force range
    if (intensity > 1.0)
        intensity = 1.0;
    else if (intensity < 0.0)
        intensity = 0.0;

    // Scale up
    intensity *= static_cast<double>(mColors.size() - 1);

    // Color indices
    const int i = CAST_S32(floor(intensity));
    const int j = CAST_S32(ceil(intensity));
    const DyeColor &colorI = mColors[i];

    if (i == j)
    {
        // Exact color.
        color[0] = colorI.value[0];
        color[1] = colorI.value[1];
        color[2] = colorI.value[2];
        return;
    }

    intensity -= i;
    const double rest = 1 - intensity;
    const DyeColor &colorJ = mColors[j];

    // Perform the interpolation.
    color[0] = CAST_S32(rest * colorI.value[0] +
        intensity * colorJ.value[0]);
    color[1] = CAST_S32(rest * colorI.value[1] +
        intensity * colorJ.value[1]);
    color[2] = CAST_S32(rest * colorI.value[2] +
        intensity * colorJ.value[2]);
}

void DyePalette::initFunctions()
{
#ifdef SIMD_SUPPORTED
    const uint32_t flags = Cpu::getFlags();
    if (flags & Cpu::FEATURE_AVX2)
    {
        funcReplaceSColor = &DyePalette::replaceSColorAvx2;
        funcReplaceSColorAvx2 = &DyePalette::replaceSColorAvx2;
        funcReplaceSColorSse2 = &DyePalette::replaceSColorSse2;
        funcReplaceAColor = &DyePalette::replaceAColorAvx2;
        funcReplaceAColorAvx2 = &DyePalette::replaceAColorAvx2;
        funcReplaceAColorSse2 = &DyePalette::replaceAColorSse2;

#ifdef USE_OPENGL
        funcReplaceSOGLColor = &DyePalette::replaceSOGLColorAvx2;
        funcReplaceSOGLColorAvx2 = &DyePalette::replaceSOGLColorAvx2;
        funcReplaceSOGLColorSse2 = &DyePalette::replaceSOGLColorSse2;
        funcReplaceAOGLColor = &DyePalette::replaceAOGLColorAvx2;
        funcReplaceAOGLColorAvx2 = &DyePalette::replaceAOGLColorAvx2;
        funcReplaceAOGLColorSse2 = &DyePalette::replaceAOGLColorSse2;
#endif  // USE_OPENGL
    }
    else if (flags & Cpu::FEATURE_SSE2)
    {
        funcReplaceSColor = &DyePalette::replaceSColorSse2;
        funcReplaceSColorAvx2 = &DyePalette::replaceSColorSse2;
        funcReplaceSColorSse2 = &DyePalette::replaceSColorSse2;
        funcReplaceAColor = &DyePalette::replaceAColorSse2;
        funcReplaceAColorAvx2 = &DyePalette::replaceAColorSse2;
        funcReplaceAColorSse2 = &DyePalette::replaceAColorSse2;

#ifdef USE_OPENGL
        funcReplaceSOGLColor = &DyePalette::replaceSOGLColorSse2;
        funcReplaceSOGLColorAvx2 = &DyePalette::replaceSOGLColorSse2;
        funcReplaceSOGLColorSse2 = &DyePalette::replaceSOGLColorSse2;
        funcReplaceAOGLColor = &DyePalette::replaceAOGLColorSse2;
        funcReplaceAOGLColorAvx2 = &DyePalette::replaceAOGLColorSse2;
        funcReplaceAOGLColorSse2 = &DyePalette::replaceAOGLColorSse2;
#endif  // USE_OPENGL
    }
    else
#endif  // SIMD_SUPPORTED
    {
        funcReplaceSColor = &DyePalette::replaceSColorDefault;
        funcReplaceSColorAvx2 = &DyePalette::replaceSColorDefault;
        funcReplaceSColorSse2 = &DyePalette::replaceSColorDefault;
        funcReplaceAColor = &DyePalette::replaceAColorDefault;
        funcReplaceAColorAvx2 = &DyePalette::replaceAColorDefault;
        funcReplaceAColorSse2 = &DyePalette::replaceAColorDefault;

#ifdef USE_OPENGL
        funcReplaceSOGLColor = &DyePalette::replaceSOGLColorDefault;
        funcReplaceSOGLColorAvx2 = &DyePalette::replaceSOGLColorDefault;
        funcReplaceSOGLColorSse2 = &DyePalette::replaceSOGLColorDefault;
        funcReplaceAOGLColor = &DyePalette::replaceAOGLColorDefault;
        funcReplaceAOGLColorAvx2 = &DyePalette::replaceAOGLColorDefault;
        funcReplaceAOGLColorSse2 = &DyePalette::replaceAOGLColorDefault;
#endif  // USE_OPENGL
    }
}
