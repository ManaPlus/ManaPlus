/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "resources/safeopenglimagehelper.h"

#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__SWITCH__)

#include "graphicsmanager.h"

#include "render/mobileopengl2graphics.h"
#include "render/mobileopenglgraphics.h"
#include "render/modernopenglgraphics.h"
#include "render/normalopenglgraphics.h"
#include "render/safeopenglgraphics.h"

#include "render/opengl/mgl.h"
#include "render/opengl/mglcheck.h"

#include "resources/dye/dye.h"
#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

int SafeOpenGLImageHelper::mTextureType = 0;
int SafeOpenGLImageHelper::mInternalTextureType = GL_RGBA8;
int SafeOpenGLImageHelper::mTextureSize = 0;
bool SafeOpenGLImageHelper::mBlur = true;
bool SafeOpenGLImageHelper::mUseTextureSampler = false;

SafeOpenGLImageHelper::~SafeOpenGLImageHelper()
{
    glDeleteTextures(static_cast<GLsizei>(texturesSize - mFreeTextureIndex),
        &mTextures[mFreeTextureIndex]);
}

Image *SafeOpenGLImageHelper::load(SDL_RWops *const rw,
                                   Dye const &dye)
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (tmpImage == nullptr)
    {
        logger->log("Error, image load failed: %s", SDL_GetError());
        return nullptr;
    }

    SDL_Surface *const surf = convertTo32Bit(tmpImage);
    MSDL_FreeSurface(tmpImage);
    if (surf == nullptr)
        return nullptr;

    uint32_t *pixels = static_cast<uint32_t *>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal != nullptr)
                DYEPALETTEP(pal, SOGLColor)(pixels, surf->w * surf->h);
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal != nullptr)
                DYEPALETTEP(pal, AOGLColor)(pixels, surf->w * surf->h);
            break;
        }
        case 0:
        default:
        {
            dye.normalOGLDye(pixels, surf->w * surf->h);
            break;
        }
    }

    Image *const image = loadSurface(surf);
    MSDL_FreeSurface(surf);
    return image;
}

Image *SafeOpenGLImageHelper::loadSurface(SDL_Surface *const tmpImage)
{
    return glLoad(tmpImage);
}

Image *SafeOpenGLImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                                const int width,
                                                const int height,
                                                const float alpha)
{
    if (tmpImage == nullptr)
        return nullptr;

    Image *const img = glLoad(tmpImage, width, height);
    if (img != nullptr)
        img->setAlpha(alpha);
    return img;
}

int SafeOpenGLImageHelper::powerOfTwo(const int input)
{
    int value = 1;
    while (value < input && value < mTextureSize)
        value <<= 1;
    return value >= mTextureSize ? mTextureSize : value;
}

SDL_Surface *SafeOpenGLImageHelper::convertSurfaceNormalize(SDL_Surface
                                                            *tmpImage,
                                                            int width,
                                                            int height)
{
    if (tmpImage == nullptr)
        return nullptr;

    int realWidth = powerOfTwo(width);
    int realHeight = powerOfTwo(height);

    if (realWidth < width || realHeight < height)
    {
        logger->log("Warning: image too large, cropping to %dx%d texture!",
                    tmpImage->w, tmpImage->h);
    }

#ifdef USE_SDL2
    SDL_SetSurfaceAlphaMod(tmpImage, SDL_ALPHA_OPAQUE);
#else  // USE_SDL2

    // Make sure the alpha channel is not used, but copied to destination
    SDL_SetAlpha(tmpImage, 0, SDL_ALPHA_OPAQUE);
#endif  // USE_SDL2

    // Determine 32-bit masks based on byte order
    uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000U;
    gmask = 0x00ff0000U;
    bmask = 0x0000ff00U;
    amask = 0x000000ffU;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0x000000ffU;
    gmask = 0x0000ff00U;
    bmask = 0x00ff0000U;
    amask = 0xff000000U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    if (tmpImage->format->BitsPerPixel != 32
        || realWidth != width || realHeight != height
        || rmask != tmpImage->format->Rmask
        || gmask != tmpImage->format->Gmask
        || amask != tmpImage->format->Amask)
    {
        SDL_Surface *oldImage = tmpImage;
#ifdef USE_SDL2
        SDL_SetSurfaceBlendMode(oldImage, SDL_BLENDMODE_NONE);
#endif  // USE_SDL2

        tmpImage = MSDL_CreateRGBSurface(SDL_SWSURFACE, realWidth, realHeight,
            32, rmask, gmask, bmask, amask);

        if (tmpImage == nullptr)
        {
            logger->log("Error, image convert failed: out of memory");
            return nullptr;
        }
        SDL_BlitSurface(oldImage, nullptr, tmpImage, nullptr);
    }
    return tmpImage;
}

