/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_SDL2IMAGEHELPER_H
#define RESOURCES_SDL2IMAGEHELPER_H

#ifdef USE_SDL2

#include "localconsts.h"

#include "resources/imagehelper.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL.h>
PRAGMA48(GCC diagnostic pop)

class Dye;
class Image;

/**
 * Defines a class for loading and storing images.
 */
class SDLImageHelper final : public ImageHelper
{
    friend class Image;

    public:
        SDLImageHelper() :
            ImageHelper()
        { }

        A_DELETE_COPY(SDLImageHelper)

        /**
         * Loads an image from an SDL surface.
         */
        Image *loadSurface(SDL_Surface *const tmpImage) override final
                           A_WARN_UNUSED;

        Image *createTextSurface(SDL_Surface *const tmpImage,
                                 const int width, const int height,
                                 const float alpha)
                                 override final A_WARN_UNUSED;

        void copySurfaceToImage(const Image *const image,
                                const int x, const int y,
                                SDL_Surface *const surface)
                                const override final;

        constexpr2 static void SDLSetEnableAlphaCache(const bool n) noexcept2
        { mEnableAlphaCache = n; }

        static bool SDLGetEnableAlphaCache() noexcept2 A_WARN_UNUSED
        { return mEnableAlphaCache; }

        static SDL_Surface* SDLDuplicateSurface(SDL_Surface *const tmpImage)
                                                A_WARN_UNUSED;

        static int combineSurface(SDL_Surface *restrict const src,
                                  SDL_Rect *restrict const srcrect,
                                  SDL_Surface *restrict const dst,
                                  SDL_Rect *restrict const dstrect);

        constexpr2 static void setRenderer(SDL_Renderer *const renderer)
                                          noexcept2
        { mRenderer = renderer; }

    protected:
        /** SDL_Surface to SDL_Surface Image loader */
        Image *_SDLload(SDL_Surface *tmpImage) A_WARN_UNUSED;

        static bool mEnableAlphaCache;
        static SDL_Renderer *mRenderer;
};

#endif  // USE_SDL2
#endif  // RESOURCES_SDL2IMAGEHELPER_H
