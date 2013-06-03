/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#ifdef USE_OPENGL

#ifndef WIN32
#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES/glext.h>
#include <EGL/egl.h>
#include <SDL_android.h>
#else
#include "GL/glx.h"
#endif
#else
#endif

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

#include <SDL_syswm.h>

#include "debug.h"

#ifdef USE_OPENGL
#ifndef GL_MAX_RENDERBUFFER_SIZE
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#endif

#ifdef WIN32
#define getFunction(name) wglGetProcAddress(name)
#elif defined ANDROID
#define getFunction(name) eglGetProcAddress(name)
#elif defined __APPLE__
#define getFunction(name) nullptr
#else
#define getFunction(name) glXGetProcAddress(\
    reinterpret_cast<const GLubyte*>(name))
#endif

#define assignFunction(func, name) m##func \
    = reinterpret_cast<func##_t>(getFunction(name))
#endif

GraphicsManager graphicsManager;

const int densitySize = 6;

const std::string densityNames[] =
{
    "low",
    "medium",
    "tv",
    "high",
    "xhigh",
    "xxhigh"
};

GraphicsManager::GraphicsManager() :
    mExtensions(),
    mPlatformExtensions(),
    mGlVersionString(),
    mGlVendor(),
    mGlRenderer(),
    mMinor(0),
    mMajor(0),
    mPlatformMinor(0),
    mPlatformMajor(0),
    mMaxVertices(500),
    mMaxFboSize(0),
    mMaxWidth(0),
    mMaxHeight(0),
    mWidthMM(0),
    mHeightMM(0),
    mDensity(-1),
#ifdef USE_OPENGL
    mUseTextureSampler(true),
    mTextureSampler(0),
    mSupportDebug(0),
#endif
    mUseAtlases(false)
{
}

GraphicsManager::~GraphicsManager()
{
#ifdef USE_OPENGL
    if (mglGenSamplers && mTextureSampler)
        mglDeleteSamplers(1, &mTextureSampler);
#endif
}

#ifdef USE_OPENGL
TestMain *GraphicsManager::startDetection()
{
    TestMain *const test = new TestMain();
    test->exec(false);
    return test;
}

