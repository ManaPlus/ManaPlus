/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#include "client.h"
#include "logger.h"
#include "main.h"

#include "resources/image.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

ImageHelper *imageHelper = nullptr;
ImageHelper *sdlImageHelper = nullptr;

bool ImageHelper::mEnableAlpha = true;

Image *ImageHelper::load(SDL_RWops *const rw)
{
    SDL_Surface *const tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    Image *const image = load(tmpImage);

    SDL_FreeSurface(tmpImage);
    return image;
}

SDL_Surface* ImageHelper::convertTo32Bit(SDL_Surface *const tmpImage)
{
    if (!tmpImage)
        return nullptr;
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette = nullptr;
    RGBAFormat.colorkey = 0;
    RGBAFormat.alpha = 0;
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
    return SDL_ConvertSurface(tmpImage, &RGBAFormat, SDL_SWSURFACE);
}

void ImageHelper::dumpSurfaceFormat(const SDL_Surface *const image) const
{
    if (image->format)
    {
        const SDL_PixelFormat * const format = image->format;
        logger->log("Bytes per pixel: %d", format->BytesPerPixel);
        logger->log("Alpha: %d", format->alpha);
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
