/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "openglgraphics.h"
#include "opengl1graphics.h"
#endif

#include "client.h"
#include "log.h"
#include "main.h"

#include "utils/stringutils.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

#ifdef USE_OPENGL
int Image::mUseOpenGL = 0;
int Image::mTextureType = 0;
int Image::mTextureSize = 0;
#endif
bool Image::mEnableAlphaCache = false;
bool Image::mEnableAlpha = true;

Image::Image(SDL_Surface *image, bool hasAlphaChannel, Uint8 *alphaChannel):
    mAlpha(1.0f),
    mHasAlphaChannel(hasAlphaChannel),
    mSDLSurface(image),
    mAlphaChannel(alphaChannel)
{
#ifdef USE_OPENGL
    mGLImage = 0;
#endif

    mUseAlphaCache = Image::mEnableAlphaCache;

    mBounds.x = 0;
    mBounds.y = 0;

    mLoaded = false;

    if (mSDLSurface)
    {
        mBounds.w = static_cast<Uint16>(mSDLSurface->w);
        mBounds.h = static_cast<Uint16>(mSDLSurface->h);

        mLoaded = true;
    }
    else
    {
        logger->log(
          "Image::Image(SDL_Surface*): Couldn't load invalid Surface!");
    }
}

#ifdef USE_OPENGL
Image::Image(GLuint glimage, int width, int height,
             int texWidth, int texHeight):
    mAlpha(1.0f),
    mHasAlphaChannel(true),
    mSDLSurface(0),
    mAlphaChannel(0),
    mUseAlphaCache(false),
    mGLImage(glimage),
    mTexWidth(texWidth),
    mTexHeight(texHeight)
{
    mBounds.x = 0;
    mBounds.y = 0;
    mBounds.w = static_cast<Uint16>(width);
    mBounds.h = static_cast<Uint16>(height);

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

Resource *Image::load(void *buffer, unsigned bufferSize)
{
    // Load the raw file data from the buffer in an RWops structure
    SDL_RWops *rw = SDL_RWFromMem(buffer, bufferSize);
    SDL_Surface *tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return NULL;
    }

    Image *image = load(tmpImage);

    SDL_FreeSurface(tmpImage);
    return image;
}

Resource *Image::load(void *buffer, unsigned bufferSize, Dye const &dye)
{
    SDL_RWops *rw = SDL_RWFromMem(buffer, bufferSize);
    SDL_Surface *tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return NULL;
    }

    SDL_PixelFormat rgba;
    rgba.palette = NULL;
    rgba.BitsPerPixel = 32;
    rgba.BytesPerPixel = 4;
    rgba.Rmask = 0xFF000000; rgba.Rloss = 0; rgba.Rshift = 24;
    rgba.Gmask = 0x00FF0000; rgba.Gloss = 0; rgba.Gshift = 16;
    rgba.Bmask = 0x0000FF00; rgba.Bloss = 0; rgba.Bshift = 8;
    rgba.Amask = 0x000000FF; rgba.Aloss = 0; rgba.Ashift = 0;
    rgba.colorkey = 0;
    rgba.alpha = 255;

    SDL_Surface *surf = SDL_ConvertSurface(tmpImage, &rgba, SDL_SWSURFACE);
    SDL_FreeSurface(tmpImage);

    Uint32 *pixels = static_cast< Uint32 * >(surf->pixels);
    for (Uint32 *p_end = pixels + surf->w * surf->h; pixels != p_end; ++pixels)
    {
        int alpha = *pixels & 255;
        if (!alpha) continue;
        int v[3];
        v[0] = (*pixels >> 24) & 255;
        v[1] = (*pixels >> 16) & 255;
        v[2] = (*pixels >> 8 ) & 255;
        dye.update(v);
        *pixels = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | alpha;
    }

    Image *image = load(surf);
    SDL_FreeSurface(surf);
    return image;
}

