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

#include "resources/openglimagehelper.h"

#ifdef USE_OPENGL

#include "resources/dye.h"
#include "resources/resourcemanager.h"

#include "normalopenglgraphics.h"
#include "safeopenglgraphics.h"

#include "client.h"
#include "logger.h"

#include "utils/stringutils.h"

#include "resources/image.h"

#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include "debug.h"

int OpenGLImageHelper::mTextureType = 0;
int OpenGLImageHelper::mInternalTextureType = GL_RGBA8;
int OpenGLImageHelper::mTextureSize = 0;
bool OpenGLImageHelper::mBlur = true;
int OpenGLImageHelper::mUseOpenGL = 0;

Resource *OpenGLImageHelper::load(SDL_RWops *rw, Dye const &dye)
{
    SDL_Surface *tmpImage = IMG_Load_RW(rw, 1);

    if (!tmpImage)
    {
        logger->log("Error, image load failed: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Surface *surf = convertTo32Bit(tmpImage);
    SDL_FreeSurface(tmpImage);

    uint32_t *pixels = static_cast<uint32_t *>(surf->pixels);
    int type = dye.getType();

    switch (type)
    {
        case 1:
        {
            DyePalette *pal = dye.getSPalete();

            if (pal)
            {
                for (uint32_t *p_end = pixels + surf->w * surf->h;
                    pixels != p_end; ++pixels)
                {
                    uint8_t *p = reinterpret_cast<uint8_t *>(pixels);
                    const int alpha = *p & 255;
                    if (!alpha)
                        continue;
                    pal->replaceSOGLColor(p);
                }
            }
            break;
        }
        case 2:
        {
            DyePalette *pal = dye.getAPalete();
            if (pal)
            {
                for (uint32_t *p_end = pixels + surf->w * surf->h;
                    pixels != p_end; ++pixels)
                {
                    pal->replaceAOGLColor(reinterpret_cast<uint8_t *>(pixels));
                }
            }
            break;
        }
        case 0:
        default:
        {
            for (uint32_t *p_end = pixels + surf->w * surf->h;
                 pixels != p_end; ++pixels)
            {
                const uint32_t p = *pixels;
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
            break;
        }
    }

    Image *image = load(surf);
    SDL_FreeSurface(surf);
    return image;
}

Image *OpenGLImageHelper::load(SDL_Surface *tmpImage)
{
    return glLoad(tmpImage);
}

Image *OpenGLImageHelper::createTextSurface(SDL_Surface *tmpImage, float alpha)
{
    if (!tmpImage)
        return nullptr;

    Image *img = glLoad(tmpImage);
    if (img)
        img->setAlpha(alpha);
    return img;
}

int OpenGLImageHelper::powerOfTwo(int input)
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

Image *OpenGLImageHelper::glLoad(SDL_Surface *tmpImage)
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
    uint32_t rmask, gmask, bmask, amask;
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
        NormalOpenGLGraphics::bindTexture(mTextureType, texture);
    else if (mUseOpenGL == 2)
        SafeOpenGLGraphics::bindTexture(mTextureType, texture);

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

void OpenGLImageHelper::setLoadAsOpenGL(int useOpenGL)
{
    OpenGLImageHelper::mUseOpenGL = useOpenGL;
}

int OpenGLImageHelper::useOpenGL()
{
    return mUseOpenGL;
}

#endif
