/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_DYE_DYEPALETTE_H
#define RESOURCES_DYE_DYEPALETTE_H

#include "resources/dye/dyecolor.h"

#include "utils/vector.h"

#include "resources/dye/dyepaletteptr.h"

#include <string>

#include "localconsts.h"

#define DYEPALETTE(palette, color) \
    ((palette).*DyePalette::funcReplace##color)

#define DYEPALETTEP(palette, color) \
    ((palette)->*DyePalette::funcReplace##color)

/**
 * Class for performing a linear interpolation between colors.
 */
class DyePalette final
{
    public:
        /**
         * Creates a palette based on the given string.
         * The string is either a file name or a sequence of hexadecimal RGB
         * values separated by ',' and starting with '#'.
         */
        DyePalette(const std::string &restrict pallete,
                   const uint8_t blockSize);

        A_DELETE_COPY(DyePalette)

        /**
         * Gets a pixel color depending on its intensity. First color is
         * implicitly black (0, 0, 0).
         */
        void getColor(const unsigned int intensity,
                      unsigned int (&restrict color)[3]) const restrict2;

        /**
         * Gets a pixel color depending on its intensity.
         */
        void getColor(double intensity,
                      int (&restrict color)[3]) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        void replaceSColorDefault(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        void replaceAColorDefault(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

#ifdef SIMD_SUPPORTED
        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceSColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for S dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceSColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAColorSse2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;

        /**
         * replace colors for SDL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAColorAvx2(uint32_t *restrict pixels,
                               const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED

#ifdef USE_OPENGL
        /**
         * replace colors for OpenGL for S dye.
         */
        void replaceSOGLColorDefault(uint32_t *restrict pixels,
                                     const int bufSize) const restrict2;
#ifdef SIMD_SUPPORTED
        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceAOGLColorAvx2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for S dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceSOGLColorSse2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
        /**
         * replace colors for OpenGL for S dye.
         */
        __attribute__ ((target ("avx2")))
        void replaceSOGLColorAvx2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;

        /**
         * replace colors for OpenGL for A dye.
         */
        __attribute__ ((target ("sse2")))
        void replaceAOGLColorSse2(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2;
#endif  // SIMD_SUPPORTED

        /**
         * replace colors for OpenGL for A dye.
         */
        void replaceAOGLColorDefault(uint32_t *restrict pixels,
                                     const int bufSize) const restrict2;

#endif  // USE_OPENGL

        static unsigned int hexDecode(const signed char c)
                                      noexcept2 A_CONST A_WARN_UNUSED;

        static void hexToColor(const std::string &restrict hexStr,
                               const uint8_t blockSize,
                               DyeColor &color) noexcept2;

        static void initFunctions();

#ifdef USE_OPENGL
        static DyeFunctionPtr funcReplaceSOGLColor;
        static DyeFunctionPtr funcReplaceSOGLColorSse2;
        static DyeFunctionPtr funcReplaceSOGLColorAvx2;
        static DyeFunctionPtr funcReplaceAOGLColor;
        static DyeFunctionPtr funcReplaceAOGLColorSse2;
        static DyeFunctionPtr funcReplaceAOGLColorAvx2;
#endif  // USE_OPENGL

        static DyeFunctionPtr funcReplaceSColor;
        static DyeFunctionPtr funcReplaceSColorSse2;
        static DyeFunctionPtr funcReplaceSColorAvx2;
        static DyeFunctionPtr funcReplaceAColor;
        static DyeFunctionPtr funcReplaceAColorSse2;
        static DyeFunctionPtr funcReplaceAColorAvx2;

#ifndef UNITTESTS
    private:
#endif  // UNITTESTS
        STD_VECTOR<DyeColor> mColors;
};

#endif  // RESOURCES_DYE_DYEPALETTE_H
