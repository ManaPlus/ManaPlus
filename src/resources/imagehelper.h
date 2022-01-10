/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef RESOURCES_IMAGEHELPER_H
#define RESOURCES_IMAGEHELPER_H

#include "localconsts.h"

#include "enums/render/rendertype.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

class Dye;
class Image;

/**
 * Defines a class for loading and storing images.
 */
class ImageHelper notfinal
{
    friend class CompoundSprite;
    friend class Image;

    public:
        A_DELETE_COPY(ImageHelper)

        virtual ~ImageHelper()
        { }

        /**
         * Loads an image from an SDL_RWops structure.
         *
         * @param rw         The SDL_RWops to load the image from.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        Image *load(SDL_RWops *const rw) A_WARN_UNUSED;

        virtual Image *load(SDL_RWops *const rw, Dye const &dye) A_WARN_UNUSED;

#ifdef __GNUC__
        virtual Image *loadSurface(SDL_Surface *const) A_WARN_UNUSED = 0;

        virtual Image *createTextSurface(SDL_Surface *const tmpImage,
                                         const int width, const int height,
                                         float alpha) A_WARN_UNUSED = 0;
#else  // __GNUC__

        virtual Image *loadSurface(SDL_Surface *const) A_WARN_UNUSED
        { return nullptr; }

        virtual Image *createTextSurface(SDL_Surface *const tmpImage,
                                         const int width, const int height,
                                         const float alpha) const A_WARN_UNUSED
        { return nullptr; }
#endif  // __GNUC__

        virtual SDL_Surface *create32BitSurface(int width,
                                                int height)
                                                const A_WARN_UNUSED;

        virtual void copySurfaceToImage(const Image *const image A_UNUSED,
                                        const int x A_UNUSED,
                                        const int y A_UNUSED,
                                        SDL_Surface *const surface A_UNUSED)
                                        const
        { }

        static SDL_Surface *convertTo32Bit(SDL_Surface *const tmpImage)
                                           A_WARN_UNUSED;

        static void dumpSurfaceFormat(const SDL_Surface *const image);

        constexpr2 static void setEnableAlpha(const bool n) noexcept2
        { mEnableAlpha = n; }

        static SDL_Surface *loadPng(SDL_RWops *const rw);

        constexpr2 static void setOpenGlMode(const RenderType useOpenGL)
                                            noexcept2
        { mUseOpenGL = useOpenGL; }

        virtual RenderType useOpenGL() const noexcept2 A_WARN_UNUSED
        { return mUseOpenGL; }

        virtual void postInit()
        { }

    protected:
        ImageHelper()
        { }

        static bool mEnableAlpha;
        static RenderType mUseOpenGL;
};

extern ImageHelper *imageHelper A_NONNULLPOINTER;
extern ImageHelper *surfaceImageHelper A_NONNULLPOINTER;
#endif  // RESOURCES_IMAGEHELPER_H