SDL_Surface *SafeOpenGLImageHelper::convertSurface(SDL_Surface *tmpImage,
                                                   int width,
                                                   int height)
{
    if (tmpImage == nullptr)
        return nullptr;

#ifdef USE_SDL2
    SDL_SetSurfaceAlphaMod(tmpImage, SDL_ALPHA_OPAQUE);
#else  // USE_SDL2

    // Make sure the alpha channel is not used, but copied to destination
    SDL_SetAlpha(tmpImage, 0, SDL_ALPHA_OPAQUE);
#endif  // USE_SDL2

    // Determine 32-bit masks based on byte order
    uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000U;
    gmask = 0x00ff0000U;
    bmask = 0x0000ff00U;
    amask = 0x000000ffU;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0x000000ffU;
    gmask = 0x0000ff00U;
    bmask = 0x00ff0000U;
    amask = 0xff000000U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    if (tmpImage->format->BitsPerPixel != 32
        || rmask != tmpImage->format->Rmask
        || gmask != tmpImage->format->Gmask
        || amask != tmpImage->format->Amask)
    {
        SDL_Surface *oldImage = tmpImage;
#ifdef USE_SDL2
        SDL_SetSurfaceBlendMode(oldImage, SDL_BLENDMODE_NONE);
#endif  // USE_SDL2

        tmpImage = MSDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
            32, rmask, gmask, bmask, amask);

        if (tmpImage == nullptr)
        {
            logger->log("Error, image convert failed: out of memory");
            return nullptr;
        }
        SDL_BlitSurface(oldImage, nullptr, tmpImage, nullptr);
    }
    return tmpImage;
}

void SafeOpenGLImageHelper::bindTexture(const GLuint texture)
{
    switch (mUseOpenGL)
    {
#ifdef __native_client__
        case RENDER_NORMAL_OPENGL:
        case RENDER_MODERN_OPENGL:
        case RENDER_GLES_OPENGL:
            break;
        case RENDER_SAFE_OPENGL:
            SafeOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
        case RENDER_GLES2_OPENGL:
            MobileOpenGL2Graphics::bindTexture(mTextureType, texture);
            break;
#elif defined(ANDROID)
        case RENDER_NORMAL_OPENGL:
        case RENDER_MODERN_OPENGL:
        case RENDER_SAFE_OPENGL:
        case RENDER_GLES2_OPENGL:
            break;
        case RENDER_GLES_OPENGL:
            MobileOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
#else  // __native_client__
        case RENDER_NORMAL_OPENGL:
            NormalOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
        case RENDER_MODERN_OPENGL:
            ModernOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
        case RENDER_SAFE_OPENGL:
            SafeOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
        case RENDER_GLES_OPENGL:
            MobileOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
        case RENDER_GLES2_OPENGL:
            MobileOpenGL2Graphics::bindTexture(mTextureType, texture);
            break;
#endif  // __native_client__
        case RENDER_SOFTWARE:
        case RENDER_SDL2_DEFAULT:
        case RENDER_NULL:
        case RENDER_LAST:
        default:
            logger->log("Unknown OpenGL backend: %d", mUseOpenGL);
            break;
    }
}

