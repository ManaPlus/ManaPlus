/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "resources/resourcemanager.h"

#include "logger.h"

#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"
#include "resources/sdlimagehelper.h"
#include "resources/subimage.h"

#include "utils/sdlcheckutils.h"

#ifdef USE_SDL2
#include <SDL2_rotozoom.h>
#else
#include <SDL_rotozoom.h>
#endif

#include "debug.h"

#ifdef USE_SDL2
Image::Image(SDL_Texture *const image, const int width, const int height) :
    Resource(),
#ifdef USE_OPENGL
    mGLImage(0),
    mTexWidth(0),
    mTexHeight(0),
#endif
    mBounds(),
    mAlpha(1.0F),
    mSDLSurface(nullptr),
    mTexture(image),
    mAlphaChannel(nullptr),
    mAlphaCache(),
    mLoaded(false),
    mHasAlphaChannel(false),
    mUseAlphaCache(false),
    mIsAlphaVisible(true),
    mIsAlphaCalculated(false)
{
#ifdef DEBUG_IMAGES
    logger->log("created image: %p", this);
#endif

    mBounds.x = 0;
    mBounds.y = 0;

    if (mTexture)
    {
        mBounds.w = static_cast<uint16_t>(width);
        mBounds.h = static_cast<uint16_t>(height);

        mLoaded = true;
    }
    else
    {
        logger->log("Image::Image(SDL_Texture *const image):"
            " Couldn't load invalid Surface!");
        mBounds.w = 0;
        mBounds.h = 0;
    }
}
#endif

Image::Image(SDL_Surface *const image, const bool hasAlphaChannel0,
             uint8_t *const alphaChannel) :
    Resource(),
#ifdef USE_OPENGL
    mGLImage(0),
    mTexWidth(0),
    mTexHeight(0),
#endif
    mBounds(),
    mAlpha(1.0F),
    mSDLSurface(image),
#ifdef USE_SDL2
    mTexture(nullptr),
#endif
    mAlphaChannel(alphaChannel),
    mAlphaCache(),
    mLoaded(false),
    mHasAlphaChannel(hasAlphaChannel0),
    mUseAlphaCache(SDLImageHelper::mEnableAlphaCache),
    mIsAlphaVisible(hasAlphaChannel0),
    mIsAlphaCalculated(false)
{
#ifdef DEBUG_IMAGES
    logger->log("created image: %p", static_cast<void*>(this));
#endif

    mBounds.x = 0;
    mBounds.y = 0;

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
Image::Image(const GLuint glimage, const int width, const int height,
             const int texWidth, const int texHeight) :
    Resource(),
    mGLImage(glimage),
    mTexWidth(texWidth),
    mTexHeight(texHeight),
    mBounds(),
    mAlpha(1.0F),
    mSDLSurface(nullptr),
#ifdef USE_SDL2
    mTexture(nullptr),
#endif
    mAlphaChannel(nullptr),
    mAlphaCache(),
    mLoaded(false),
    mHasAlphaChannel(true),
    mUseAlphaCache(false),
    mIsAlphaVisible(true),
    mIsAlphaCalculated(false)
{
#ifdef DEBUG_IMAGES
    logger->log("created image: %p", static_cast<void*>(this));
#endif

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
        logger->log("Image::Image(GLuint*, ...):"
            " Couldn't load invalid Surface!");
    }
}
#endif

Image::~Image()
{
#ifdef DEBUG_IMAGES
    logger->log("delete image: %p", static_cast<void*>(this));
    logger->log("  %s, %s", mIdPath.c_str(), mSource.c_str());
#endif
    unload();
}

void Image::SDLCleanCache()
{
    ResourceManager *const resman = ResourceManager::getInstance();

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
        MSDL_FreeSurface(mSDLSurface);
        mSDLSurface = nullptr;

        delete [] mAlphaChannel;
        mAlphaChannel = nullptr;
    }
