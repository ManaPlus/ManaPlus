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

#ifndef RESOURCES_SUBIMAGE_H
#define RESOURCES_SUBIMAGE_H

#include "localconsts.h"
#include "main.h"

#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
#else
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL_opengl.h>
#endif

#endif

#include "resources/image.h"

/**
 * A clipped version of a larger image.
 */
class SubImage final : public Image
{
    public:
        /**
         * Constructor.
         */
#ifdef USE_SDL2
        SubImage(Image *const parent, SDL_Texture *const image,
                 const int x, const int y, const int width, const int height);
#endif
        SubImage(Image *const parent, SDL_Surface *const image,
                 const int x, const int y, const int width, const int height);
#ifdef USE_OPENGL
        SubImage(Image *const parent, const GLuint image,
                 const int x, const int y, const int width, const int height,
                 const int texWidth, const int textHeight);
#endif

        A_DELETE_COPY(SubImage)

        /**
         * Destructor.
         */
        ~SubImage();

        /**
         * Creates a new image with the desired clipping rectangle.
         *
         * @return <code>NULL</code> if creation failed and a valid
         *         image otherwise.
         */
        Image *getSubImage(const int x, const int y,
                           const int width,
                           const int height) override final A_WARN_UNUSED;

        SDL_Rect mInternalBounds;

    private:
        Image *mParent;
};

#endif  // RESOURCES_SUBIMAGE_H
