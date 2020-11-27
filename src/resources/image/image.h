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

#ifndef RESOURCES_IMAGE_H
#define RESOURCES_IMAGE_H

#include "localconsts.h"

#include "enums/resources/imagetype.h"

#include "resources/resource.h"

#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
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
#endif  // USE_OPENGL

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef USE_SDL2
#include <SDL_render.h>
#else  // USE_SDL2
#include <SDL_video.h>
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include <map>

/**
 * Defines a class for loading and storing images.
 */
class Image notfinal : public Resource
{
    friend class CompoundSprite;
    friend class Graphics;
    friend class ImageHelper;
    friend class SDLGraphics;
    friend class SDLImageHelper;
    friend class SurfaceGraphics;
#ifdef USE_SDL2
    friend class SDL2SoftwareGraphics;
    friend class SDL2SoftwareImageHelper;
    friend class SurfaceImageHelper;
#endif  // USE_SDL2
    friend class TestLauncher;
#ifdef USE_OPENGL
    friend class AtlasManager;
    friend class MobileOpenGL2Graphics;
    friend class MobileOpenGLGraphics;
    friend class ModernOpenGLGraphics;
    friend class NormalOpenGLGraphics;
    friend class NullOpenGLGraphics;
    friend class SafeOpenGLGraphics;
    friend class OpenGLImageHelper;
#ifndef ANDROID
    friend class SafeOpenGLImageHelper;
#endif  // ANDROID
#endif  // USE_OPENGL

    public:
#ifdef UNITTESTS
        Image(const int width,
              const int height);
#endif  // UNITTESTS

        A_DELETE_COPY(Image)

        /**
         * Destructor.
         */
        ~Image() override;

        /**
         * Frees the resources created by SDL.
         */
        void unload();

        /**
         * Tells is the image is loaded
         */
        bool isLoaded() const noexcept2 A_WARN_UNUSED
        { return mLoaded; }

        /**
         * Returns the width of the image.
         */
        inline int getWidth() const noexcept2 A_WARN_UNUSED A_INLINE
        { return mBounds.w; }

        /**
         * Returns the height of the image.
         */
        inline int getHeight() const noexcept2 A_WARN_UNUSED A_INLINE
        { return mBounds.h; }

        /**
         * Tells if the image has got an alpha channel
         * @return true if it's true, false otherwise.
         */
        bool hasAlphaChannel() const A_WARN_UNUSED;

        /**
         * Sets the alpha value of this image.
         */
        virtual void setAlpha(const float alpha);

        /**
         * Creates a new image with the desired clipping rectangle.
         *
         * @return <code>NULL</code> if creation failed and a valid
         *         object otherwise.
         */
        virtual Image *getSubImage(const int x, const int y,
                                   const int width,
                                   const int height) A_WARN_UNUSED;


        // SDL only public functions

        /**
         * Gets an scaled instance of an image.
         *
         * @param width The desired width of the scaled image.
         * @param height The desired height of the scaled image.
         *
         * @return A new Image* object.
         */
        Image* SDLgetScaledImage(const int width,
                                 const int height) const A_WARN_UNUSED;

        /**
         * Get the alpha Channel of a SDL surface.
         */
        uint8_t *SDLgetAlphaChannel() const noexcept2 A_WARN_UNUSED
        { return mAlphaChannel; }

        void SDLCleanCache();

        void SDLTerminateAlphaCache();

#ifdef USE_OPENGL
        int getTextureWidth() const noexcept2 A_WARN_UNUSED
        { return mTexWidth; }

        int getTextureHeight() const noexcept2 A_WARN_UNUSED
        { return mTexHeight; }

        GLuint getGLImage() const noexcept2 A_WARN_UNUSED
        { return mGLImage; }

        void decRef() override;

        GLuint mGLImage;
        int mTexWidth;
        int mTexHeight;
#endif  // USE_OPENGL

        bool isHasAlphaChannel() const noexcept2 A_WARN_UNUSED
        { return mHasAlphaChannel; }

        bool isAlphaVisible() const noexcept2 A_WARN_UNUSED
        { return mIsAlphaVisible; }

        void setAlphaVisible(const bool b)
        { mIsAlphaVisible = b; }

        bool isAlphaCalculated() const noexcept2 A_WARN_UNUSED
        { return mIsAlphaCalculated; }

        void setAlphaCalculated(const bool b) noexcept2
        { mIsAlphaCalculated = b; }

        SDL_Surface* getSDLSurface() noexcept2 A_WARN_UNUSED
        { return mSDLSurface; }

        int calcMemoryLocal() const override;

        virtual ImageTypeT getType() const noexcept2 A_WARN_UNUSED
        { return ImageType::Image; }

        SDL_Rect mBounds;

        float mAlpha;

    protected:
        // -----------------------
        // SDL protected members
        // -----------------------

        /** SDL Constructor */
        Image(SDL_Surface *restrict const image,
              const bool hasAlphaChannel,
              uint8_t *restrict const alphaChannel);

#ifdef USE_SDL2
        Image(SDL_Texture *restrict const image,
              const int width, const int height);
#endif  // USE_SDL2

        SDL_Surface *getByAlpha(const float alpha) A_WARN_UNUSED;

        SDL_Surface *mSDLSurface;
#ifdef USE_SDL2
        SDL_Texture *mTexture;
#endif  // USE_SDL2

        /** Alpha Channel pointer used for 32bit based SDL surfaces */
        uint8_t *mAlphaChannel;

        std::map<float, SDL_Surface*> mAlphaCache;

        bool mLoaded;
        bool mHasAlphaChannel;
        bool mUseAlphaCache;
        bool mIsAlphaVisible;
        bool mIsAlphaCalculated;

        // -----------------------
        // OpenGL protected members
        // -----------------------
#ifdef USE_OPENGL
        /**
         * OpenGL Constructor.
         */
        Image(const GLuint glimage, const int width, const int height,
              const int texWidth, const int texHeight);
#endif  // USE_OPENGL
};

#endif  // RESOURCES_IMAGE_H
