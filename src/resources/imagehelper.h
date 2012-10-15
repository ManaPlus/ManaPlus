/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include "localconsts.h"

#include "resources/resource.h"

#include <SDL.h>

class Dye;
class Image;

struct Position;

/**
 * Defines a class for loading and storing images.
 */
class ImageHelper
{
    friend class CompoundSprite;
    friend class Image;

    public:
        ImageHelper()
        { }

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
        Image *load(SDL_RWops *const rw);

#ifdef __GNUC__
        virtual Image *load(SDL_RWops *rw, Dye const &dye) = 0;

        virtual Image *load(SDL_Surface *) = 0;

        virtual Image *createTextSurface(SDL_Surface *tmpImage,
                                         float alpha) = 0;

        virtual int useOpenGL() = 0;
#else
        virtual Image *load(SDL_RWops *rw, Dye const &dye)
        { return nullptr; }

        virtual Image *load(SDL_Surface *)
        { return nullptr; }

        virtual Image *createTextSurface(SDL_Surface *const tmpImage,
                                         const float alpha)
        { return nullptr; }

        virtual int useOpenGL()
        { return 0; }
#endif

        static SDL_Surface *convertTo32Bit(SDL_Surface *const tmpImage);

        void dumpSurfaceFormat(const SDL_Surface *const image) const;

        static void setEnableAlpha(const bool n)
        { mEnableAlpha = n; }

    protected:
        static bool mEnableAlpha;
};

extern ImageHelper *imageHelper;
extern ImageHelper *sdlImageHelper;
#endif
