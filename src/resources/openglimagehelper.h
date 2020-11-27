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

#ifndef RESOURCES_OPENGLIMAGEHELPER_H
#define RESOURCES_OPENGLIMAGEHELPER_H

#include "localconsts.h"

#ifdef USE_OPENGL

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif  // GL_TEXTURE_RECTANGLE_ARB

#include "resources/imagehelper.h"

#ifdef ANDROID
#include <GLES/gl.h>
#define GL_RGBA8 GL_RGBA8_OES
#else  // ANDROID
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif  // USE_SDL2
#ifdef HAVE_GLEXT
#define NO_SDL_GLEXT
#endif  // HAVE_GLEXT
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
// ignore -Wredundant-decls for SDL 1.2
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <SDL_opengl.h>
PRAGMA45(GCC diagnostic pop)
PRAGMA48(GCC diagnostic pop)
#ifdef HAVE_GLEXT
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <GL/glext.h>
PRAGMA45(GCC diagnostic pop)
#endif  // HAVE_GLEXT
#endif  // ANDROID

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
        OpenGLImageHelper() :
            mFreeTextureIndex(0U),
            mTextures()
        {
        }

        A_DELETE_COPY(OpenGLImageHelper)

        ~OpenGLImageHelper() override final;

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
                    Dye const &dye) override final A_WARN_UNUSED;

        /**
         * Loads an image from an SDL surface.
         */
        Image *loadSurface(SDL_Surface *const tmpImage) override final
                           A_WARN_UNUSED;

        Image *createTextSurface(SDL_Surface *const tmpImage,
                                 const int width, const int height,
                                 const float alpha)
                                 override final A_WARN_UNUSED;

        // OpenGL only public functions

        static int getTextureType() noexcept2 A_WARN_UNUSED
        { return mTextureType; }

        static int getInternalTextureType() noexcept2 A_WARN_UNUSED
        { return mInternalTextureType; }

        constexpr2 static void setInternalTextureType(const int n) noexcept2
        { mInternalTextureType = n; }

        constexpr2 static void setBlur(const bool n) noexcept2
        { mBlur = n; }

        static int mTextureType;

        static int mInternalTextureType;

        static int getTextureSize() noexcept2 A_WARN_UNUSED
        { return mTextureSize; }

        static void initTextureSampler(const GLint id);

        constexpr2 static void setUseTextureSampler(const bool b) noexcept2
        { mUseTextureSampler = b; }

        static void invalidate(const GLuint textureId);

        static void bindTexture(const GLuint texture);

        SDL_Surface *create32BitSurface(int width,
                                        int height) const override final;

        void postInit() override final;

        void copySurfaceToImage(const Image *const image,
                                const int x, const int y,
                                SDL_Surface *surface) const override final;

    protected:
        /**
         * Returns the first power of two equal or bigger than the input.
         */
        static int powerOfTwo(const int input) A_WARN_UNUSED;

        static SDL_Surface *convertSurfaceNormalize(SDL_Surface *tmpImage,
                                                    int width, int height);

        static SDL_Surface *convertSurface(SDL_Surface *tmpImage,
                                           int width, int height);

        Image *glLoad(SDL_Surface *tmpImage,
                      int width = 0, int height = 0) A_WARN_UNUSED;

        GLuint getNewTexture();

        static const size_t texturesSize = 10;
        size_t mFreeTextureIndex;
        GLuint mTextures[texturesSize];

        static int mTextureSize;
        static bool mBlur;
        static bool mUseTextureSampler;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_OPENGLIMAGEHELPER_H
