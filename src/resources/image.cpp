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

#include "resources/image.h"

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#ifdef USE_OPENGL
#include "normalopenglgraphics.h"
#include "safeopenglgraphics.h"
#endif

#include "client.h"
#include "logger.h"

#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"
#include "resources/sdlimagehelper.h"
#include "resources/subimage.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

Image::Image(SDL_Surface *image, bool hasAlphaChannel0, uint8_t *alphaChannel):
    mAlpha(1.0f),
    mHasAlphaChannel(hasAlphaChannel0),
    mSDLSurface(image),
    mAlphaChannel(alphaChannel),
    mUseAlphaCache(SDLImageHelper::mEnableAlphaCache),
    mIsAlphaVisible(hasAlphaChannel0),
#ifdef USE_OPENGL
    mIsAlphaCalculated(false),
    mGLImage(0)
#else
    mIsAlphaCalculated(false)
#endif
{
    mBounds.x = 0;
    mBounds.y = 0;

    mLoaded = false;

    if (mSDLSurface)
    {
        mBounds.w = static_cast<uint16_t>(mSDLSurface->w);
        mBounds.h = static_cast<uint16_t>(mSDLSurface->h);

        mLoaded = true;
    }
    else
    {
        logger->log(
          "Image::Image(SDL_Surface*): Couldn't load invalid Surface!");
        mBounds.w = 0;
        mBounds.h = 0;
    }
}

#ifdef USE_OPENGL
Image::Image(GLuint glimage, int width, int height,
             int texWidth, int texHeight):
    mAlpha(1.0f),
    mHasAlphaChannel(true),
    mSDLSurface(nullptr),
    mAlphaChannel(nullptr),
    mUseAlphaCache(false),
    mIsAlphaVisible(true),
    mIsAlphaCalculated(false),
    mGLImage(glimage),
    mTexWidth(texWidth),
    mTexHeight(texHeight)
{
    mBounds.x = 0;
    mBounds.y = 0;
    mBounds.w = static_cast<uint16_t>(width);
    mBounds.h = static_cast<uint16_t>(height);

    if (mGLImage)
    {
        mLoaded = true;
    }
    else
    {
        logger->log(
          "Image::Image(GLuint*, ...): Couldn't load invalid Surface!");
        mLoaded = false;
    }
}
#endif

Image::~Image()
{
    unload();
}

void Image::SDLCleanCache()
{
    ResourceManager *resman = ResourceManager::getInstance();

    for (std::map<float, SDL_Surface*>::iterator
         i = mAlphaCache.begin(), i_end = mAlphaCache.end();
         i != i_end; ++i)
    {
        if (mSDLSurface != i->second)
            resman->scheduleDelete(i->second);
        i->second = nullptr;
    }
    mAlphaCache.clear();
}

void Image::unload()
{
    mLoaded = false;

    if (mSDLSurface)
    {
        SDLCleanCache();
        // Free the image surface.
        SDL_FreeSurface(mSDLSurface);
        mSDLSurface = nullptr;

        delete [] mAlphaChannel;
        mAlphaChannel = nullptr;
    }

#ifdef USE_OPENGL
    if (mGLImage)
    {
        glDeleteTextures(1, &mGLImage);
        mGLImage = 0;
#ifdef DEBUG_OPENGL_LEAKS
        if (textures_count > 0)
            textures_count --;
#endif
    }
#endif
}

bool Image::hasAlphaChannel()
{
    if (mLoaded)
        return mHasAlphaChannel;

#ifdef USE_OPENGL
    if (OpenGLImageHelper::mUseOpenGL)
        return true;
#endif

    return false;
}

SDL_Surface *Image::getByAlpha(float alpha)
{
    std::map<float, SDL_Surface*>::const_iterator it = mAlphaCache.find(alpha);
    if (it != mAlphaCache.end())
        return (*it).second;
    return nullptr;
}

