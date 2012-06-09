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

#include "resources/imagehelper.h"

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#ifdef USE_OPENGL
#include "openglgraphics.h"
#include "opengl1graphics.h"
#endif

#include "client.h"
#include "logger.h"
#include "main.h"

#include "utils/stringutils.h"

#include "resources/image.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

#ifdef USE_OPENGL
int ImageHelper::mUseOpenGL = 0;
int ImageHelper::mTextureType = 0;
int ImageHelper::mInternalTextureType = GL_RGBA8;
int ImageHelper::mTextureSize = 0;
bool ImageHelper::mBlur = true;
#endif
bool ImageHelper::mEnableAlphaCache = false;
bool ImageHelper::mEnableAlpha = true;

Resource *ImageHelper::load(SDL_RWops *rw)
{
    SDL_Surface *tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    Image *image = load(tmpImage);

    SDL_FreeSurface(tmpImage);
    return image;
}

Resource *ImageHelper::load(SDL_RWops *rw, Dye const &dye)
{
    SDL_Surface *tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Surface *surf = nullptr;
    SDL_PixelFormat rgba;
    rgba.palette = nullptr;
    rgba.BitsPerPixel = 32;
    rgba.BytesPerPixel = 4;
    rgba.colorkey = 0;
    rgba.alpha = 255;
    if (mUseOpenGL)
    {
        surf = convertTo32Bit(tmpImage);
        SDL_FreeSurface(tmpImage);

        Uint32 *pixels = static_cast<Uint32 *>(surf->pixels);
        DyePalette *pal = dye.getSPalete();

        if (pal)
        {
            for (Uint32 *p_end = pixels + surf->w * surf->h;
                pixels != p_end; ++pixels)
            {
                Uint8 *p = (Uint8 *)pixels;
                const int alpha = *p & 255;
                if (!alpha)
                    continue;
                pal->replaceOGLColor(p);
            }
        }
        else
        {
            for (Uint32 *p_end = pixels + surf->w * surf->h;
                pixels != p_end; ++pixels)
            {
                const Uint32 p = *pixels;
                const int alpha = (p >> 24) & 255;
                if (!alpha)
                    continue;
                int v[3];
                v[0] = (p) & 255;
                v[1] = (p >> 8) & 255;
                v[2] = (p >> 16 ) & 255;
                dye.update(v);
                *pixels = (v[0]) | (v[1] << 8) | (v[2] << 16) | (alpha << 24);
            }
        }
    }
    else
    {
        rgba.Rmask = 0xFF000000; rgba.Rloss = 0; rgba.Rshift = 24;
        rgba.Gmask = 0x00FF0000; rgba.Gloss = 0; rgba.Gshift = 16;
        rgba.Bmask = 0x0000FF00; rgba.Bloss = 0; rgba.Bshift = 8;
        rgba.Amask = 0x000000FF; rgba.Aloss = 0; rgba.Ashift = 0;

        surf = SDL_ConvertSurface(tmpImage, &rgba, SDL_SWSURFACE);
        SDL_FreeSurface(tmpImage);

        Uint32 *pixels = static_cast<Uint32 *>(surf->pixels);
        DyePalette *pal = dye.getSPalete();

        if (pal)
        {
            for (Uint32 *p_end = pixels + surf->w * surf->h;
                pixels != p_end; ++pixels)
            {
                Uint8 *p = (Uint8 *)pixels;
                const int alpha = *p & 255;
                if (!alpha)
                    continue;
                pal->replaceColor(p + 1);
            }
        }
        else
        {
            for (Uint32 *p_end = pixels + surf->w * surf->h;
                pixels != p_end; ++pixels)
            {
                const Uint32 p = *pixels;
                const int alpha = p & 255;
                if (!alpha)
                    continue;
                int v[3];
                v[0] = (p >> 24) & 255;
                v[1] = (p >> 16) & 255;
                v[2] = (p >> 8 ) & 255;
                dye.update(v);
                *pixels = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | alpha;
            }
        }
    }

    Image *image = load(surf);
    SDL_FreeSurface(surf);
    return image;
}

Image *ImageHelper::load(SDL_Surface *tmpImage)
{
#ifdef USE_OPENGL
    if (mUseOpenGL)
        return _GLload(tmpImage);
#endif
    return _SDLload(tmpImage);
}

