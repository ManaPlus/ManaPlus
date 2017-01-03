/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2009  Aethyra Development Team
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

#include "gui/fonts/textchunk.h"

#include "sdlshared.h"

#include "gui/fonts/font.h"

#include "resources/surfaceimagehelper.h"

#include "resources/image/image.h"

#include "utils/delete2.h"
#include "utils/sdlcheckutils.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    const int OUTLINE_SIZE = 1;
}  // namespace

char *restrict strBuf = nullptr;

#ifdef UNITTESTS
int textChunkCnt = 0;
#endif  // UNITTESTS

TextChunk::TextChunk() :
    img(nullptr),
    textFont(nullptr),
    text(),
    color(),
    color2(),
    prev(nullptr),
    next(nullptr)
{
#ifdef UNITTESTS
    textChunkCnt ++;
#endif  // UNITTESTS
}

TextChunk::TextChunk(const std::string &restrict text0,
                     const Color &restrict color0,
                     const Color &restrict color1,
                     Font *restrict const font) :
    img(nullptr),
    textFont(font),
    text(text0),
    color(color0),
    color2(color1),
    prev(nullptr),
    next(nullptr)
{
#ifdef UNITTESTS
    textChunkCnt ++;
#endif  // UNITTESTS
}

TextChunk::~TextChunk()
{
    delete2(img);
#ifdef UNITTESTS
    textChunkCnt --;
#endif  // UNITTESTS
}

bool TextChunk::operator==(const TextChunk &restrict chunk) const
{
    return (chunk.text == text && chunk.color == color
            && chunk.color2 == color2);
}

void TextChunk::generate(TTF_Font *restrict const font,
                         const float alpha)
{
    BLOCK_START("TextChunk::generate")
    SDL_Color sdlCol;
    sdlCol.b = CAST_U8(color.b);
    sdlCol.r = CAST_U8(color.r);
    sdlCol.g = CAST_U8(color.g);
#ifdef USE_SDL2
    sdlCol.a = 255;
#else  // USE_SDL2

    sdlCol.unused = 0;
#endif  // USE_SDL2

    getSafeUtf8String(text, strBuf);

    SDL_Surface *surface = MTTF_RenderUTF8_Blended(
        font, strBuf, sdlCol);

    if (!surface)
    {
        img = nullptr;
        BLOCK_END("TextChunk::generate")
        return;
    }

    const int width = surface->w;
    const int height = surface->h;

    if (color.r != color2.r || color.g != color2.g
        || color.b != color2.b)
    {   // outlining
        SDL_Color sdlCol2;
        SDL_Surface *const background = imageHelper->create32BitSurface(
            width, height);
        if (!background)
        {
            img = nullptr;
            MSDL_FreeSurface(surface);
            BLOCK_END("TextChunk::generate")
            return;
        }
        sdlCol2.b = CAST_U8(color2.b);
        sdlCol2.r = CAST_U8(color2.r);
        sdlCol2.g = CAST_U8(color2.g);
#ifdef USE_SDL2
        sdlCol2.a = 255;
#else  // USE_SDL2

        sdlCol2.unused = 0;
#endif  // USE_SDL2

        SDL_Surface *const surface2 = MTTF_RenderUTF8_Blended(
            font, strBuf, sdlCol2);
        if (!surface2)
        {
            img = nullptr;
            MSDL_FreeSurface(surface);
            BLOCK_END("TextChunk::generate")
            return;
        }
        SDL_Rect rect =
        {
            OUTLINE_SIZE,
            0,
            static_cast<Uint16>(surface->w),
            static_cast<Uint16>(surface->h)
        };
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = -OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = 0;
        rect.y = -OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.y = OUTLINE_SIZE;
        SurfaceImageHelper::combineSurface(surface2, nullptr,
            background, &rect);
        rect.x = 0;
        rect.y = 0;
        SurfaceImageHelper::combineSurface(surface, nullptr,
            background, &rect);
        MSDL_FreeSurface(surface);
        MSDL_FreeSurface(surface2);
        surface = background;
    }
    img = imageHelper->createTextSurface(
        surface, width, height, alpha);
    MSDL_FreeSurface(surface);

    BLOCK_END("TextChunk::generate")
}

void TextChunk::deleteImage()
{
    if (textFont)
    {
        textFont->insertChunk(this);
        img = nullptr;
    }
    else
    {
        delete2(img);
    }
}
