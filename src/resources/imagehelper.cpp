/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/imagehelper.h"

#include "logger.h"

#include "resources/dye/dye.h"
#include "resources/dye/dyepalette.h"

#include "utils/sdlcheckutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_image.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

ImageHelper *imageHelper = nullptr;
ImageHelper *surfaceImageHelper = nullptr;

bool ImageHelper::mEnableAlpha = true;
RenderType ImageHelper::mUseOpenGL = RENDER_SOFTWARE;

Image *ImageHelper::load(SDL_RWops *const rw)
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (tmpImage == nullptr)
    {
        logger->log("Error, image load failed: %s", SDL_GetError());
        return nullptr;
    }

    Image *const image = loadSurface(tmpImage);

    MSDL_FreeSurface(tmpImage);
    return image;
}

Image *ImageHelper::load(SDL_RWops *const rw, Dye const &dye)
{
    BLOCK_START("ImageHelper::load")
    SDL_Surface *const tmpImage = loadPng(rw);
    if (tmpImage == nullptr)
    {
        logger->log("Error, image load failed: %s", SDL_GetError());
        BLOCK_END("ImageHelper::load")
        return nullptr;
    }

    SDL_PixelFormat rgba;
    rgba.palette = nullptr;
    rgba.BitsPerPixel = 32;
    rgba.BytesPerPixel = 4;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rgba.Rmask = 0x000000FFU;
    rgba.Gmask = 0x0000FF00U;
    rgba.Bmask = 0x00FF0000U;
    rgba.Amask = 0xFF000000U;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rgba.Rmask = 0xFF000000U;
    rgba.Gmask = 0x00FF0000U;
    rgba.Bmask = 0x0000FF00U;
    rgba.Amask = 0x000000FFU;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    SDL_Surface *const surf = MSDL_ConvertSurface(
        tmpImage, &rgba, SDL_SWSURFACE);
    MSDL_FreeSurface(tmpImage);

    if (surf == nullptr)
        return nullptr;

    uint32_t *const pixels = static_cast<uint32_t *>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal != nullptr)
                DYEPALETTEP(pal, SColor)(pixels, surf->w * surf->h);
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal != nullptr)
                DYEPALETTEP(pal, AColor)(pixels, surf->w * surf->h);
            break;
        }
        case 0:
        default:
        {
            dye.normalDye(pixels, surf->w * surf->h);
            break;
        }
    }

    Image *const image = loadSurface(surf);
    MSDL_FreeSurface(surf);
    BLOCK_END("ImageHelper::load")
    return image;
}

SDL_Surface* ImageHelper::convertTo32Bit(SDL_Surface *const tmpImage)
{
    if (tmpImage == nullptr)
        return nullptr;
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette = nullptr;
#ifndef USE_SDL2
    RGBAFormat.colorkey = 0;
    RGBAFormat.alpha = 0;
#endif  // USE_SDL2

    RGBAFormat.BitsPerPixel = 32;
    RGBAFormat.BytesPerPixel = 4;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    RGBAFormat.Rmask = 0xFF000000U;
    RGBAFormat.Rshift = 0;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x00FF0000U;
    RGBAFormat.Gshift = 8;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x0000FF00U;
    RGBAFormat.Bshift = 16;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0x000000FFU;
    RGBAFormat.Ashift = 24;
    RGBAFormat.Aloss = 0;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    RGBAFormat.Rmask = 0x000000FFU;
    RGBAFormat.Rshift = 24;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x0000FF00U;
    RGBAFormat.Gshift = 16;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x00FF0000U;
    RGBAFormat.Bshift = 8;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0xFF000000U;
    RGBAFormat.Ashift = 0;
    RGBAFormat.Aloss = 0;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    return MSDL_ConvertSurface(tmpImage, &RGBAFormat, SDL_SWSURFACE);
}

void ImageHelper::dumpSurfaceFormat(const SDL_Surface *const image)
{
    if (image == nullptr)
        return;
    if (image->format != nullptr)
    {
        const SDL_PixelFormat * const format = image->format;
        logger->log("Bytes per pixel: %d", format->BytesPerPixel);
#ifdef USE_SDL2
        logger->log("Format: %u", format->format);
#else  // USE_SDL2

        logger->log("Alpha: %d", format->alpha);
        logger->log("Color key: %u", format->colorkey);
#endif  // USE_SDL2

        logger->log("Loss: %02x, %02x, %02x, %02x",
            CAST_U32(format->Rloss),
            CAST_U32(format->Gloss),
            CAST_U32(format->Bloss),
            CAST_U32(format->Aloss));
        logger->log("Shift: %02x, %02x, %02x, %02x",
            CAST_U32(format->Rshift),
            CAST_U32(format->Gshift),
            CAST_U32(format->Bshift),
            CAST_U32(format->Ashift));
        logger->log("Mask: %08x, %08x, %08x, %08x", format->Rmask,
            format->Gmask, format->Bmask, format->Amask);
    }
    logger->log("Flags: %u", image->flags);
    logger->log("Pitch: %d", CAST_S32(image->pitch));
#ifndef USE_SDL2
    logger->log("Offset: %d", image->offset);
#endif  // USE_SDL2
}

SDL_Surface *ImageHelper::loadPng(SDL_RWops *const rw)
{
    if (rw == nullptr)
        return nullptr;

    if (IMG_isPNG(rw) != 0)
    {
        SDL_Surface *const tmpImage = MIMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        return tmpImage;
    }

    if (IMG_isJPG(rw) != 0)
    {
        SDL_Surface *const tmpImage = MIMG_LoadJPG_RW(rw);
        SDL_RWclose(rw);
        return tmpImage;
    }

    logger->log("Error, image is not png");
    SDL_RWclose(rw);
    return nullptr;
}

SDL_Surface *ImageHelper::create32BitSurface(int width,
                                             int height) const
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t rmask = 0xff000000U;
    const uint32_t gmask = 0x00ff0000U;
    const uint32_t bmask = 0x0000ff00U;
    const uint32_t amask = 0x000000ffU;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const uint32_t rmask = 0x000000ffU;
    const uint32_t gmask = 0x0000ff00U;
    const uint32_t bmask = 0x00ff0000U;
    const uint32_t amask = 0xff000000U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    return MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}