void Image::setAlpha(float alpha)
{
    if (mAlpha == alpha || !ImageHelper::mEnableAlpha)
        return;

    if (alpha < 0.0f || alpha > 1.0f)
        return;

    if (mSDLSurface)
    {
        if (mUseAlphaCache)
        {
            SDL_Surface *surface = getByAlpha(mAlpha);
            if (!surface)
            {
                if (mAlphaCache.size() > 100)
                {
#ifdef DEBUG_ALPHA_CACHE
                    logger->log("cleanCache");
                    for (std::map<float, SDL_Surface*>::const_iterator
                         i = mAlphaCache.begin(), i_end = mAlphaCache.end();
                         i != i_end; ++i)
                    {
                        logger->log("alpha: " + toString(i->first));
                    }
#endif
                    SDLCleanCache();
                }
                surface = mSDLSurface;
                if (surface)
                    mAlphaCache[mAlpha] = surface;
            }
            else
            {
                logger->log("cache bug");
            }

            surface = getByAlpha(alpha);
            if (surface)
            {
//              logger->log("hit");
                if (mSDLSurface == surface)
                    logger->log("bug");
//                else
//                    SDL_FreeSurface(mSDLSurface);
                mAlphaCache.erase(alpha);
                mSDLSurface = surface;
                mAlpha = alpha;
                return;
            }
            else
            {
                mSDLSurface = SDLImageHelper::SDLDuplicateSurface(mSDLSurface);
            }
    //        logger->log("miss");
        }

        mAlpha = alpha;

        if (!mHasAlphaChannel)
        {
            // Set the alpha value this image is drawn at
            SDL_SetAlpha(mSDLSurface, SDL_SRCALPHA,
                         static_cast<unsigned char>(255 * mAlpha));
        }
        else
        {
            if (SDL_MUSTLOCK(mSDLSurface))
                SDL_LockSurface(mSDLSurface);

            // Precompute as much as possible
            const int maxHeight = std::min((mBounds.y + mBounds.h),
                mSDLSurface->h);
            const int maxWidth = std::min((mBounds.x + mBounds.w),
                mSDLSurface->w);
            const int i1 = mBounds.y * mSDLSurface->w + mBounds.x;
            const int i2 = (maxHeight - 1) * mSDLSurface->w + maxWidth - 1;

            const SDL_PixelFormat * const fmt = mSDLSurface->format;

            for (int i = i1; i <= i2; i++)
            {
                // Only change the pixel if it was visible at load time...
                uint8_t sourceAlpha = mAlphaChannel[i];
                if (sourceAlpha > 0)
                {
                    uint8_t a = static_cast<uint8_t>(
                        static_cast<float>(sourceAlpha) * mAlpha);

                    uint32_t c = (static_cast<uint32_t*>(
                        mSDLSurface->pixels))[i];
                    c &= ~fmt->Amask;
                    c |= ((a >> fmt->Aloss) << fmt->Ashift & fmt->Amask);
                    (static_cast<uint32_t*>(mSDLSurface->pixels))[i] = c;
                }
            }

            if (SDL_MUSTLOCK(mSDLSurface))
                SDL_UnlockSurface(mSDLSurface);
        }
    }
    else
    {
        mAlpha = alpha;
    }
}

Image* Image::SDLgetScaledImage(int width, int height)
{
    // No scaling on incorrect new values.
    if (width == 0 || height == 0)
        return nullptr;

    // No scaling when there is ... no different given size ...
    if (width == mBounds.w && height == mBounds.h)
        return nullptr;

    Image* scaledImage = nullptr;
    SDL_Surface* scaledSurface = nullptr;

    if (mSDLSurface)
    {
        scaledSurface = zoomSurface(mSDLSurface,
                    static_cast<double>(width) / mBounds.w,
                    static_cast<double>(height) / mBounds.h,
                    1);

        // The load function takes care of the SDL<->OpenGL implementation
        // and about freeing the given SDL_surface*.
        if (scaledSurface)
        {
            scaledImage = imageHelper->load(scaledSurface);
            SDL_FreeSurface(scaledSurface);
        }
    }
    return scaledImage;
}

Image *Image::getSubImage(int x, int y, int width, int height)
{
    // Create a new clipped sub-image
#ifdef USE_OPENGL
    if (OpenGLImageHelper::mUseOpenGL)
    {
        return new SubImage(this, mGLImage, x, y, width, height,
                            mTexWidth, mTexHeight);
    }
#endif

    return new SubImage(this, mSDLSurface, x, y, width, height);
}

void Image::SDLTerminateAlphaCache()
{
    SDLCleanCache();
    mUseAlphaCache = false;
}
