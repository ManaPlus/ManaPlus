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

#define GL_GLEXT_PROTOTYPES 1
#ifndef WIN32
#include "GL/glx.h"
// hack to hide warnings
#undef GL_GLEXT_VERSION
#undef GL_GLEXT_PROTOTYPES
//#else
//#include "GL/glext.h"
//#undef GL_GLEXT_VERSION
//#undef WIN32_LEAN_AND_MEAN
#endif

#include "configuration.h"
#include "graphics.h"
#include "graphicsvertexes.h"
#include "logger.h"
#include "mgl.h"

#include "resources/fboinfo.h"
#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"
#include "resources/sdlimagehelper.h"

#include "utils/paths.h"
#include "utils/stringutils.h"

#include "test/testmain.h"

#include "debug.h"

GraphicsManager graphicsManager;

GraphicsManager::GraphicsManager() :
    mMinor(0),
    mMajor(0)
{
}

GraphicsManager::~GraphicsManager()
{
}

int GraphicsManager::startDetection()
{
#ifdef USE_OPENGL
    TestMain *test = new TestMain();
    test->exec(false);
    return test->getConfig().getValueInt("opengl", -1);
#else
    return 0;
#endif
}

bool GraphicsManager::detectGraphics()
{
#ifdef USE_OPENGL
    logger->log("start detecting best mode...");
    logger->log("enable opengl mode");
    SDL_SetVideoMode(100, 100, 0, SDL_ANYFORMAT | SDL_OPENGL);

    std::string vendor = getGLString(GL_VENDOR);
    std::string renderer = getGLString(GL_RENDERER);
    std::string version = getGLString(GL_VERSION);
    logger->log("gl vendor: %s", vendor.c_str());
    logger->log("gl renderer: %s", renderer.c_str());
    logger->log("gl version: %s", version.c_str());
    sscanf(version.c_str(), "%5d.%5d", &mMajor, &mMinor);

    char const *glExtensions = reinterpret_cast<char const *>(
        glGetString(GL_EXTENSIONS));
    updateExtensions(glExtensions);

    int mode = 1;

    // detecting features by known renderers or vendors
    if (findI(renderer, "gdi generic") != std::string::npos)
    {
        // windows gdi OpenGL emulation
        logger->log("detected gdi drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(renderer, "Software Rasterizer") != std::string::npos)
    {
        // software OpenGL emulation
        logger->log("detected software drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(renderer, "Indirect") != std::string::npos)
    {
        // indirect OpenGL drawing
        logger->log("detected indirect drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(vendor, "VMWARE") != std::string::npos)
    {
        // vmware emulation
        logger->log("detected VMWARE driver");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(renderer, "LLVM") != std::string::npos)
    {
        // llvm opengl emulation
        logger->log("detected llvm driver");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(vendor, "NVIDIA") != std::string::npos)
    {
        // hope it can work well
        logger->log("detected NVIDIA driver");
        mode = 1;
    }

    // detecting feature based on OpenGL version
    if (!checkGLVersion(1, 1))
    {
        // very old OpenGL version
        logger->log("OpenGL version too old");
        mode = 0;
    }

    if (mode > 0 && findI(version, "Mesa") != std::string::npos)
    {
        // Mesa detected
        config.setValue("compresstextures", true);
    }

    config.setValue("opengl", mode);
    config.setValue("videoconfigured", true);

    logger->log("detection complete");
    return true;
#else
    return false;
#endif
}

void GraphicsManager::initGraphics(bool noOpenGL)
{
#ifdef USE_OPENGL
    int useOpenGL = 0;
    if (!noOpenGL)
        useOpenGL = config.getIntValue("opengl");

    // Setup image loading for the right image format
    OpenGLImageHelper::setLoadAsOpenGL(useOpenGL);
    GraphicsVertexes::setLoadAsOpenGL(useOpenGL);

    // Create the graphics context
    switch (useOpenGL)
    {
        case 0:
            imageHelper = new SDLImageHelper;
            mainGraphics = new Graphics;
            break;
        case 1:
        default:
            imageHelper = new OpenGLImageHelper;
            mainGraphics = new NormalOpenGLGraphics;
            break;
        case 2:
            imageHelper = new OpenGLImageHelper;
            mainGraphics = new SafeOpenGLGraphics;
            break;
    };
#else
    // Create the graphics context
    imageHelper = new SDLImageHelper;
    mainGraphics = new Graphics;
#endif
}

Graphics *GraphicsManager::createGraphics()
{
#ifdef USE_OPENGL
    switch (config.getIntValue("opengl"))
    {
        case 0:
            return new Graphics;
        case 1:
        default:
            return new NormalOpenGLGraphics;
        case 2:
            return new SafeOpenGLGraphics;
    };
#else
    return new Graphics;
#endif
}

void GraphicsManager::updateExtensions(const char *extensions)
{
    mExtensions.clear();
    splitToStringSet(mExtensions, extensions, ' ');
}

bool GraphicsManager::supportExtension(const std::string &ext)
{
    return mExtensions.find(ext) != mExtensions.end();
}

void GraphicsManager::updateTextureFormat()
{
#ifdef USE_OPENGL
    if (config.getBoolValue("compresstextures"))
    {
        // using extensions if can
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
                        OpenGLImageHelper::setInternalTextureType(
                            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
                        logger->log1("using s3tc texture compression");
                        return;
                    }
                    else if (formats[f] == GL_COMPRESSED_RGBA_FXT1_3DFX)
                    {
                        delete []formats;
                        OpenGLImageHelper::setInternalTextureType(
                            GL_COMPRESSED_RGBA_FXT1_3DFX);
                        logger->log1("using fxt1 texture compression");
                        return;
                    }
                }
                OpenGLImageHelper::setInternalTextureType(
                    GL_COMPRESSED_RGBA_ARB);
                logger->log1("using texture compression");
                return;
            }
            else
            {
                OpenGLImageHelper::setInternalTextureType(
                    GL_COMPRESSED_RGBA_ARB);
                logger->log1("using texture compression");
                return;
            }
        }
    }

    // using default formats
    if (config.getBoolValue("newtextures"))
    {
        OpenGLImageHelper::setInternalTextureType(GL_RGBA);
        logger->log1("using RGBA texture format");
    }
    else
    {
        OpenGLImageHelper::setInternalTextureType(4);
        logger->log1("using 4 texture format");
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

std::string GraphicsManager::getGLString(int num) const
{
#ifdef USE_OPENGL
    const char *str = reinterpret_cast<const char*>(glGetString(num));
    if (str)
        return str;
    else
        return "";
#else
    return "";
#endif
}

void GraphicsManager::setGLVersion()
{
    sscanf(getGLString(GL_VERSION).c_str(), "%5d.%5d", &mMajor, &mMinor);
}

void GraphicsManager::setVideoMode()
{
    const int width = config.getIntValue("screenwidth");
    const int height = config.getIntValue("screenheight");
    const int bpp = 0;
    const bool fullscreen = config.getBoolValue("screen");
    const bool hwaccel = config.getBoolValue("hwaccel");
    const bool enableResize = config.getBoolValue("enableresize");
    const bool noFrame = config.getBoolValue("noframe");

    // Try to set the desired video mode
    if (!mainGraphics->setVideoMode(width, height, bpp,
        fullscreen, hwaccel, enableResize, noFrame))
    {
        logger->log(strprintf("Couldn't set %dx%dx%d video mode: %s",
            width, height, bpp, SDL_GetError()));

        const int oldWidth = config.getValueInt("oldscreenwidth", -1);
        const int oldHeight = config.getValueInt("oldscreenheight", -1);
        const int oldFullscreen = config.getValueInt("oldscreen", -1);
        if (oldWidth != -1 && oldHeight != -1 && oldFullscreen != -1)
        {
            config.deleteKey("oldscreenwidth");
            config.deleteKey("oldscreenheight");
            config.deleteKey("oldscreen");

            config.setValueInt("screenwidth", oldWidth);
            config.setValueInt("screenheight", oldHeight);
            config.setValue("screen", oldFullscreen == 1);
            if (!mainGraphics->setVideoMode(oldWidth, oldHeight, bpp,
                oldFullscreen, hwaccel, enableResize, noFrame))
            {
                logger->safeError(strprintf("Couldn't restore %dx%dx%d "
                    "video mode: %s", oldWidth, oldHeight, bpp,
                    SDL_GetError()));
            }
        }
    }
}


bool GraphicsManager::checkGLVersion(int major, int minor) const
{
    return mMajor > major || (mMajor == major && mMinor >= minor);
}

void GraphicsManager::createFBO(int width, int height, FBOInfo *fbo)
{
#ifdef USE_OPENGL
    if (!fbo)
        return;

    // create a texture object
    glGenTextures(1, &fbo->textureId);
    glBindTexture(OpenGLImageHelper::mTextureType, fbo->textureId);
    glTexParameterf(OpenGLImageHelper::mTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(OpenGLImageHelper::mTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(OpenGLImageHelper::mTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(OpenGLImageHelper::mTextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(OpenGLImageHelper::mTextureType, 0, GL_RGBA8, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(OpenGLImageHelper::mTextureType, 0);

    // create a renderbuffer object to store depth info
    mglGenRenderbuffers(1, &fbo->rboId);
    mglBindRenderbuffer(GL_RENDERBUFFER, fbo->rboId);
    mglRenderbufferStorage(GL_RENDERBUFFER,
        GL_DEPTH_COMPONENT, width, height);
    mglBindRenderbuffer(GL_RENDERBUFFER, 0);

    // create a framebuffer object
    mglGenFramebuffers(1, &fbo->fboId);
    mglBindFramebuffer(GL_FRAMEBUFFER, fbo->fboId);

    // attach the texture to FBO color attachment point
    mglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        OpenGLImageHelper::mTextureType, fbo->textureId, 0);

    // attach the renderbuffer to depth attachment point
    mglFramebufferRenderbuffer(GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->rboId);

    mglBindFramebuffer(GL_FRAMEBUFFER, fbo->fboId);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void GraphicsManager::deleteFBO(FBOInfo *fbo)
{
#ifdef USE_OPENGL
    if (!fbo)
        return;

    mglBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (fbo->fboId)
    {
        mglDeleteFramebuffers(1, &fbo->fboId);
        fbo->fboId = 0;
    }
    mglBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (fbo->rboId)
    {
        mglDeleteRenderbuffers(1, &fbo->rboId);
        fbo->rboId = 0;
    }
    if (fbo->textureId)
    {
        glDeleteTextures(1, &fbo->textureId);
        fbo->textureId = 0;
    }
#endif
}

#ifdef WIN32
#define getFunction(name) wglGetProcAddress(name);
#else
#define getFunction(name) glXGetProcAddress((const GLubyte*)(name))
#endif

#define assignFunction(func, name) m##func = (func##_t)getFunction(name)

void GraphicsManager::initOpenGLFunctions()
{
    if (!checkGLVersion(1, 1))
        return;

    if (supportExtension("GL_ARB_framebuffer_object"))
    {   // frame buffer supported
        assignFunction(glGenRenderbuffers, "glGenRenderbuffers");
        assignFunction(glBindRenderbuffer, "glBindRenderbuffer");
        assignFunction(glRenderbufferStorage, "glRenderbufferStorage");
        assignFunction(glGenFramebuffers, "glGenFramebuffers");
        assignFunction(glBindFramebuffer, "glBindFramebuffer");
        assignFunction(glFramebufferTexture2D, "glFramebufferTexture2D");
        assignFunction(glFramebufferRenderbuffer, "glFramebufferRenderbuffer");
        assignFunction(glDeleteFramebuffers, "glDeleteFramebuffers");
        assignFunction(glDeleteRenderbuffers, "glDeleteRenderbuffers");
    }
    else if (supportExtension("GL_EXT_framebuffer_object"))
    {   // old frame buffer extension
        assignFunction(glGenRenderbuffers, "glGenRenderbuffersEXT");
        assignFunction(glBindRenderbuffer, "glBindRenderbufferEXT");
        assignFunction(glRenderbufferStorage, "glRenderbufferStorageEXT");
        assignFunction(glGenFramebuffers, "glGenFramebuffersEXT");
        assignFunction(glBindFramebuffer, "glBindFramebufferEXT");
        assignFunction(glFramebufferTexture2D, "glFramebufferTexture2DEXT");
        assignFunction(glFramebufferRenderbuffer,
            "glFramebufferRenderbufferEXT");
        assignFunction(glDeleteFramebuffers, "glDeleteFramebuffersEXT");
        assignFunction(glDeleteRenderbuffers, "glDeleteRenderbuffersEXT");
    }
    else
    {   // no frame buffer support
        config.setValue("usefbo", false);
    }
}
