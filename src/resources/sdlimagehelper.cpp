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

#include "resources/sdlimagehelper.h"

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#include "client.h"
#include "logger.h"
#include "main.h"

#include "resources/image.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

bool SDLImageHelper::mEnableAlphaCache = false;

Image *SDLImageHelper::load(SDL_RWops *const rw, Dye const &dye) const
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
    rgba.colorkey = 0;
    rgba.alpha = 255;
    rgba.Rmask = 0xFF000000; rgba.Rloss = 0; rgba.Rshift = 24;
    rgba.Gmask = 0x00FF0000; rgba.Gloss = 0; rgba.Gshift = 16;
    rgba.Bmask = 0x0000FF00; rgba.Bloss = 0; rgba.Bshift = 8;
    rgba.Amask = 0x000000FF; rgba.Aloss = 0; rgba.Ashift = 0;

    SDL_Surface *const surf = SDL_ConvertSurface(
        tmpImage, &rgba, SDL_SWSURFACE);
    SDL_FreeSurface(tmpImage);

    uint32_t *pixels = static_cast<uint32_t *>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal)
            {
                for (uint32_t *p_end = pixels + surf->w * surf->h;
                      pixels != p_end; ++pixels)
                {
                    uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
                    const int alpha = *p & 255;
                    if (!alpha)
                        continue;
                    pal->replaceSColor(p + 1);
                }
            }
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal)
            {
                for (uint32_t *p_end = pixels + surf->w * surf->h;
                    pixels != p_end; ++pixels)
                {
                    pal->replaceAColor(reinterpret_cast<uint8_t *>(pixels));
                }
            }
            break;
        }
        case 0:
        default:
        {
            for (uint32_t *p_end = pixels + surf->w * surf->h;
                pixels != p_end; ++pixels)
            {
                const uint32_t p = *pixels;
                const int alpha = p & 255;
                if (!alpha)
                    continue;
                int v[3];
                v[0] = (p >> 24) & 255;
                v[1] = (p >> 16) & 255;
                v[2] = (p >> 8) & 255;
                dye.update(v);
                *pixels = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | alpha;
            }
            break;
        }
    }

    Image *const image = load(surf);
    SDL_FreeSurface(surf);
    return image;
}

Image *SDLImageHelper::load(SDL_Surface *const tmpImage) const
{
    return _SDLload(tmpImage);
}

Image *SDLImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                         const int width A_UNUSED,
                                         const int height A_UNUSED,
                                         const float alpha) const
{
    if (!tmpImage)
        return nullptr;

    Image *img;

    bool hasAlpha = false;
    const bool converted = false;

    const int sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    uint8_t *alphaChannel = new uint8_t[sz];

    const SDL_PixelFormat *const fmt = tmpImage->format;
    if (fmt->Amask)
    {
        for (int i = 0; i < sz; ++ i)
        {
            uint32_t c = (static_cast<uint32_t*>(tmpImage->pixels))[i];

            const unsigned v = (c & fmt->Amask) >> fmt->Ashift;
            const uint8_t a = static_cast<const uint8_t>((v << fmt->Aloss)
                + (v >> (8 - (fmt->Aloss << 1))));

            const uint8_t a2 = static_cast<uint8_t>(
                static_cast<float>(a) * alpha);

            c &= ~fmt->Amask;
            c |= ((a2 >> fmt->Aloss) << fmt->Ashift & fmt->Amask);
            (static_cast<uint32_t*>(tmpImage->pixels))[i] = c;

            if (a != 255)
                hasAlpha = true;

            alphaChannel[i] = a;
        }
    }

    SDL_Surface *image;

    // Convert the surface to the current display format
    if (hasAlpha)
    {
        image = SDL_DisplayFormatAlpha(tmpImage);
    }
    else
    {
        image = SDL_DisplayFormat(tmpImage);

        // We also delete the alpha channel since
        // it's not used.
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        logger->log1("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    if (converted)
        SDL_FreeSurface(tmpImage);

    img = new Image(image, hasAlpha, alphaChannel);
    img->mAlpha = alpha;
    return img;
}

SDL_Surface* SDLImageHelper::SDLDuplicateSurface(SDL_Surface *const tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return nullptr;

    return SDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *SDLImageHelper::_SDLload(SDL_Surface *tmpImage) const
{
    if (!tmpImage)
        return nullptr;

    bool hasAlpha = false;
    bool converted = false;

    if (tmpImage->format->BitsPerPixel != 32)
    {
        tmpImage = convertTo32Bit(tmpImage);

        if (!tmpImage)
            return nullptr;
        converted = true;
    }

    const int sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    uint8_t *alphaChannel = new uint8_t[sz];

    // Figure out whether the image uses its alpha layer
    if (!tmpImage->format->palette)
    {
        const SDL_PixelFormat *const fmt = tmpImage->format;
        if (fmt->Amask)
        {
            for (int i = 0; i < sz; ++ i)
            {
                const unsigned v = ((static_cast<uint32_t*>(
                    tmpImage->pixels))[i] & fmt->Amask) >> fmt->Ashift;
                const uint8_t a = static_cast<const uint8_t>((v << fmt->Aloss)
                    + (v >> (8 - (fmt->Aloss << 1))));

                if (a != 255)
                    hasAlpha = true;

                alphaChannel[i] = a;
            }
        }
        else
        {
            if (SDL_ALPHA_OPAQUE != 255)
            {
                hasAlpha = true;
                memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
            }
        }
    }
    else
    {
        if (SDL_ALPHA_OPAQUE != 255)
        {
            hasAlpha = true;
            memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
        }
    }

    SDL_Surface *image;

    // Convert the surface to the current display format
    if (hasAlpha)
    {
        image = SDL_DisplayFormatAlpha(tmpImage);
    }
    else
    {
        image = SDL_DisplayFormat(tmpImage);

        // We also delete the alpha channel since
        // it's not used.
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        logger->log1("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    if (converted)
        SDL_FreeSurface(tmpImage);

    return new Image(image, hasAlpha, alphaChannel);
}

int SDLImageHelper::useOpenGL() const
{
    return 0;
}

SDL_Surface *SDLImageHelper::create32BitSurface(int width,int height) const
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

    return SDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}
