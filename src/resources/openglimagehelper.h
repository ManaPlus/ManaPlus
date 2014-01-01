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

#ifndef RESOURCES_OPENGLIMAGEHELPER_H
#define RESOURCES_OPENGLIMAGEHELPER_H

#include "localconsts.h"
#include "main.h"

#ifdef USE_OPENGL

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif

#include "resources/imagehelper.h"

#ifdef ANDROID
#include <GLES/gl.h>
#define GL_RGBA8 GL_RGBA8_OES
#else
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL_opengl.h>
#endif

class Dye;
class Image;

/**
 * Defines a class for loading and storing images.
 */
class OpenGLImageHelper final : public ImageHelper
{
    friend class CompoundSprite;
    friend class Graphics;
    friend class Image;

    public:
        OpenGLImageHelper()
        {
        }

        A_DELETE_COPY(OpenGLImageHelper)

        ~OpenGLImageHelper()
        { }

        /**
         * Loads an image from an SDL_RWops structure and recolors it.
         *
         * @param rw         The SDL_RWops to load the image from.
         * @param dye        The dye used to recolor the image.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        Image *load(SDL_RWops *const rw,
                    Dye const &dye) const override final A_WARN_UNUSED;

        /**
         * Loads an image from an SDL surface.
         */
        Image *load(SDL_Surface *const tmpImage) const
                    override final A_WARN_UNUSED;

        Image *createTextSurface(SDL_Surface *const tmpImage,
                                 const int width, const int height,
                                 const float alpha)
                                 const override final A_WARN_UNUSED;

        // OpenGL only public functions

        static int getTextureType() A_WARN_UNUSED
        { return mTextureType; }

        static int getInternalTextureType() A_WARN_UNUSED
        { return mInternalTextureType; }

        static void setInternalTextureType(const int n)
        { mInternalTextureType = n; }

        static void setBlur(const bool n)
        { mBlur = n; }

        static int mTextureType;

        static int mInternalTextureType;

        static int getTextureSize() A_WARN_UNUSED
        { return mTextureSize; }

        static void initTextureSampler(const GLint id);

        static void setUseTextureSampler(const bool b)
        { mUseTextureSampler = b; }

        SDL_Surface *create32BitSurface(int width,
                                        int height) const override final;

    protected:
        /**
         * Returns the first power of two equal or bigger than the input.
         */
        int powerOfTwo(const int input) const A_WARN_UNUSED;

        Image *glLoad(SDL_Surface *tmpImage,
                      int width = 0, int height = 0) const A_WARN_UNUSED;

        static int mTextureSize;
        static bool mBlur;
        static bool mUseTextureSampler;
};

#endif
#endif  // RESOURCES_OPENGLIMAGEHELPER_H
