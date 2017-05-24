/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "resources/openglimagehelper.h"

#ifdef USE_OPENGL

#include "graphicsmanager.h"

#include "render/mobileopengl2graphics.h"
#include "render/mobileopenglgraphics.h"
#include "render/modernopenglgraphics.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__
#include "render/normalopenglgraphics.h"
#include "render/safeopenglgraphics.h"

#include "render/opengl/mgl.h"
#include "render/opengl/mglcheck.h"

#include "resources/dye/dye.h"
#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#include "utils/checkutils.h"
#include "utils/sdlcheckutils.h"

#include <SDL_image.h>

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

int OpenGLImageHelper::mTextureType = 0;
int OpenGLImageHelper::mInternalTextureType = GL_RGBA8;
int OpenGLImageHelper::mTextureSize = 0;
bool OpenGLImageHelper::mBlur = true;
bool OpenGLImageHelper::mUseTextureSampler = false;

OpenGLImageHelper::~OpenGLImageHelper()
{
    glDeleteTextures(static_cast<GLsizei>(texturesSize - mFreeTextureIndex),
        &mTextures[mFreeTextureIndex]);
}

Image *OpenGLImageHelper::load(SDL_RWops *const rw, Dye const &dye)
{
    SDL_Surface *const tmpImage = loadPng(rw);
    if (!tmpImage)
    {
        reportAlways("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Surface *const surf = convertTo32Bit(tmpImage);
    MSDL_FreeSurface(tmpImage);
    if (!surf)
        return nullptr;

    uint32_t *pixels = static_cast<uint32_t *>(surf->pixels);
    const int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            const DyePalette *const pal = dye.getSPalete();
            if (pal)
                DYEPALETTEP(pal, SOGLColor)(pixels, surf->w * surf->h);
            break;
        }
        case 2:
        {
            const DyePalette *const pal = dye.getAPalete();
            if (pal)
                pal->replaceAOGLColor(pixels, surf->w * surf->h);
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

Image *OpenGLImageHelper::loadSurface(SDL_Surface *const tmpImage)
{
    return glLoad(tmpImage);
}

Image *OpenGLImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                            const int width, const int height,
                                            const float alpha)
{
    if (!tmpImage)
        return nullptr;

    Image *const img = glLoad(tmpImage, width, height);
    if (img)
        img->setAlpha(alpha);
    return img;
}

int OpenGLImageHelper::powerOfTwo(const int input)
{
    int value = 1;
    while (value < input && value < mTextureSize)
        value <<= 1;
    return value >= mTextureSize ? mTextureSize : value;
}

SDL_Surface *OpenGLImageHelper::convertSurfaceNormalize(SDL_Surface *tmpImage,
                                                        int width, int height)
{
    if (!tmpImage)
        return nullptr;

    int realWidth = powerOfTwo(width);
    int realHeight = powerOfTwo(height);

    if (realWidth < width || realHeight < height)
    {
        reportAlways("Warning: image too large, cropping to %dx%d texture!",
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
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
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

        if (!tmpImage)
        {
            reportAlways("Error, image convert failed: out of memory");
            return nullptr;
        }
        SDL_BlitSurface(oldImage, nullptr, tmpImage, nullptr);
    }
    return tmpImage;
}

SDL_Surface *OpenGLImageHelper::convertSurface(SDL_Surface *tmpImage,
                                               int width, int height)
{
    if (!tmpImage)
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
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
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

        if (!tmpImage)
        {
            reportAlways("Error, image convert failed: out of memory");
            return nullptr;
        }
        SDL_BlitSurface(oldImage, nullptr, tmpImage, nullptr);
    }
    return tmpImage;
}

void OpenGLImageHelper::bindTexture(const GLuint texture)
{
    switch (mUseOpenGL)
    {
#ifdef ANDROID
        case RENDER_NORMAL_OPENGL:
        case RENDER_SAFE_OPENGL:
        case RENDER_MODERN_OPENGL:
        case RENDER_GLES2_OPENGL:
            break;
        case RENDER_GLES_OPENGL:
            MobileOpenGLGraphics::bindTexture(mTextureType, texture);
            break;
#elif defined(__native_client__)
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
#else  // ANDROID

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
#endif  // ANDROID

        case RENDER_SOFTWARE:
        case RENDER_SDL2_DEFAULT:
        case RENDER_NULL:
        case RENDER_LAST:
        default:
            reportAlways("Unknown OpenGL backend: %d", mUseOpenGL);
            break;
    }
}

Image *OpenGLImageHelper::glLoad(SDL_Surface *tmpImage,
                                 int width, int height)
{
    if (!tmpImage)
        return nullptr;

    BLOCK_START("OpenGLImageHelper::glLoad")
    // Flush current error flag.
    graphicsManager.getLastError();

    if (!width)
        width = tmpImage->w;
    if (!height)
        height = tmpImage->h;

    SDL_Surface *oldImage = tmpImage;
    tmpImage = convertSurfaceNormalize(tmpImage, width, height);
    if (!tmpImage)
        return nullptr;

    const int realWidth = tmpImage->w;
    const int realHeight = tmpImage->h;

    const GLuint texture = getNewTexture();
    bindTexture(texture);

    if (SDL_MUSTLOCK(tmpImage))
        SDL_LockSurface(tmpImage);

    if (mUseOpenGL != RENDER_MODERN_OPENGL &&
        mUseOpenGL != RENDER_GLES_OPENGL &&
        mUseOpenGL != RENDER_GLES2_OPENGL)
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }

    if (!mUseTextureSampler)
    {
        if (mBlur)
        {
            mglTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef OPENGLERRORS
            graphicsManager.logError();
#endif  // OPENGLERRORS

            mglTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef OPENGLERRORS
            graphicsManager.logError();
#endif  // OPENGLERRORS
        }
        else
        {
            mglTexParameteri(mTextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#ifdef OPENGLERRORS
            graphicsManager.logError();
#endif  // OPENGLERRORS

            mglTexParameteri(mTextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#ifdef OPENGLERRORS
            graphicsManager.logError();
#endif  // OPENGLERRORS
        }
    }
#if !defined(ANDROID) && !defined(__native_client__)
    mglTexParameteri(mTextureType, GL_TEXTURE_MAX_LEVEL, 0);
#endif  // !defined(ANDROID) && !defined(__native_client__)

    mglTexImage2D(mTextureType, 0, mInternalTextureType,
        tmpImage->w, tmpImage->h,
        0, GL_RGBA, GL_UNSIGNED_BYTE, tmpImage->pixels);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS

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
    if (error)
    {
        std::string errmsg = GraphicsManager::errorToString(error);
        reportAlways("Error: Image GL import failed: %s (%u)",
            errmsg.c_str(), error);
//        return nullptr;
    }

    BLOCK_END("OpenGLImageHelper::glLoad")
    return new Image(texture, width, height, realWidth, realHeight);
}

void OpenGLImageHelper::initTextureSampler(const GLint id)
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

SDL_Surface *OpenGLImageHelper::create32BitSurface(int width,
                                                   int height) const
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    width = powerOfTwo(width);
    height = powerOfTwo(height);

    return MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}

GLuint OpenGLImageHelper::getNewTexture()
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

void OpenGLImageHelper::postInit()
{
    mglGenTextures(texturesSize, &mTextures[mFreeTextureIndex]);
}

void OpenGLImageHelper::invalidate(const GLuint textureId)
{
    if (isGLNotNull(mglInvalidateTexImage))
    {
        logger->log("invalidate: %u", textureId);
        mglInvalidateTexImage(textureId, 0);
    }
}

void OpenGLImageHelper::copySurfaceToImage(const Image *const image,
                                           const int x, const int y,
                                           SDL_Surface *surface) const
{
    if (!surface || !image)
        return;

    SDL_Surface *const oldSurface = surface;
    surface = convertSurface(surface, surface->w, surface->h);
    if (!surface)
        return;

    // +++ probably need combine
    // mglTextureSubImage2D and mglTextureSubImage2DEXT
    if (mglTextureSubImage2D)
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
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS

    if (surface != oldSurface)
        MSDL_FreeSurface(surface);
}

#endif  // USE_OPENGL
