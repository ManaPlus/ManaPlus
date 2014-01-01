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

#ifndef RESOURCES_IMAGE_H
#define RESOURCES_IMAGE_H

#include "localconsts.h"
#include "main.h"

#include "resources/resource.h"

#include <SDL_video.h>

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

#ifdef USE_SDL2
#include <SDL_render.h>
#endif

#include <map>

/**
 * Defines a class for loading and storing images.
 */
class Image : public Resource
{
    friend class CompoundSprite;
    friend class Graphics;
    friend class ImageHelper;
    friend class OpenGLImageHelper;
    friend class SDLGraphics;
    friend class SDLImageHelper;
    friend class SurfaceGraphics;
#ifdef USE_SDL2
    friend class SDL2SoftwareGraphics;
    friend class SDL2SoftwareImageHelper;
    friend class SurfaceImageHelper;
#endif
    friend class TestLauncher;
#ifdef USE_OPENGL
    friend class AtlasManager;
    friend class MobileOpenGLGraphics;
    friend class NormalOpenGLGraphics;
    friend class NullOpenGLGraphics;
    friend class SafeOpenGLGraphics;
#endif

    public:
        A_DELETE_COPY(Image)

        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Frees the resources created by SDL.
         */
        void unload();

        /**
         * Tells is the image is loaded
         */
        bool isLoaded() const A_WARN_UNUSED
        { return mLoaded; }

        /**
         * Returns the width of the image.
         */
        inline int getWidth() const A_WARN_UNUSED
        { return mBounds.w; }

        /**
         * Returns the height of the image.
         */
        inline int getHeight() const A_WARN_UNUSED
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
         * Returns the alpha value of this image.
         */
        float getAlpha() const A_WARN_UNUSED
        { return mAlpha; }

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
        uint8_t *SDLgetAlphaChannel() const A_WARN_UNUSED
        { return mAlphaChannel; }

        void SDLCleanCache();

        void SDLTerminateAlphaCache();

#ifdef USE_OPENGL
        int getTextureWidth() const A_WARN_UNUSED
        { return mTexWidth; }

        int getTextureHeight() const A_WARN_UNUSED
        { return mTexHeight; }

        GLuint getGLImage() const A_WARN_UNUSED
        { return mGLImage; }

        GLuint mGLImage;
        int mTexWidth;
        int mTexHeight;
#endif

        bool isHasAlphaChannel() const A_WARN_UNUSED
        { return mHasAlphaChannel; }

        bool isAlphaVisible() const A_WARN_UNUSED
        { return mIsAlphaVisible; }

        void setAlphaVisible(const bool b)
        { mIsAlphaVisible = b; }

        bool isAlphaCalculated() const A_WARN_UNUSED
        { return mIsAlphaCalculated; }

        void setAlphaCalculated(const bool b)
        { mIsAlphaCalculated = b; }

        SDL_Surface* getSDLSurface()
        { return mSDLSurface; }

        SDL_Rect mBounds;

    protected:
        // -----------------------
        // Generic protected members
        // -----------------------

        float mAlpha;

        // -----------------------
        // SDL protected members
        // -----------------------

        /** SDL Constructor */
        Image(SDL_Surface *restrict const image, const bool hasAlphaChannel,
              uint8_t *restrict const alphaChannel = nullptr);

#ifdef USE_SDL2
        Image(SDL_Texture *restrict const image,
              const int width, const int height);
#endif

        SDL_Surface *getByAlpha(const float alpha) A_WARN_UNUSED;

        SDL_Surface *mSDLSurface;
#ifdef USE_SDL2
        SDL_Texture *mTexture;
#endif

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
#endif
};

#endif  // RESOURCES_IMAGE_H
