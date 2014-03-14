/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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
#include "main.h"

#include "resources/dye.h"

#include "utils/sdlcheckutils.h"

#include <SDL_image.h>

#include "debug.h"

ImageHelper *imageHelper = nullptr;
ImageHelper *surfaceImageHelper = nullptr;

bool ImageHelper::mEnableAlpha = true;
RenderType ImageHelper::mUseOpenGL = RENDER_SOFTWARE;

Image *ImageHelper::load(SDL_RWops *const rw) const
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    Image *const image = load(tmpImage);

    MSDL_FreeSurface(tmpImage);
    return image;
}

Image *ImageHelper::load(SDL_RWops *const rw, Dye const &dye) const
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    SDL_PixelFormat rgba;
    rgba.palette = nullptr;
    rgba.BitsPerPixel = 32;
    rgba.BytesPerPixel = 4;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rgba.Rmask = 0x000000FF;
    rgba.Gmask = 0x0000FF00;
    rgba.Bmask = 0x00FF0000;
    rgba.Amask = 0xFF000000;
#else
    rgba.Rmask = 0xFF000000;
    rgba.Gmask = 0x00FF0000;
    rgba.Bmask = 0x0000FF00;
    rgba.Amask = 0x000000FF;
#endif

    SDL_Surface *const surf = MSDL_ConvertSurface(
        tmpImage, &rgba, SDL_SWSURFACE);
    MSDL_FreeSurface(tmpImage);

    uint32_t *const pixels = static_cast<uint32_t *const>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal)
                pal->replaceSColor(pixels, surf->w * surf->h);
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal)
                pal->replaceAColor(pixels, surf->w * surf->h);
            break;
        }
        case 0:
        default:
        {
            dye.normalDye(pixels, surf->w * surf->h);
            break;
        }
    }

    Image *const image = load(surf);
    MSDL_FreeSurface(surf);
    return image;
}

SDL_Surface* ImageHelper::convertTo32Bit(SDL_Surface *const tmpImage)
{
    if (!tmpImage)
        return nullptr;
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette = nullptr;
#ifndef USE_SDL2
    RGBAFormat.colorkey = 0;
    RGBAFormat.alpha = 0;
#endif
    RGBAFormat.BitsPerPixel = 32;
    RGBAFormat.BytesPerPixel = 4;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    RGBAFormat.Rmask = 0xFF000000;
    RGBAFormat.Rshift = 0;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x00FF0000;
    RGBAFormat.Gshift = 8;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x0000FF00;
    RGBAFormat.Bshift = 16;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0x000000FF;
    RGBAFormat.Ashift = 24;
    RGBAFormat.Aloss = 0;
#else
    RGBAFormat.Rmask = 0x000000FF;
    RGBAFormat.Rshift = 24;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x0000FF00;
    RGBAFormat.Gshift = 16;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x00FF0000;
    RGBAFormat.Bshift = 8;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0xFF000000;
    RGBAFormat.Ashift = 0;
    RGBAFormat.Aloss = 0;
#endif
    return MSDL_ConvertSurface(tmpImage, &RGBAFormat, SDL_SWSURFACE);
}

void ImageHelper::dumpSurfaceFormat(const SDL_Surface *const image)
{
    if (image->format)
    {
        const SDL_PixelFormat * const format = image->format;
        logger->log("Bytes per pixel: %d", format->BytesPerPixel);
#ifndef USE_SDL2
        logger->log("Alpha: %d", format->alpha);
#endif
        logger->log("Loss: %02x, %02x, %02x, %02x",
            static_cast<int>(format->Rloss), static_cast<int>(format->Gloss),
            static_cast<int>(format->Bloss), static_cast<int>(format->Aloss));
        logger->log("Shift: %02x, %02x, %02x, %02x",
            static_cast<int>(format->Rshift), static_cast<int>(format->Gshift),
            static_cast<int>(format->Bshift),
            static_cast<int>(format->Ashift));
        logger->log("Mask: %08x, %08x, %08x, %08x", format->Rmask,
            format->Gmask, format->Bmask, format->Amask);
    }
}

SDL_Surface *ImageHelper::loadPng(SDL_RWops *const rw)
{
    if (!rw)
        return nullptr;

    if (IMG_isPNG(rw))
    {
        SDL_Surface *const tmpImage = MIMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        return tmpImage;
    }

    if (IMG_isJPG(rw))
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
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;
#endif

    return MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}