Image *Image::load(SDL_Surface *tmpImage)
{
#ifdef USE_OPENGL
    if (mUseOpenGL)
        return _GLload(tmpImage);
#endif
    return _SDLload(tmpImage);
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
        i->second = 0;
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
        mSDLSurface = NULL;

        delete[] mAlphaChannel;
        mAlphaChannel = NULL;
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

int Image::useOpenGL() const
{
#ifdef USE_OPENGL
    return mUseOpenGL;
#else
    return 0;
#endif
}

bool Image::hasAlphaChannel()
{
    if (mLoaded)
        return mHasAlphaChannel;

#ifdef USE_OPENGL
    if (mUseOpenGL)
        return true;
#endif

    return false;
}

SDL_Surface *Image::getByAlpha(float alpha)
{
    std::map<float, SDL_Surface*>::iterator it = mAlphaCache.find(alpha);
    if (it != mAlphaCache.end())
        return (*it).second;
    return 0;
}

void Image::setAlpha(float alpha)
{
    if (mAlpha == alpha || !mEnableAlpha)
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
                    for (std::map<float, SDL_Surface*>::iterator
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
                mSDLSurface = Image::SDLDuplicateSurface(mSDLSurface);
            }
    //        logger->log("miss");
        }

        mAlpha = alpha;

        if (!hasAlphaChannel())
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

            for (int i = i1; i <= i2; i++)
            {
                // Only change the pixel if it was visible at load time...
                Uint8 sourceAlpha = mAlphaChannel[i];
                if (sourceAlpha > 0)
                {
                    Uint8 r, g, b, a;
                    SDL_GetRGBA((static_cast<Uint32*>
                        (mSDLSurface->pixels))[i],
                        mSDLSurface->format,
                        &r, &g, &b, &a);

                    a = static_cast<Uint8>(static_cast<float>(
                        sourceAlpha) * mAlpha);

                    // Here is the pixel we want to set
                    (static_cast<Uint32 *>(mSDLSurface->pixels))[i] =
                        SDL_MapRGBA(mSDLSurface->format, r, g, b, a);
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

Image* Image::SDLmerge(Image *image, int x, int y)
{
    if (!mSDLSurface || !image || !image->mSDLSurface)
        return NULL;

    SDL_Surface* surface = new SDL_Surface(*(image->mSDLSurface));

    Uint32 surface_pix, cur_pix;
    Uint8 r, g, b, a, p_r, p_g, p_b, p_a;
    double f_a, f_ca, f_pa;
    SDL_PixelFormat *current_fmt = mSDLSurface->format;
    SDL_PixelFormat *surface_fmt = surface->format;
    int current_offset, surface_offset;
    int offset_x, offset_y;

    SDL_LockSurface(surface);
    SDL_LockSurface(mSDLSurface);

    const int x0 = (y * getWidth()) + x;
    const int maxX = std::min(image->getWidth(), getWidth() - x);
    const int maxY = std::min(image->getHeight(), getHeight() - y);

    // for each pixel lines of a source image
    for (offset_x = (x > 0 ? 0 : -x); offset_x < maxX; offset_x++)
    {
        const int x1 = x0 + offset_x;
        for (offset_y = (y > 0 ? 0 : -y); offset_y < maxY; offset_y++)
        {
            // Computing offset on both images
            current_offset = (offset_y * getWidth()) + x1;
            surface_offset = offset_y * surface->w + offset_x;

            // Retrieving a pixel to merge
            surface_pix = (static_cast<Uint32*>(
                surface->pixels))[surface_offset];
            cur_pix = (static_cast<Uint32*>(
                mSDLSurface->pixels))[current_offset];

            // Retreiving each channel of the pixel using pixel format
            r = static_cast<Uint8>(((surface_pix & surface_fmt->Rmask) >>
                          surface_fmt->Rshift) << surface_fmt->Rloss);
            g = static_cast<Uint8>(((surface_pix & surface_fmt->Gmask) >>
                          surface_fmt->Gshift) << surface_fmt->Gloss);
            b = static_cast<Uint8>(((surface_pix & surface_fmt->Bmask) >>
                          surface_fmt->Bshift) << surface_fmt->Bloss);
            a = static_cast<Uint8>(((surface_pix & surface_fmt->Amask) >>
                          surface_fmt->Ashift) << surface_fmt->Aloss);

            // Retreiving previous alpha value
            p_a = static_cast<Uint8>(((cur_pix & current_fmt->Amask) >>
                            current_fmt->Ashift) << current_fmt->Aloss);

            // new pixel with no alpha or nothing on previous pixel
            if (a == SDL_ALPHA_OPAQUE || (p_a == 0 && a > 0))
            {
                (static_cast<Uint32 *>(surface->pixels))[current_offset] =
                    SDL_MapRGBA(current_fmt, r, g, b, a);
            }
            else if (a > 0)
            { // alpha is lower => merge color with previous value
                f_a = static_cast<double>(a) / 255.0;
                f_ca = 1.0 - f_a;
                f_pa = static_cast<double>(p_a) / 255.0;
                p_r = static_cast<Uint8>(((cur_pix & current_fmt->Rmask) >>
                                current_fmt->Rshift) << current_fmt->Rloss);
                p_g = static_cast<Uint8>(((cur_pix & current_fmt->Gmask) >>
                                current_fmt->Gshift) << current_fmt->Gloss);
                p_b = static_cast<Uint8>(((cur_pix & current_fmt->Bmask) >>
                                current_fmt->Bshift) << current_fmt->Bloss);
                r = static_cast<Uint8>(static_cast<double>(p_r) * f_ca
                    * f_pa + static_cast<double>(r) * f_a);
                g = static_cast<Uint8>(static_cast<double>(p_g) * f_ca
                    * f_pa + static_cast<double>(g) * f_a);
                b = static_cast<Uint8>(static_cast<double>(p_b) * f_ca
                    * f_pa + static_cast<double>(b) * f_a);
                a = (a > p_a ? a : p_a);
               (static_cast<Uint32 *>(surface->pixels))[current_offset] =
                   SDL_MapRGBA(current_fmt, r, g, b, a);
            }
        }
    }
    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(mSDLSurface);

    Image *newImage = new Image(surface);

    return newImage;
}

Image* Image::SDLgetScaledImage(int width, int height)
{
    // No scaling on incorrect new values.
    if (width == 0 || height == 0)
        return NULL;

    // No scaling when there is ... no different given size ...
    if (width == getWidth() && height == getHeight())
        return NULL;

    Image* scaledImage = NULL;
    SDL_Surface* scaledSurface = NULL;

    if (mSDLSurface)
    {
        scaledSurface = zoomSurface(mSDLSurface,
                    static_cast<double>(width) / getWidth(),
                    static_cast<double>(height) / getHeight(),
                    1);

        // The load function takes care of the SDL<->OpenGL implementation
        // and about freeing the given SDL_surface*.
        if (scaledSurface)
        {
            scaledImage = load(scaledSurface);
            SDL_FreeSurface(scaledSurface);
        }
    }
    return scaledImage;
}

SDL_Surface* Image::convertTo32Bit(SDL_Surface* tmpImage)
{
    if (!tmpImage)
        return NULL;
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette = 0;
    RGBAFormat.colorkey = 0;
    RGBAFormat.alpha = 0;
    RGBAFormat.BitsPerPixel = 32;
    RGBAFormat.BytesPerPixel = 4;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    RGBAFormat.Rmask = 0xFF000000;
    RGBAFormat.Rshift = 0;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x00FF0000;
    RGBAFormat.Gshift = 8;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x0000FF00;
    RGBAFormat.Bshift = 16;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0x000000FF;
    RGBAFormat.Ashift = 24;
    RGBAFormat.Aloss = 0;
#else
    RGBAFormat.Rmask = 0x000000FF;
    RGBAFormat.Rshift = 24;
    RGBAFormat.Rloss = 0;
    RGBAFormat.Gmask = 0x0000FF00;
    RGBAFormat.Gshift = 16;
    RGBAFormat.Gloss = 0;
    RGBAFormat.Bmask = 0x00FF0000;
    RGBAFormat.Bshift = 8;
    RGBAFormat.Bloss = 0;
    RGBAFormat.Amask = 0xFF000000;
    RGBAFormat.Ashift = 0;
    RGBAFormat.Aloss = 0;
#endif
    return SDL_ConvertSurface(tmpImage, &RGBAFormat, SDL_SWSURFACE);
}

SDL_Surface* Image::SDLDuplicateSurface(SDL_Surface* tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return NULL;

    return SDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *Image::_SDLload(SDL_Surface *tmpImage)
{
    if (!tmpImage)
        return NULL;

    bool hasAlpha = false;
    bool converted = false;

    // The alpha channel to be filled with alpha values
    Uint8 *alphaChannel = new Uint8[tmpImage->w * tmpImage->h];

    if (tmpImage->format->BitsPerPixel != 32)
    {
        tmpImage = convertTo32Bit(tmpImage);

        if (!tmpImage)
        {
            delete[] alphaChannel;
            return NULL;
        }
        converted = true;
    }

    const int sz = tmpImage->w * tmpImage->h;
    // Figure out whether the image uses its alpha layer
    if (tmpImage->format->palette == NULL)
    {
        const SDL_PixelFormat * const fmt = tmpImage->format;
        if (fmt->Amask)
        {
            for (int i = 0; i < sz; ++ i)
            {
                unsigned v = ((static_cast<Uint32*>(tmpImage->pixels))[i]
                    & fmt->Amask) >> fmt->Ashift;
                Uint8 a = (v << fmt->Aloss) + (v >> (8 - (fmt->Aloss << 1)));

                if (a != 255)
                    hasAlpha = true;

                alphaChannel[i] = a;
            }
        }
        else
        {
            if (SDL_ALPHA_OPAQUE != 255)
                hasAlpha = true;
            for (int i = 0; i < sz; ++ i)
                alphaChannel[i] = SDL_ALPHA_OPAQUE;
        }
    }
    else
    {
        if (SDL_ALPHA_OPAQUE != 255)
            hasAlpha = true;
        for (int i = 0; i < sz; ++ i)
            alphaChannel[i] = SDL_ALPHA_OPAQUE;
    }

    SDL_Surface *image;

    // Convert the surface to the current display format
    if (hasAlpha)
    {
        image = SDL_DisplayFormatAlpha(tmpImage);
    }
    else
    {
        image = SDL_DisplayFormat(tmpImage);

        // We also delete the alpha channel since
        // it's not used.
        delete[] alphaChannel;
        alphaChannel = 0;
    }

    if (!image)
    {
        logger->log1("Error: Image convert failed.");
        delete[] alphaChannel;
        return 0;
    }

    if (converted)
        SDL_FreeSurface(tmpImage);

//    SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
    return new Image(image, hasAlpha, alphaChannel);
}

#ifdef USE_OPENGL
Image *Image::_GLload(SDL_Surface *tmpImage)
{
    if (!tmpImage)
        return NULL;

    // Flush current error flag.
    glGetError();

    int width = tmpImage->w;
    int height = tmpImage->h;
    int realWidth = powerOfTwo(width);
    int realHeight = powerOfTwo(height);

    if (realWidth < width || realHeight < height)
    {
        logger->log("Warning: image too large, cropping to %dx%d texture!",
                    tmpImage->w, tmpImage->h);
    }

    // Make sure the alpha channel is not used, but copied to destination
    SDL_SetAlpha(tmpImage, 0, SDL_ALPHA_OPAQUE);

    // Determine 32-bit masks based on byte order
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *oldImage = tmpImage;
    tmpImage = SDL_CreateRGBSurface(SDL_SWSURFACE, realWidth, realHeight,
                                    32, rmask, gmask, bmask, amask);

    if (!tmpImage)
    {
        logger->log("Error, image convert failed: out of memory");
        return NULL;
    }

    SDL_BlitSurface(oldImage, NULL, tmpImage, NULL);

    GLuint texture;
    glGenTextures(1, &texture);
    if (mUseOpenGL == 1)
        OpenGLGraphics::bindTexture(mTextureType, texture);
    else if (mUseOpenGL == 2)
//        glBindTexture(mTextureType, texture);
        OpenGL1Graphics::bindTexture(mTextureType, texture);

    if (SDL_MUSTLOCK(tmpImage))
        SDL_LockSurface(tmpImage);

//    glTexImage2D(mTextureType, 0, GL_COMPRESSED_RGBA_ARB, tmpImage->w, tmpImage->h,
    glTexImage2D(mTextureType, 0, 4, tmpImage->w, tmpImage->h,
                0, GL_RGBA, GL_UNSIGNED_BYTE, tmpImage->pixels);

#ifdef DEBUG_OPENGL_LEAKS
    textures_count ++;
#endif
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (SDL_MUSTLOCK(tmpImage))
        SDL_UnlockSurface(tmpImage);

    SDL_FreeSurface(tmpImage);

    GLenum error = glGetError();
    if (error)
    {
        std::string errmsg = "Unknown error";
        switch (error)
        {
            case GL_INVALID_ENUM:
                errmsg = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errmsg = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errmsg = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                errmsg = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                errmsg = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                errmsg = "GL_OUT_OF_MEMORY";
                break;
            default:
                break;
        }
        logger->log("Error: Image GL import failed: %s", errmsg.c_str());
        return NULL;
    }

    return new Image(texture, width, height, realWidth, realHeight);
}

void Image::setLoadAsOpenGL(int useOpenGL)
{
    Image::mUseOpenGL = useOpenGL;
}

int Image::powerOfTwo(int input)
{
    int value;
    if (mTextureType == GL_TEXTURE_2D)
    {
        value = 1;
        while (value < input && value < mTextureSize)
            value <<= 1;
    }
    else
    {
        value = input;
    }
    return value >= mTextureSize ? mTextureSize : value;
}
#endif

Image *Image::getSubImage(int x, int y, int width, int height)
{
    // Create a new clipped sub-image
#ifdef USE_OPENGL
    if (mUseOpenGL)
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

//============================================================================
// SubImage Class
//============================================================================

SubImage::SubImage(Image *parent, SDL_Surface *image,
                   int x, int y, int width, int height):
    Image(image),
    mParent(parent)
{
    if (mParent)
    {
        mParent->incRef();
        mParent->SDLTerminateAlphaCache();
        mHasAlphaChannel = mParent->hasAlphaChannel();
        mAlphaChannel = mParent->SDLgetAlphaChannel();
    }
    else
    {
        mHasAlphaChannel = false;
        mAlphaChannel = 0;
    }

    // Set up the rectangle.
    mBounds.x = static_cast<short>(x);
    mBounds.y = static_cast<short>(y);
    mBounds.w = static_cast<Uint16>(width);
    mBounds.h = static_cast<Uint16>(height);
    mUseAlphaCache = false;
}

#ifdef USE_OPENGL
SubImage::SubImage(Image *parent, GLuint image,
                   int x, int y, int width, int height,
                   int texWidth, int texHeight):
    Image(image, width, height, texWidth, texHeight),
    mParent(parent)
{
    if (mParent)
        mParent->incRef();

    // Set up the rectangle.
    mBounds.x = static_cast<short>(x);
    mBounds.y = static_cast<short>(y);
    mBounds.w = static_cast<Uint16>(width);
    mBounds.h = static_cast<Uint16>(height);
}
#endif

SubImage::~SubImage()
{
    // Avoid destruction of the image
    mSDLSurface = 0;
    // Avoid possible destruction of its alpha channel
    mAlphaChannel = 0;
#ifdef USE_OPENGL
    mGLImage = 0;
#endif
    if (mParent)
    {
        mParent->decRef();
        mParent = 0;
    }
}

Image *SubImage::getSubImage(int x, int y, int w, int h)
{
    if (mParent)
        return mParent->getSubImage(mBounds.x + x, mBounds.y + y, w, h);
    else
        return NULL;
}