int GraphicsManager::detectGraphics()
{
    logger->log1("start detecting best mode...");
    logger->log1("enable opengl mode");
    int textureSampler = 0;
    int compressTextures = 0;
    SDL_SetVideoMode(100, 100, 0, SDL_ANYFORMAT | SDL_OPENGL);

    initOpenGL();
    logVersion();

    int mode = 1;

    // detecting features by known renderers or vendors
    if (findI(mGlRenderer, "gdi generic") != std::string::npos)
    {
        // windows gdi OpenGL emulation
        logger->log("detected gdi drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(mGlRenderer, "Software Rasterizer") != std::string::npos)
    {
        // software OpenGL emulation
        logger->log("detected software drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(mGlRenderer, "Indirect") != std::string::npos)
    {
        // indirect OpenGL drawing
        logger->log("detected indirect drawing");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(mGlVendor, "VMWARE") != std::string::npos)
    {
        // vmware emulation
        logger->log("detected VMWARE driver");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(mGlRenderer, "LLVM") != std::string::npos)
    {
        // llvm opengl emulation
        logger->log("detected llvm driver");
        logger->log("disable OpenGL");
        mode = 0;
    }
    else if (findI(mGlVendor, "NVIDIA") != std::string::npos)
    {
        // hope it can work well
        logger->log("detected NVIDIA driver");
        config.setValue("useTextureSampler", true);
        textureSampler = 1;
        mode = 1;
    }

    // detecting feature based on OpenGL version
    if (!checkGLVersion(1, 1))
    {
        // very old OpenGL version
        logger->log("OpenGL version too old");
        mode = 0;
    }

    if (mode > 0 && findI(mGlVersionString, "Mesa") != std::string::npos)
    {
        // Mesa detected. In latest Mesa look like compression broken.
        config.setValue("compresstextures", false);
        compressTextures = 0;
    }

    config.setValue("opengl", mode);
    config.setValue("videoconfigured", true);
    config.write();

    logger->log("detection complete");
    return mode | (1024 * textureSampler) | (2048 * compressTextures);
}

void GraphicsManager::initGraphics(const bool noOpenGL)
{
    int useOpenGL = 0;
    if (!noOpenGL)
        useOpenGL = config.getIntValue("opengl");

    // Setup image loading for the right image format
    OpenGLImageHelper::setLoadAsOpenGL(useOpenGL);

    // Create the graphics context
    switch (useOpenGL)
    {
        case 0:
            imageHelper = new SDLImageHelper;
            sdlImageHelper = imageHelper;
            mainGraphics = new Graphics;
            mUseTextureSampler = false;
            break;
        case 1:
        default:
#ifndef ANDROID
            imageHelper = new OpenGLImageHelper;
            sdlImageHelper = new SDLImageHelper;
            mainGraphics = new NormalOpenGLGraphics;
            mUseTextureSampler = true;
            break;
        case 2:
            imageHelper = new OpenGLImageHelper;
            sdlImageHelper = new SDLImageHelper;
            mainGraphics = new SafeOpenGLGraphics;
            mUseTextureSampler = false;
            break;
#endif
        case 3:
            imageHelper = new OpenGLImageHelper;
            sdlImageHelper = new SDLImageHelper;
            mainGraphics = new MobileOpenGLGraphics;
            mUseTextureSampler = false;
            break;
    };
    mUseAtlases = imageHelper->useOpenGL()
        && config.getBoolValue("useAtlases");
#else
void GraphicsManager::initGraphics(const bool noOpenGL A_UNUSED)
{
    // Create the graphics context
    imageHelper = new SDLImageHelper;
    sdlImageHelper = imageHelper;
    mainGraphics = new Graphics;
#endif
}

void GraphicsManager::setVideoMode()
{
    const int bpp = 0;
    const bool fullscreen = config.getBoolValue("screen");
    const bool hwaccel = config.getBoolValue("hwaccel");
    const bool enableResize = config.getBoolValue("enableresize");
    const bool noFrame = config.getBoolValue("noframe");

#ifdef ANDROID
    int width = config.getValue("screenwidth", 0);
    int height = config.getValue("screenheight", 0);
    StringVect videoModes;
    getAllVideoModes(videoModes);
    if (!videoModes.empty())
    {
        bool found(false);
        std::string str = strprintf("%dx%d", width, height);
        if (width != 0 && height != 0)
        {
            FOR_EACH (StringVectCIter, it, videoModes)
            {
                if (str == *it)
                {
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            if (width != 0 && height != 0)
            {
                logger->log("Current resolution %s is incorrect.",
                    str.c_str());
            }
//            str = videoModes[0];
            std::vector<int> res;
            splitToIntVector(res, videoModes[0], 'x');
            if (res.size() == 2)
            {
                width = res[0];
                height = res[1];
                logger->log("Autoselect mode %dx%d", width, height);
            }
        }
    }
#else
    int width = config.getIntValue("screenwidth");
    int height = config.getIntValue("screenheight");
#endif

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

bool GraphicsManager::getAllVideoModes(StringVect &modeList)
{
    /* Get available fullscreen/hardware modes */
    SDL_Rect **const modes = SDL_ListModes(nullptr,
        SDL_FULLSCREEN | SDL_HWSURFACE);

    /* Check which modes are available */
    if (modes == static_cast<SDL_Rect **>(nullptr))
    {
        logger->log1("No modes available");
        return false;
    }
    else if (modes == reinterpret_cast<SDL_Rect **>(-1))
    {
        logger->log1("All resolutions available");
        return true;
    }
    else
    {
        for (int i = 0; modes[i]; ++ i)
        {
            const std::string modeString =
                toString(static_cast<int>(modes[i]->w)).append("x")
                .append(toString(static_cast<int>(modes[i]->h)));
            logger->log("support mode: " + modeString);
            modeList.push_back(modeString);
        }
        return true;
    }
}

#ifdef USE_OPENGL
void GraphicsManager::updateExtensions()
{
    mExtensions.clear();
    logger->log1("opengl extensions: ");
    if (checkGLVersion(3, 0))
    {   // get extensions in new way
        assignFunction(glGetStringi, "glGetStringi");
        std::string extList;
        int num = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num);
        for (int f = 0; f < num; f ++)
        {
            std::string str = reinterpret_cast<const char*>(
                mglGetStringi(GL_EXTENSIONS, f));
            mExtensions.insert(str);
            extList.append(str).append(" ");
        }
        logger->log1(extList.c_str());
    }
    else
    {   // get extensions in old way
        char const *extensions = reinterpret_cast<char const *>(
            glGetString(GL_EXTENSIONS));
        logger->log1(extensions);

        splitToStringSet(mExtensions, extensions, ' ');
    }
}

void GraphicsManager::updatePlanformExtensions()
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info))
    {
#ifdef WIN32
        if (!mwglGetExtensionsString)
            return;

        HDC hdc = GetDC(info.window);
        if (hdc)
        {
            const char *const extensions = mwglGetExtensionsString(hdc);
            if (extensions)
            {
                logger->log1("wGL extensions:");
                logger->log1(extensions);
                splitToStringSet(mPlatformExtensions, extensions, ' ');
            }
        }
#elif defined USE_X11
        Display *const display = info.info.x11.display;
        if (display)
        {
            Screen *const screen = XDefaultScreenOfDisplay(display);
            if (!screen)
                return;

            const int screenNum = XScreenNumberOfScreen(screen);
            const char *const extensions = glXQueryExtensionsString(
                display, screenNum);
            if (extensions)
            {
                logger->log1("glx extensions:");
                logger->log1(extensions);
                splitToStringSet(mPlatformExtensions, extensions, ' ');
            }
            glXQueryVersion(display, &mPlatformMajor, &mPlatformMinor);
            if (checkPlatformVersion(1, 1))
            {
                const char *const vendor1 = glXQueryServerString(
                    display, screenNum, GLX_VENDOR);
                if (vendor1)
                    logger->log("glx server vendor: %s", vendor1);
                const char *const version1 = glXQueryServerString(
                    display, screenNum, GLX_VERSION);
                if (version1)
                    logger->log("glx server version: %s", version1);
                const char *const extensions1 = glXQueryServerString(
                    display, screenNum, GLX_EXTENSIONS);
                if (extensions1)
                {
                    logger->log1("glx server extensions:");
                    logger->log1(extensions1);
                }

                const char *const vendor2 = glXGetClientString(
                    display, GLX_VENDOR);
                if (vendor2)
                    logger->log("glx client vendor: %s", vendor2);
                const char *const version2 = glXGetClientString(
                    display, GLX_VERSION);
                if (version2)
                    logger->log("glx client version: %s", version2);
                const char *const extensions2 = glXGetClientString(
                    display, GLX_EXTENSIONS);
                if (extensions2)
                {
                    logger->log1("glx client extensions:");
                    logger->log1(extensions2);
                }
            }
            logger->log("width=%d", DisplayWidth(display, screenNum));
        }
#endif
    }
}

bool GraphicsManager::supportExtension(const std::string &ext) const
{
    return mExtensions.find(ext) != mExtensions.end();
}

void GraphicsManager::updateTextureFormat()
{
    const int compressionFormat = config.getIntValue("compresstextures");
    if (compressionFormat)
    {
        // using extensions if can
        if (supportExtension("GL_ARB_texture_compression"))
        {
            if (supportExtension("GL_EXT_texture_compression_s3tc")
                || supportExtension("3DFX_texture_compression_FXT1"))
            {
                GLint num = 0;
                glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
                logger->log("support %d compressed formats", num);
                GLint *const formats = new GLint[num > 10 ? num : 10];
                glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
                for (int f = 0; f < num; f ++)
                    logger->log(" 0x%x", formats[f]);

                for (int f = 0; f < num; f ++)
                {
                    if (formats[f] == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
                        && compressionFormat == 1)
                    {
                        delete []formats;
                        OpenGLImageHelper::setInternalTextureType(
                            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
                        logger->log1("using s3tc texture compression");
                        return;
                    }
                    else if (formats[f] == GL_COMPRESSED_RGBA_FXT1_3DFX
                             && compressionFormat == 2)
                    {
                        delete []formats;
                        OpenGLImageHelper::setInternalTextureType(
                            GL_COMPRESSED_RGBA_FXT1_3DFX);
                        logger->log1("using fxt1 texture compression");
                        return;
                    }
                }
                delete []formats;
                if (compressionFormat == 3)
                {
                    OpenGLImageHelper::setInternalTextureType(
                        GL_COMPRESSED_RGBA_ARB);
                    logger->log1("using ARB texture compression");
                    return;
                }
            }
            else
            {
                if (compressionFormat == 3)
                {
                    OpenGLImageHelper::setInternalTextureType(
                        GL_COMPRESSED_RGBA_ARB);
                    logger->log1("using ARB texture compression");
                    return;
                }
            }
        }
        else
        {
            logger->log1("no correct compression format found");
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
}
#endif

#ifdef USE_OPENGL

void GraphicsManager::logString(const char *const format, const int num)
{
    const char *str = reinterpret_cast<const char*>(glGetString(num));
    if (!str)
        logger->log(format, "?");
    else
        logger->log(format, str);
}

std::string GraphicsManager::getGLString(const int num)
{
    const char *str = reinterpret_cast<const char*>(glGetString(num));
    return str ? str : "";
}

void GraphicsManager::setGLVersion()
{
    mGlVersionString = getGLString(GL_VERSION);
    sscanf(mGlVersionString.c_str(), "%5d.%5d", &mMajor, &mMinor);
    mGlVendor = getGLString(GL_VENDOR);
    mGlRenderer = getGLString(GL_RENDERER);
}

void GraphicsManager::logVersion() const
{
    logger->log("gl vendor: " + mGlVendor);
    logger->log("gl renderer: " + mGlRenderer);
    logger->log("gl version: " + mGlVersionString);
}

bool GraphicsManager::checkGLVersion(const int major, const int minor) const
{
    return mMajor > major || (mMajor == major && mMinor >= minor);
}

bool GraphicsManager::checkPlatformVersion(const int major,
                                           const int minor) const
{
    return mPlatformMajor > major || (mPlatformMajor == major
        && mPlatformMinor >= minor);
}

void GraphicsManager::createFBO(const int width, const int height,
                                FBOInfo *const fbo)
{
    if (!fbo)
        return;

    // create a texture object
    glGenTextures(1, &fbo->textureId);
    glBindTexture(OpenGLImageHelper::mTextureType, fbo->textureId);
    glTexParameterf(OpenGLImageHelper::mTextureType,
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(OpenGLImageHelper::mTextureType,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(OpenGLImageHelper::mTextureType,
        GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(OpenGLImageHelper::mTextureType,
        GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(OpenGLImageHelper::mTextureType, 0, GL_RGBA8, width, height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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
}

void GraphicsManager::deleteFBO(FBOInfo *const fbo)
{
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
}

void GraphicsManager::initOpenGLFunctions()
{
    // Texture sampler
    if (checkGLVersion(1, 0) && supportExtension("GL_ARB_sampler_objects"))
    {
        logger->log1("found GL_ARB_sampler_objects");
        assignFunction(glGenSamplers, "glGenSamplers");
        assignFunction(glDeleteSamplers, "glDeleteSamplers");
        assignFunction(glBindSampler, "glBindSampler");
        assignFunction(glSamplerParameteri, "glSamplerParameteri");
        if (mglGenSamplers && config.getBoolValue("useTextureSampler"))
            mUseTextureSampler &= true;
        else
            mUseTextureSampler = false;
    }
    else
    {
        logger->log1("texture sampler not found");
        mUseTextureSampler = false;
    }

    if (!checkGLVersion(1, 1))
        return;

    if (supportExtension("GL_ARB_framebuffer_object"))
    {   // frame buffer supported
        logger->log1("found GL_ARB_framebuffer_object");
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
        logger->log1("found GL_EXT_framebuffer_object");
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
        logger->log1("frame buffer not found");
        config.setValue("usefbo", false);
    }

    // debug extensions
    if (supportExtension("GL_KHR_debug"))
    {
        logger->log1("found GL_KHR_debug");
        assignFunction(glDebugMessageControl, "glDebugMessageControl");
        assignFunction(glDebugMessageCallback, "glDebugMessageCallback");
        mSupportDebug = 2;
    }
    else if (supportExtension("GL_ARB_debug_output"))
    {
        logger->log1("found GL_ARB_debug_output");
        assignFunction(glDebugMessageControl, "glDebugMessageControlARB");
        assignFunction(glDebugMessageCallback, "glDebugMessageCallbackARB");
        mSupportDebug = 1;
    }
    else
    {
        logger->log1("debug extensions not found");
        mSupportDebug = 0;
    }

#ifdef WIN32
    assignFunction(wglGetExtensionsString, "wglGetExtensionsStringARB");
#endif
}

void GraphicsManager::updateLimits()
{
    GLint value = 0;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &value);
    logger->log("GL_MAX_ELEMENTS_VERTICES: %d", value);

    mMaxVertices = value;

    value = 0;
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &value);
    logger->log("GL_MAX_ELEMENTS_INDICES: %d", value);
    if (value < mMaxVertices)
        mMaxVertices = value;

    value = 0;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &value);
    logger->log("Max FBO size: %d", value);
    mMaxFboSize = value;
}

void GraphicsManager::initOpenGL()
{
    setGLVersion();
    updateExtensions();
    initOpenGLFunctions();
    updatePlanformExtensions();
    updateDebugLog();
    createTextureSampler();
    updateLimits();
}

void GraphicsManager::createTextureSampler()
{
    GLenum err = getLastError();
    if (err)
        logger->log(errorToString(err));
    if (mUseTextureSampler)
    {
        logger->log1("using texture sampler");
        mglGenSamplers(1, &mTextureSampler);
        if (getLastError() != GL_NO_ERROR)
        {
            mUseTextureSampler = false;
            logger->log1("texture sampler error");
            OpenGLImageHelper::setUseTextureSampler(mUseTextureSampler);
            return;
        }
        OpenGLImageHelper::initTextureSampler(mTextureSampler);
        mglBindSampler(0, mTextureSampler);
        if (getLastError() != GL_NO_ERROR)
        {
            mUseTextureSampler = false;
            logger->log1("texture sampler error");
        }
    }
    OpenGLImageHelper::setUseTextureSampler(mUseTextureSampler);
}

GLenum GraphicsManager::getLastError()
{
    GLenum tmp = glGetError();
    GLenum error = GL_NO_ERROR;
    while (tmp != GL_NO_ERROR)
    {
        error = tmp;
        tmp = glGetError();
    }
    return error;
}

std::string GraphicsManager::errorToString(const GLenum error)
{
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
        return errmsg;
    }
    return "";
}

void GraphicsManager::detectVideoSettings()
{
    config.setValue("videodetected", true);
    TestMain *const test = startDetection();

    if (test)
    {
        const Configuration &conf = test->getConfig();
        int val = conf.getValueInt("opengl", -1);
        if (val >= 0 && val <= 2)
        {
            config.setValue("opengl", val);
            val = conf.getValue("useTextureSampler", -1);
            if (val != -1)
                config.setValue("useTextureSampler", val);
            val = conf.getValue("compresstextures", -1);
            if (val != -1)
                config.setValue("compresstextures", val);
        }
        delete test;
    }
}

static CALLBACK void debugCallback(GLenum source, GLenum type, GLuint id,
                          GLenum severity, GLsizei length,
                          const GLchar *text, GLvoid *userParam A_UNUSED)
{
    std::string message("OPENGL:");
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            message.append(" API");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            message.append(" WM");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            message.append(" SHADERS");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            message.append(" THIRD_PARTY");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            message.append(" APP");
            break;
        case GL_DEBUG_SOURCE_OTHER:
            message.append(" OTHER");
            break;
        default:
            message.append(" ?").append(toString(source));
            break;
    }
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            message.append(" ERROR");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            message.append(" DEPRECATED");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            message.append(" UNDEFINED");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            message.append(" PORTABILITY");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            message.append(" PERFOMANCE");
            break;
        case GL_DEBUG_TYPE_OTHER:
            message.append(" OTHER");
            break;
        case GL_DEBUG_TYPE_MARKER:
            message.append(" MARKER");
            break;
        default:
            message.append(" ?").append(toString(type));
            break;
    }
    message.append(" ").append(toString(id));
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            message.append(" N");
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            message.append(" H");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            message.append(" M");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            message.append(" L");
            break;
        default:
            message.append(" ?").append(toString(type));
            break;
    }
    char *const buf = new char[length + 1];
    memcpy(buf, text, length);
    buf[length] = 0;
    message.append(" ").append(buf);
    delete [] buf;
    logger->log(message);
}

void GraphicsManager::updateDebugLog() const
{
    if (mSupportDebug && config.getIntValue("debugOpenGL"))
    {
        logger->log1("Enable OpenGL debug log");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        mglDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
            0, nullptr, GL_TRUE);
        mglDebugMessageCallback(&debugCallback, this);
    }
}
#endif

void GraphicsManager::detectPixelSize()
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info))
    {
#ifdef WIN32
        HDC hdc = GetDC(info.window);
        if (hdc)
        {
//            SetProcessDPIAware();
            mMaxWidth = GetDeviceCaps(hdc, HORZRES);
            mMaxHeight = GetDeviceCaps(hdc, VERTRES);
            mWidthMM = GetDeviceCaps(hdc, HORZSIZE);
            mHeightMM = GetDeviceCaps(hdc, VERTSIZE);
        }
#elif defined USE_X11
        Display *const display = info.info.x11.display;
        if (display)
        {
            Screen *const screen = XDefaultScreenOfDisplay(display);
            if (!screen)
                return;

            const int screenNum = XScreenNumberOfScreen(screen);
            mMaxWidth = DisplayWidth(display, screenNum);
            mMaxHeight = DisplayHeight(display, screenNum);
            mWidthMM = DisplayWidthMM(display, screenNum);
            mHeightMM = DisplayHeightMM(display, screenNum);
        }
#endif
    }
#if defined ANDROID
    SDL_ANDROID_GetMetrics(&mMaxWidth, &mMaxHeight,
        &mWidthMM, &mHeightMM, &mDensity);
#endif
    logger->log("screen size in pixels: %dx%d", mMaxWidth, mMaxHeight);
    logger->log("screen size in millimeters: %dx%d", mWidthMM, mHeightMM);
    logger->log("actual screen density: " + getDensityString());
    const int density = config.getIntValue("screenDensity");
    if (density > 0 && density <= densitySize)
    {
        mDensity = density - 1;
        logger->log("selected screen density: " + getDensityString());
    }
}

std::string GraphicsManager::getDensityString() const
{
    if (mDensity >= 0 && mDensity < densitySize)
        return densityNames[mDensity];
    return "";
}