#ifdef USE_SDL2
    if (mTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
#endif

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

bool Image::hasAlphaChannel() const
{
    if (mLoaded)
        return mHasAlphaChannel;

#ifdef USE_OPENGL
    if (OpenGLImageHelper::mUseOpenGL != RENDER_SOFTWARE)
        return true;
#endif

    return false;
}

SDL_Surface *Image::getByAlpha(const float alpha)
{
    const std::map<float, SDL_Surface*>::const_iterator
        it = mAlphaCache.find(alpha);
    if (it != mAlphaCache.end())
        return (*it).second;
    return nullptr;
}

void Image::setAlpha(const float alpha)
{
    if (mAlpha == alpha || !ImageHelper::mEnableAlpha)
        return;

    if (alpha < 0.0F || alpha > 1.0F)
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
                if (mSDLSurface == surface)
                    logger->log("bug");
                mAlphaCache.erase(alpha);
                mSDLSurface = surface;
                mAlpha = alpha;
                return;
            }
            else
            {
                mSDLSurface = SDLImageHelper::SDLDuplicateSurface(mSDLSurface);
            }
        }

        mAlpha = alpha;

        if (!mHasAlphaChannel)
        {
#ifdef USE_SDL2
            SDL_SetSurfaceAlphaMod(mSDLSurface,
                static_cast<unsigned char>(255 * mAlpha));
#else
            // Set the alpha value this image is drawn at
            SDL_SetAlpha(mSDLSurface, SDL_SRCALPHA,
                static_cast<unsigned char>(255 * mAlpha));
#endif
        }
        else
        {
            if (SDL_MUSTLOCK(mSDLSurface))
                SDL_LockSurface(mSDLSurface);

            const int bx = mBounds.x;
            const int by = mBounds.y;
            const int bw = mBounds.w;
            const int bh = mBounds.h;
            const int bxw = bx + bw;
            const int sw = mSDLSurface->w;
            const int maxHeight = std::min(by + bh, mSDLSurface->h);
            const int maxWidth = std::min(bxw, sw);
            const int i1 = by * sw + bx;
            const SDL_PixelFormat * const fmt = mSDLSurface->format;
            const uint32_t amask = fmt->Amask;
            const uint32_t invMask = ~fmt->Amask;
            const uint8_t aloss = fmt->Aloss;
            const uint8_t ashift = fmt->Ashift;

            if (!bx && bxw == sw)
            {
                const int i2 = (maxHeight - 1) * sw + maxWidth - 1;
                for (int i = i1; i <= i2; i++)
                {
                    const uint8_t sourceAlpha = mAlphaChannel[i];
                    if (sourceAlpha > 0)
                    {
                        const uint8_t a = static_cast<uint8_t>(
                            static_cast<float>(sourceAlpha) * mAlpha);

                        uint32_t c = (static_cast<uint32_t*>(
                            mSDLSurface->pixels))[i];
                        c &= invMask;
                        c |= ((a >> aloss) << ashift & amask);
                        (static_cast<uint32_t*>(mSDLSurface->pixels))[i] = c;
                    }
                }
            }
            else
            {
                for (int y = by; y < maxHeight; y ++)
                {
                    const int idx = y * sw;
                    const int x1 = idx + bx;
                    const int x2 = idx + maxWidth;
                    for (int i = x1; i < x2; i ++)
                    {
                        const uint8_t sourceAlpha = mAlphaChannel[i];
                        if (sourceAlpha > 0)
                        {
                            const uint8_t a = static_cast<uint8_t>(
                                static_cast<float>(sourceAlpha) * mAlpha);

                            uint32_t c = (static_cast<uint32_t*>(
                                mSDLSurface->pixels))[i];
                            c &= invMask;
                            c |= ((a >> aloss) << ashift & amask);
                            (static_cast<uint32_t*>(
                                mSDLSurface->pixels))[i] = c;
                        }
                    }
                }
            }

            if (SDL_MUSTLOCK(mSDLSurface))
                SDL_UnlockSurface(mSDLSurface);
        }
    }
#ifdef USE_SDL2
    else if (mTexture)
    {
        mAlpha = alpha;
        SDL_SetTextureAlphaMod(mTexture,
            static_cast<unsigned char>(255 * mAlpha));
    }
#endif
    else
    {
        mAlpha = alpha;
    }
}

Image* Image::SDLgetScaledImage(const int width, const int height) const
{
    // No scaling on incorrect new values.
    if (width == 0 || height == 0)
        return nullptr;

    // No scaling when there is ... no different given size ...
    if (width == mBounds.w && height == mBounds.h)
        return nullptr;

    Image* scaledImage = nullptr;

    if (mSDLSurface)
    {
        SDL_Surface *const scaledSurface = zoomSurface(mSDLSurface,
                    static_cast<double>(width) / mBounds.w,
                    static_cast<double>(height) / mBounds.h,
                    1);

        // The load function takes care of the SDL<->OpenGL implementation
        // and about freeing the given SDL_surface*.
        if (scaledSurface)
        {
            scaledImage = imageHelper->load(scaledSurface);
            MSDL_FreeSurface(scaledSurface);
        }
    }
    return scaledImage;
}

Image *Image::getSubImage(const int x, const int y,
                          const int width, const int height)
{
    // Create a new clipped sub-image
#ifdef USE_OPENGL
    const RenderType mode = OpenGLImageHelper::mUseOpenGL;
    if (mode == RENDER_NORMAL_OPENGL || mode == RENDER_SAFE_OPENGL
        || mode == RENDER_GLES_OPENGL)
    {
        return new SubImage(this, mGLImage, x, y, width, height,
                            mTexWidth, mTexHeight);
    }
#endif

#ifdef USE_SDL2
#ifndef USE_OPENGL
    const RenderType mode = ImageHelper::mUseOpenGL;
#endif
    if (mode == RENDER_SOFTWARE)
        return new SubImage(this, mSDLSurface, x, y, width, height);
    else
        return new SubImage(this, mTexture, x, y, width, height);
#else
    return new SubImage(this, mSDLSurface, x, y, width, height);
#endif
}

void Image::SDLTerminateAlphaCache()
{
    SDLCleanCache();
    mUseAlphaCache = false;
}
