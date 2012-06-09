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
class Image;

struct Position;

/**
 * Defines a class for loading and storing images.
 */
class ImageHelper : public Resource
{
    friend class CompoundSprite;
    friend class Graphics;
    friend class Image;
#ifdef USE_OPENGL
    friend class OpenGLGraphics;
    friend class OpenGL1Graphics;
#endif

    public:
        /**
         * Loads an image from an SDL_RWops structure.
         *
         * @param rw         The SDL_RWops to load the image from.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        static Resource *load(SDL_RWops *rw);

        /**
         * Loads an image from an SDL_RWops structure and recolors it.
         *
         * @param rw         The SDL_RWops to load the image from.
         * @param dye        The dye used to recolor the image.
         *
         * @return <code>NULL</code> if an error occurred, a valid pointer
         *         otherwise.
         */
        static Resource *load(SDL_RWops *rw, Dye const &dye);

        /**
         * Loads an image from an SDL surface.
         */
        static Image *load(SDL_Surface *);

        static SDL_Surface *convertTo32Bit(SDL_Surface* tmpImage);

        static Image *createTextSurface(SDL_Surface *tmpImage, float alpha);

        static void SDLSetEnableAlphaCache(bool n)
        { mEnableAlphaCache = n; }

        static bool SDLGetEnableAlphaCache()
        { return mEnableAlphaCache; }

        static void setEnableAlpha(bool n)
        { mEnableAlpha = n; }

#ifdef USE_OPENGL

        // OpenGL only public functions

        /**
         * Sets the target image format. Use <code>false</code> for SDL and
         * <code>true</code> for OpenGL.
         */
        static void setLoadAsOpenGL(int useOpenGL);

        static int getLoadAsOpenGL()
        { return mUseOpenGL; }

        static int getTextureType()
        { return mTextureType; }

        static int getInternalTextureType()
        { return mInternalTextureType; }

        static void setInternalTextureType(int n)
        { mInternalTextureType = n; }

        static void setBlur(bool n)
        { mBlur = n; }

        static int mTextureType;

        static int mInternalTextureType;

        static void dumpSurfaceFormat(SDL_Surface *image);

        static SDL_Surface* SDLDuplicateSurface(SDL_Surface* tmpImage);

#endif

         /**
         * Tells if the image was loaded using OpenGL or SDL
         * @return true if OpenGL, false if SDL.
         */
        static int useOpenGL();

    protected:
        /** SDL_Surface to SDL_Surface Image loader */
        static Image *_SDLload(SDL_Surface *tmpImage);

        static bool mEnableAlphaCache;
        static bool mEnableAlpha;

        // -----------------------
        // OpenGL protected members
        // -----------------------
#ifdef USE_OPENGL
        /**
         * Returns the first power of two equal or bigger than the input.
         */
        static int powerOfTwo(int input);

        static Image *_GLload(SDL_Surface *tmpImage);

        static int mUseOpenGL;
        static int mTextureSize;
        static bool mBlur;
#endif
};

#endif