Image *SafeOpenGLImageHelper::glLoad(SDL_Surface *tmpImage,
                                     int width,
                                     int height)
{
    if (tmpImage == nullptr)
        return nullptr;

    BLOCK_START("SafeOpenGLImageHelper::glLoad")
    // Flush current error flag.
    graphicsManager.getLastError();

    if (width == 0)
        width = tmpImage->w;
    if (height == 0)
        height = tmpImage->h;

    SDL_Surface *oldImage = tmpImage;
    tmpImage = convertSurfaceNormalize(tmpImage, width, height);
    if (tmpImage == nullptr)
        return nullptr;

    const int realWidth = tmpImage->w;
    const int realHeight = tmpImage->h;

    const GLuint texture = getNewTexture();
    bindTexture(texture);

    if (SDL_MUSTLOCK(tmpImage))
        SDL_LockSurface(tmpImage);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (!mUseTextureSampler)
    {
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
    }
#ifndef ANDROID
    glTexParameteri(mTextureType, GL_TEXTURE_MAX_LEVEL, 0);
#endif  // ANDROID

    glTexImage2D(mTextureType, 0, mInternalTextureType,
        tmpImage->w, tmpImage->h,
        0, GL_RGBA, GL_UNSIGNED_BYTE, tmpImage->pixels);

#ifdef DEBUG_OPENGL
/*
    disabled for now, because debugger can't show it
    if (isGLNotNull(mglLabelObject))
    {
        const char *const text = "image text";
        mglLabelObject(GL_TEXTURE, texture, strlen(text), text);
    }
*/
#endif  // DEBUG_OPENGL

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
#endif  // DEBUG_OPENGL_LEAKS

    if (SDL_MUSTLOCK(tmpImage))
        SDL_UnlockSurface(tmpImage);

    if (oldImage != tmpImage)
        MSDL_FreeSurface(tmpImage);

    GLenum error = graphicsManager.getLastError();
    if (error != 0U)
    {
        std::string errmsg = GraphicsManager::errorToString(error);
        logger->log("Error: Image GL import failed: %s (%u)",
            errmsg.c_str(), error);
//        return nullptr;
    }

    BLOCK_END("SafeOpenGLImageHelper::glLoad")
    return new Image(texture, width, height, realWidth, realHeight);
}

void SafeOpenGLImageHelper::initTextureSampler(const GLint id)
{
    if (mBlur)
    {
        mglSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        mglSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        mglSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mglSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

SDL_Surface *SafeOpenGLImageHelper::create32BitSurface(int width,
                                                       int height) const
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t rmask = 0xff000000U;
    const uint32_t gmask = 0x00ff0000U;
    const uint32_t bmask = 0x0000ff00U;
    const uint32_t amask = 0x000000ffU;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const uint32_t rmask = 0x000000ffU;
    const uint32_t gmask = 0x0000ff00U;
    const uint32_t bmask = 0x00ff0000U;
    const uint32_t amask = 0xff000000U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    width = powerOfTwo(width);
    height = powerOfTwo(height);

    return MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}

GLuint SafeOpenGLImageHelper::getNewTexture()
{
    GLuint texture = mTextures[mFreeTextureIndex];
    mFreeTextureIndex ++;
    if (mFreeTextureIndex == texturesSize)
    {
        mFreeTextureIndex = 0;
        postInit();
    }
    return texture;
}

void SafeOpenGLImageHelper::postInit()
{
    glGenTextures(texturesSize, &mTextures[mFreeTextureIndex]);
}

void SafeOpenGLImageHelper::invalidate(const GLuint textureId)
{
    if (isGLNotNull(mglInvalidateTexImage))
    {
        logger->log("invalidate: %u", textureId);
        mglInvalidateTexImage(textureId, 0);
    }
}

void SafeOpenGLImageHelper::copySurfaceToImage(const Image *const image,
                                               const int x,
                                               const int y,
                                               SDL_Surface *surface) const
{
    if (surface == nullptr || image == nullptr)
        return;

    SDL_Surface *const oldSurface = surface;
    surface = convertSurface(surface, surface->w, surface->h);
    if (surface == nullptr)
        return;

    // +++ probably need combine
    // mglTextureSubImage2D and mglTextureSubImage2DEXT
    if (mglTextureSubImage2D != nullptr)
    {
        mglTextureSubImage2D(image->mGLImage,
            0,
            x, y,
            surface->w, surface->h,
            GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    }
    else
    {
        mglTextureSubImage2DEXT(image->mGLImage,
            mTextureType, 0,
            x, y,
            surface->w, surface->h,
            GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    }

    if (surface != oldSurface)
        MSDL_FreeSurface(surface);
}

#endif  // defined(USE_OPENGL) && !defined(ANDROID)