Image *ImageHelper::createTextSurface(SDL_Surface *tmpImage, float alpha)
{
    if (!tmpImage)
        return nullptr;

    Image *img;
#ifdef USE_OPENGL
    if (mUseOpenGL)
    {
        img = _GLload(tmpImage);
        if (img)
            img->setAlpha(alpha);
        return img;
    }
#endif

    bool hasAlpha = false;
    bool converted = false;

    const int sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    Uint8 *alphaChannel = new Uint8[sz];

    const SDL_PixelFormat * const fmt = tmpImage->format;
    if (fmt->Amask)
    {
        for (int i = 0; i < sz; ++ i)
        {
            Uint32 c = (static_cast<Uint32*>(tmpImage->pixels))[i];

            unsigned v = (c & fmt->Amask) >> fmt->Ashift;
            Uint8 a = (v << fmt->Aloss) + (v >> (8 - (fmt->Aloss << 1)));

            Uint8 a2 = static_cast<Uint8>(static_cast<float>(a) * alpha);

            c &= ~fmt->Amask;
            c |= ((a2 >> fmt->Aloss) << fmt->Ashift & fmt->Amask);
            (static_cast<Uint32*>(tmpImage->pixels))[i] = c;

            if (a != 255)
                hasAlpha = true;

            alphaChannel[i] = a;
        }
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
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        logger->log1("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    if (converted)
        SDL_FreeSurface(tmpImage);

    img = new Image(image, hasAlpha, alphaChannel);
    img->mAlpha = alpha;
    return img;
}

SDL_Surface* ImageHelper::convertTo32Bit(SDL_Surface* tmpImage)
{
    if (!tmpImage)
        return nullptr;
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette = nullptr;
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

SDL_Surface* ImageHelper::SDLDuplicateSurface(SDL_Surface* tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return nullptr;

    return SDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *ImageHelper::_SDLload(SDL_Surface *tmpImage)
{
    if (!tmpImage)
        return nullptr;

    bool hasAlpha = false;
    bool converted = false;

    if (tmpImage->format->BitsPerPixel != 32)
    {
        tmpImage = convertTo32Bit(tmpImage);

        if (!tmpImage)
            return nullptr;
        converted = true;
    }

    const int sz = tmpImage->w * tmpImage->h;

    // The alpha channel to be filled with alpha values
    Uint8 *alphaChannel = new Uint8[sz];

    // Figure out whether the image uses its alpha layer
    if (!tmpImage->format->palette)
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
            {
                hasAlpha = true;
                memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
            }
        }
    }
    else
    {
        if (SDL_ALPHA_OPAQUE != 255)
        {
            hasAlpha = true;
            memset(alphaChannel, SDL_ALPHA_OPAQUE, sz);
        }
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
        delete [] alphaChannel;
        alphaChannel = nullptr;
    }

    if (!image)
    {
        logger->log1("Error: Image convert failed.");
        delete [] alphaChannel;
        return nullptr;
    }

    if (converted)
        SDL_FreeSurface(tmpImage);

    return new Image(image, hasAlpha, alphaChannel);
}

#ifdef USE_OPENGL
int ImageHelper::powerOfTwo(int input)
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

Image *ImageHelper::_GLload(SDL_Surface *tmpImage)
{
    if (!tmpImage)
        return nullptr;

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

    SDL_Surface *oldImage = nullptr;
    if (tmpImage->format->BitsPerPixel != 32
        || realWidth != width || realHeight != height
        || rmask != tmpImage->format->Rmask
        || gmask != tmpImage->format->Gmask
        || amask != tmpImage->format->Amask)
    {
        oldImage = tmpImage;
        tmpImage = SDL_CreateRGBSurface(SDL_SWSURFACE, realWidth, realHeight,
            32, rmask, gmask, bmask, amask);

        if (!tmpImage)
        {
            logger->log("Error, image convert failed: out of memory");
            return nullptr;
        }
        SDL_BlitSurface(oldImage, nullptr, tmpImage, nullptr);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    if (mUseOpenGL == 1)
        OpenGLGraphics::bindTexture(mTextureType, texture);
    else if (mUseOpenGL == 2)
        OpenGL1Graphics::bindTexture(mTextureType, texture);

    if (SDL_MUSTLOCK(tmpImage))
        SDL_LockSurface(tmpImage);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if (mBlur)
    {
        glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexImage2D(mTextureType, 0, mInternalTextureType,
        tmpImage->w, tmpImage->h,
        0, GL_RGBA, GL_UNSIGNED_BYTE, tmpImage->pixels);

/*
    GLint compressed;
    glGetTexLevelParameteriv(mTextureType, 0,
        GL_TEXTURE_COMPRESSED_ARB, &compressed);
    if (compressed)
        logger->log("image compressed");
    else
        logger->log("image not compressed");
*/

#ifdef DEBUG_OPENGL_LEAKS
    textures_count ++;
#endif

    if (SDL_MUSTLOCK(tmpImage))
        SDL_UnlockSurface(tmpImage);

    if (oldImage)
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
        return nullptr;
    }

    return new Image(texture, width, height, realWidth, realHeight);
}

void ImageHelper::setLoadAsOpenGL(int useOpenGL)
{
    ImageHelper::mUseOpenGL = useOpenGL;
}

#endif

void ImageHelper::dumpSurfaceFormat(SDL_Surface *image)
{
    if (image->format)
    {
        const SDL_PixelFormat * const format = image->format;
        logger->log("Bytes per pixel: %d", format->BytesPerPixel);
        logger->log("Alpha: %d", format->alpha);
        logger->log("Loss: %02x, %02x, %02x, %02x", (int)format->Rloss,
            (int)format->Gloss, (int)format->Bloss, (int)format->Aloss);
        logger->log("Shift: %02x, %02x, %02x, %02x", (int)format->Rshift,
            (int)format->Gshift, (int)format->Bshift, (int)format->Ashift);
        logger->log("Mask: %08x, %08x, %08x, %08x", format->Rmask,
            format->Gmask, format->Bmask, format->Amask);
    }
}

int ImageHelper::useOpenGL()
{
#ifdef USE_OPENGL
    return mUseOpenGL;
#else
    return 0;
#endif
}
