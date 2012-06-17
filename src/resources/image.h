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

#ifndef IMAGE_H
#define IMAGE_H

#include "localconsts.h"
#include "main.h"

#include "resources/resource.h"

#include <SDL.h>

#ifdef USE_OPENGL

/* The definition of OpenGL extensions by SDL is giving problems with recent
 * gl.h headers, since they also include these definitions. As we're not using
 * extensions anyway it's safe to just disable the SDL version.
 */
//#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
#endif

#include <map>

class Dye;

struct Position;

/**
 * Defines a class for loading and storing images.
 */
class Image : public Resource
{
    friend class CompoundSprite;
    friend class Graphics;
    friend class ImageHelper;
    friend class OpenGLImageHelper;
    friend class SDLImageHelper;
#ifdef USE_OPENGL
    friend class OpenGLGraphics;
    friend class OpenGL1Graphics;
#endif

    public:
        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Frees the resources created by SDL.
         */
        virtual void unload();

        /**
         * Tells is the image is loaded
         */
        bool isLoaded() const
        { return mLoaded; }

        /**
         * Returns the width of the image.
         */
        inline int getWidth() const     // was virtual
        { return mBounds.w; }

        /**
         * Returns the height of the image.
         */
        inline int getHeight() const     // was virtual
        { return mBounds.h; }

        /**
         * Tells if the image has got an alpha channel
         * @return true if it's true, false otherwise.
         */
        bool hasAlphaChannel();

        /**
         * Sets the alpha value of this image.
         */
        virtual void setAlpha(float alpha);

        /**
         * Returns the alpha value of this image.
         */
        float getAlpha() const
        { return mAlpha; }

        /**
         * Creates a new image with the desired clipping rectangle.
         *
         * @return <code>NULL</code> if creation failed and a valid
         *         object otherwise.
         */
        virtual Image *getSubImage(int x, int y, int width, int height);


        // SDL only public functions

        /**
         * Gets an scaled instance of an image.
         *
         * @param width The desired width of the scaled image.
         * @param height The desired height of the scaled image.
         *
         * @return A new Image* object.
         */
        Image* SDLgetScaledImage(int width, int height);

        /**
         * Get the alpha Channel of a SDL surface.
         */
        uint8_t *SDLgetAlphaChannel() const
        { return mAlphaChannel; }

        void SDLCleanCache();

        void SDLTerminateAlphaCache();

#ifdef USE_OPENGL
        // OpenGL only public functions

        int getTextureWidth() const
        { return mTexWidth; }

        int getTextureHeight() const
        { return mTexHeight; }
#endif

        bool isHasAlphaChannel() const
        { return mHasAlphaChannel; }

        bool isAlphaVisible() const
        { return mIsAlphaVisible; }

        void setAlphaVisible(bool b)
        { mIsAlphaVisible = b; }

        bool isAlphaCalculated() const
        { return mIsAlphaCalculated; }

        void setAlphaCalculated(bool b)
        { mIsAlphaCalculated = b; }

        SDL_Rect mBounds;

    protected:

        // -----------------------
        // Generic protected members
        // -----------------------

        bool mLoaded;
        float mAlpha;
        bool mHasAlphaChannel;

        // -----------------------
        // SDL protected members
        // -----------------------

        /** SDL Constructor */
        Image(SDL_Surface *image, bool hasAlphaChannel = false,
              uint8_t *alphaChannel = nullptr);

        SDL_Surface *getByAlpha(float alpha);

        SDL_Surface *mSDLSurface;

        /** Alpha Channel pointer used for 32bit based SDL surfaces */
        uint8_t *mAlphaChannel;

        std::map<float, SDL_Surface*> mAlphaCache;

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
        Image(GLuint glimage, int width, int height,
              int texWidth, int texHeight);

        GLuint mGLImage;
        int mTexWidth, mTexHeight;
#endif
};

#endif
