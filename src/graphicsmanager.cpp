/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "graphicsmanager.h"

#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/image.h"

#include "utils/stringutils.h"

#include "debug.h"

GraphicsManager graphicsManager;

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::initGraphics(bool noOpenGL)
{
#ifdef USE_OPENGL
    int useOpenGL = 0;
    if (!noOpenGL)
        useOpenGL = config.getIntValue("opengl");

    // Setup image loading for the right image format
    Image::setLoadAsOpenGL(useOpenGL);
    GraphicsVertexes::setLoadAsOpenGL(useOpenGL);

    // Create the graphics context
    switch (useOpenGL)
    {
        case 0:
            mainGraphics = new Graphics;
            break;
        case 1:
        default:
            mainGraphics = new OpenGLGraphics;
            break;
        case 2:
            mainGraphics = new OpenGL1Graphics;
            break;
    };

#else
    // Create the graphics context
    mainGraphics = new Graphics;
#endif
}

void GraphicsManager::updateExtensions(const char *extensions)
{
    mExtensions.clear();
    splitToStringSet(mExtensions, extensions, ' ');
}

bool GraphicsManager::supportExtension(const std::string ext)
{
    return mExtensions.find(ext) != mExtensions.end();
}

void GraphicsManager::updateTextureFormat()
{
#ifdef USE_OPENGL
    if (!config.getBoolValue("compresstextures"))
        return;

    if (supportExtension("GL_ARB_texture_compression"))
    {
        if (supportExtension("GL_EXT_texture_compression_s3tc")
            || supportExtension("3DFX_texture_compression_FXT1"))
        {
            GLint num;
            GLint *formats = nullptr;
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
            logger->log("support %d compressed formats", num);
            formats = new GLint[num > 10 ? num : 10];
            glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
            for (int f = 0; f < num; f ++)
            {
                if (formats[f] == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
                {
                    delete []formats;
                    Image::setInternalTextureType(
                        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
                    logger->log("using s3tc texture compression");
                    return;
                }
                else if (formats[f] == GL_COMPRESSED_RGBA_FXT1_3DFX)
                {
                    delete []formats;
                    Image::setInternalTextureType(
                        GL_COMPRESSED_RGBA_FXT1_3DFX);
                    logger->log("using fxt1 texture compression");
                    return;
                }
            }
            Image::setInternalTextureType(GL_COMPRESSED_RGBA_ARB);
            logger->log("using texture compression");
        }
        else
        {
            Image::setInternalTextureType(GL_COMPRESSED_RGBA_ARB);
            logger->log("using texture compression");
        }
    }
#endif
}

void GraphicsManager::logString(const char *format, int num)
{
#ifdef USE_OPENGL
    const char *str = reinterpret_cast<const char*>(glGetString(num));
    if (!str)
        logger->log(format, "?");
    else
        logger->log(format, str);
#endif
}
