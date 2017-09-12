/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef USE_SDL2
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_android.h>
PRAGMA48(GCC diagnostic pop)
#endif  // USE_SDL2
#elif defined(__native_client__)
#include <GL/Regal.h>
#include "render/opengl/naclglfunctions.h"
#else  // ANDROID
#ifdef USE_X11
#include <GL/glx.h>
#endif  // USE_X11
#endif  // ANDROID
#endif  // WIN32
#endif  // USE_OPENGL

#include "settings.h"

#ifdef USE_OPENGL
#include "render/mobileopengl2graphics.h"
#include "render/mobileopenglgraphics.h"
#include "render/modernopenglgraphics.h"
#include "render/normalopenglgraphics.h"
#include "render/safeopenglgraphics.h"

#include "render/opengl/mgl.h"
#include "render/opengl/mglcheck.h"
#include "render/opengl/mglemu.h"
#endif  // USE_OPENGL

#include "render/sdlgraphics.h"

#ifdef USE_OPENGL
#include "resources/openglimagehelper.h"
#include "resources/openglscreenshothelper.h"
#ifndef ANDROID
#include "resources/mobileopenglscreenshothelper.h"
#include "resources/safeopenglimagehelper.h"
#endif  // ANDROID
#include "render/opengl/mglfunctions.h"
#endif  // USE_OPENGL

#include "resources/sdlimagehelper.h"
#include "resources/sdlscreenshothelper.h"

#ifdef USE_SDL2
#include "render/sdl2softwaregraphics.h"

#include "resources/sdl2softwareimagehelper.h"
#include "resources/sdl2softwarescreenshothelper.h"
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "utils/delete2.h"
#include "utils/sdlhelper.h"

#ifdef USE_OPENGL
#include "test/testmain.h"
#else  // USE_OPENGL
#include "configuration.h"

#include "render/renderers.h"
#endif  // USE_OPENGL

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#ifdef USE_OPENGL
#ifndef GL_MAX_RENDERBUFFER_SIZE
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#endif  // GL_MAX_RENDERBUFFER_SIZE
#define useCompression(name) \
    OpenGLImageHelper::setInternalTextureType(name); \
    logger->log("using " #name " texture compression");
#endif  // USE_OPENGL

GraphicsManager graphicsManager;

RenderType openGLMode = RENDER_SOFTWARE;

ScreenshotHelper *screenshortHelper = nullptr;

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

#ifdef USE_OPENGL
GLenum GraphicsManager::mLastError(GL_NO_ERROR);
#endif  // USE_OPENGL

GraphicsManager::GraphicsManager() :
    mExtensions(),
    mPlatformExtensions(),
    mGlVersionString(),
    mGlVendor(),
    mGlRenderer(),
    mGlShaderVersionString(),
    mMinor(0),
    mMajor(0),
    mSLMinor(0),
    mSLMajor(0),
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
    mSupportModernOpengl(false),
    mGles(false),
#endif  // USE_OPENGL
    mUseAtlases(false)
{
}

GraphicsManager::~GraphicsManager()
{
#ifdef USE_OPENGL
    if (isGLNotNull(mglGenSamplers) && mTextureSampler)
        mglDeleteSamplers(1, &mTextureSampler);
#endif  // USE_OPENGL
}

#ifdef USE_OPENGL
TestMain *GraphicsManager::startDetection()
{
    TestMain *const test = new TestMain;
    test->exec(false);
    return test;
}

int GraphicsManager::detectGraphics()
{
    logger->log1("start detecting best mode...");
    logger->log1("enable opengl mode");
    int textureSampler = 0;
    int compressTextures = 0;
#if !defined(ANDROID) && !defined(__native_client__)
    mainGraphics = new NormalOpenGLGraphics;
#endif  // !defined(ANDROID) && !defined(__native_client__)

    SDL_Window *const window = createWindow(100, 100, 0,
        SDL_ANYFORMAT | SDL_OPENGL);
    mainGraphics->setWindow(window, 100, 100);
    mainGraphics->createGLContext(false);

    initOpenGL();
    logVersion();

    RenderType mode = RENDER_NORMAL_OPENGL;

    // detecting features by known renderers or vendors
    if (findI(mGlRenderer, "gdi generic") != std::string::npos)
    {
        // windows gdi OpenGL emulation
        logger->log("detected gdi drawing");
        logger->log("disable OpenGL");
        mode = RENDER_SOFTWARE;
    }
    else if (findI(mGlRenderer, "Software Rasterizer") != std::string::npos)
    {
        // software OpenGL emulation
        logger->log("detected software drawing");
        logger->log("disable OpenGL");
        mode = RENDER_SOFTWARE;
    }
    else if (findI(mGlRenderer, "Indirect") != std::string::npos)
    {
        // indirect OpenGL drawing
        logger->log("detected indirect drawing");
        logger->log("disable OpenGL");
        mode = RENDER_SOFTWARE;
    }
    else if (findI(mGlVendor, "VMWARE") != std::string::npos)
    {
        // vmware emulation
        logger->log("detected VMWARE driver");
        logger->log("disable OpenGL");
        mode = RENDER_SOFTWARE;
    }
    else if (findI(mGlVendor, "NVIDIA") != std::string::npos)
    {
        // hope it can work well
        logger->log("detected NVIDIA driver");
        config.setValue("useTextureSampler", true);
        textureSampler = 1;
        mode = RENDER_NORMAL_OPENGL;
    }

    // detecting feature based on OpenGL version
    if (!checkGLVersion(1, 1))
    {
        // very old OpenGL version
        logger->log("OpenGL version too old");
        mode = RENDER_SOFTWARE;
    }

    if (mode != RENDER_SOFTWARE && findI(mGlVersionString, "Mesa")
        != std::string::npos)
    {
        // Mesa detected. In latest Mesa look like compression broken.
        config.setValue("compresstextures", false);
        compressTextures = 0;
    }

    config.setValue("opengl", CAST_S32(mode));
    config.setValue("videoconfigured", true);
    config.write();

    logger->log("detection complete");
    return CAST_U32(mode)
        | (1024 * textureSampler) | (2048 * compressTextures);
}

#ifdef USE_SDL2
#define RENDER_SOFTWARE_INIT \
    imageHelper = new SDL2SoftwareImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new SDL2SoftwareGraphics; \
    screenshortHelper = new Sdl2SoftwareScreenshotHelper;
#define RENDER_SDL2_DEFAULT_INIT \
    imageHelper = new SDLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new SDLGraphics; \
    screenshortHelper = new SdlScreenshotHelper; \
    mainGraphics->setRendererFlags(SDL_RENDERER_ACCELERATED); \
    mUseTextureSampler = false;
#else  // USE_SDL2
#define RENDER_SOFTWARE_INIT \
    imageHelper = new SDLImageHelper; \
    surfaceImageHelper = imageHelper; \
    mainGraphics = new SDLGraphics; \
    screenshortHelper = new SdlScreenshotHelper;
#define RENDER_SDL2_DEFAULT_INIT
#endif  // USE_SDL2

#if defined(ANDROID) || defined(__native_client__)
#define RENDER_NORMAL_OPENGL_INIT
#define RENDER_MODERN_OPENGL_INIT
#else  // defined(ANDROID) || defined(__native_client__)
#define RENDER_NORMAL_OPENGL_INIT \
    imageHelper = new OpenGLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new NormalOpenGLGraphics; \
    screenshortHelper = new OpenGLScreenshotHelper; \
    mUseTextureSampler = true;
#define RENDER_MODERN_OPENGL_INIT \
    imageHelper = new OpenGLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new ModernOpenGLGraphics; \
    screenshortHelper = new OpenGLScreenshotHelper; \
    mUseTextureSampler = true;
#endif  // defined(ANDROID) || defined(__native_client__)

#if defined(ANDROID)
#define RENDER_SAFE_OPENGL_INIT
#define RENDER_GLES2_OPENGL_INIT
#else  // defined(ANDROID)
#define RENDER_SAFE_OPENGL_INIT \
    imageHelper = new SafeOpenGLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new SafeOpenGLGraphics; \
    screenshortHelper = new OpenGLScreenshotHelper; \
    mUseTextureSampler = false;
#define RENDER_GLES2_OPENGL_INIT \
    imageHelper = new OpenGLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new MobileOpenGL2Graphics; \
    screenshortHelper = new MobileOpenGLScreenshotHelper; \
    mUseTextureSampler = false;
#endif  // defined(ANDROID)

#if defined(__native_client__)
#define RENDER_GLES_OPENGL_INIT
#else  // defined(__native_client__)
#define RENDER_GLES_OPENGL_INIT \
    imageHelper = new OpenGLImageHelper; \
    surfaceImageHelper = new SurfaceImageHelper; \
    mainGraphics = new MobileOpenGLGraphics; \
    screenshortHelper = new OpenGLScreenshotHelper; \
    mUseTextureSampler = false;
#endif  // defined(__native_client__)

void GraphicsManager::createRenderers()
{
    RenderType useOpenGL = RENDER_SOFTWARE;
    if (!settings.options.noOpenGL)
    {
        if (settings.options.renderer < 0)
        {
            useOpenGL = intToRenderType(config.getIntValue("opengl"));
            settings.options.renderer = CAST_S32(useOpenGL);
        }
        else
        {
            useOpenGL = intToRenderType(settings.options.renderer);
        }
    }

    // Setup image loading for the right image format
    ImageHelper::setOpenGlMode(useOpenGL);

    // Create the graphics context
    switch (useOpenGL)
    {
        case RENDER_SOFTWARE:
            RENDER_SOFTWARE_INIT
            mUseTextureSampler = false;
            break;
        case RENDER_LAST:
        case RENDER_NULL:
        default:
            break;
        case RENDER_NORMAL_OPENGL:
            RENDER_NORMAL_OPENGL_INIT
            break;
        case RENDER_SAFE_OPENGL:
            RENDER_SAFE_OPENGL_INIT
            break;
        case RENDER_MODERN_OPENGL:
            RENDER_MODERN_OPENGL_INIT
            break;
        case RENDER_GLES2_OPENGL:
            RENDER_GLES2_OPENGL_INIT
            break;
        case RENDER_GLES_OPENGL:
            RENDER_GLES_OPENGL_INIT
            break;
        case RENDER_SDL2_DEFAULT:
            RENDER_SDL2_DEFAULT_INIT
            break;
    };
    mUseAtlases = (useOpenGL == RENDER_NORMAL_OPENGL ||
        useOpenGL == RENDER_SAFE_OPENGL ||
        useOpenGL == RENDER_MODERN_OPENGL ||
        useOpenGL == RENDER_GLES_OPENGL ||
        useOpenGL == RENDER_GLES2_OPENGL) &&
        config.getBoolValue("useAtlases");

#else  // USE_OPENGL

void GraphicsManager::createRenderers()
{
    RenderType useOpenGL = RENDER_SOFTWARE;
    if (!settings.options.noOpenGL)
        useOpenGL = intToRenderType(config.getIntValue("opengl"));

    // Setup image loading for the right image format
    ImageHelper::setOpenGlMode(useOpenGL);

    // Create the graphics context
    switch (useOpenGL)
    {
        case RENDER_SOFTWARE:
        case RENDER_SAFE_OPENGL:
        case RENDER_GLES_OPENGL:
        case RENDER_GLES2_OPENGL:
        case RENDER_MODERN_OPENGL:
        case RENDER_NORMAL_OPENGL:
        case RENDER_NULL:
        case RENDER_LAST:
        default:
#ifndef USE_SDL2
        case RENDER_SDL2_DEFAULT:
            imageHelper = new SDLImageHelper;
            surfaceImageHelper = imageHelper;
            mainGraphics = new SDLGraphics;
            screenshortHelper = new SdlScreenshotHelper;
#else  // USE_SDL2

            imageHelper = new SDL2SoftwareImageHelper;
            surfaceImageHelper = new SurfaceImageHelper;
            mainGraphics = new SDL2SoftwareGraphics;
            screenshortHelper = new Sdl2SoftwareScreenshotHelper;

#endif  // USE_SDL2

            break;
#ifdef USE_SDL2
        case RENDER_SDL2_DEFAULT:
            imageHelper = new SDLImageHelper;
            surfaceImageHelper = new SurfaceImageHelper;
            mainGraphics = new SDLGraphics;
            mainGraphics->setRendererFlags(SDL_RENDERER_ACCELERATED);
            screenshortHelper = new SdlScreenshotHelper;
            break;
#endif  // USE_SDL2
    };
#endif  // USE_OPENGL
}

void GraphicsManager::deleteRenderers()
{
    delete2(mainGraphics);
    if (imageHelper != surfaceImageHelper)
        delete surfaceImageHelper;
    surfaceImageHelper = nullptr;
    delete2(imageHelper);
}

void GraphicsManager::setVideoMode()
{
    const int bpp = 0;
    const bool fullscreen = config.getBoolValue("screen");
    const bool hwaccel = config.getBoolValue("hwaccel");
    const bool enableResize = config.getBoolValue("enableresize");
    const bool noFrame = config.getBoolValue("noframe");
    const bool allowHighDPI = config.getBoolValue("allowHighDPI");

#ifdef ANDROID
//    int width = config.getValue("screenwidth", 0);
//    int height = config.getValue("screenheight", 0);
    StringVect videoModes;
    SDL::getAllVideoModes(videoModes);
    if (videoModes.empty())
        logger->error("no video modes detected");
    STD_VECTOR<int> res;
    splitToIntVector(res, videoModes[0], 'x');
    if (res.size() != 2)
        logger->error("no video modes detected");

    int width = res[0];
    int height = res[1];
#elif defined __native_client__
#ifdef USE_SDL2
    // not implemented
#else  // USE_SDL2

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    int width = info->current_w;
    int height = info->current_h;
#endif  // USE_SDL2
#else  // defined __native_client__

    int width = config.getIntValue("screenwidth");
    int height = config.getIntValue("screenheight");
#endif  // defined __native_client__

    const int scale = config.getIntValue("scale");

    // Try to set the desired video mode
    if (!mainGraphics->setVideoMode(width, height, scale, bpp,
        fullscreen, hwaccel, enableResize, noFrame, allowHighDPI))
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
            if (!mainGraphics->setVideoMode(oldWidth, oldHeight,
                scale,
                bpp,
                oldFullscreen != 0,
                hwaccel,
                enableResize,
                noFrame,
                allowHighDPI))
            {
                logger->safeError(strprintf("Couldn't restore %dx%dx%d "
                    "video mode: %s", oldWidth, oldHeight, bpp,
                    SDL_GetError()));
            }
        }
    }
}

void GraphicsManager::initGraphics()
{
    openGLMode = intToRenderType(config.getIntValue("opengl"));
#ifdef USE_OPENGL
    OpenGLImageHelper::setBlur(config.getBoolValue("blur"));
#ifndef ANDROID
    SafeOpenGLImageHelper::setBlur(config.getBoolValue("blur"));
#endif  // ANDROID
    SurfaceImageHelper::SDLSetEnableAlphaCache(
        config.getBoolValue("alphaCache") &&
        openGLMode == RENDER_SOFTWARE);
    ImageHelper::setEnableAlpha((config.getFloatValue("guialpha") != 1.0F ||
        openGLMode != RENDER_SOFTWARE) &&
        config.getBoolValue("enableGuiOpacity"));
#else  // USE_OPENGL
    SurfaceImageHelper::SDLSetEnableAlphaCache(
        config.getBoolValue("alphaCache"));
    ImageHelper::setEnableAlpha(config.getFloatValue("guialpha") != 1.0F &&
        config.getBoolValue("enableGuiOpacity"));
#endif  // USE_OPENGL
    createRenderers();
    setVideoMode();
    detectPixelSize();
#ifdef USE_OPENGL
    if (config.getBoolValue("checkOpenGLVersion") == true)
    {
        const RenderType oldOpenGLMode = openGLMode;
        if (openGLMode == RENDER_MODERN_OPENGL)
        {
            if (!mSupportModernOpengl || !checkGLVersion(3, 0))
            {
                logger->log("Fallback to normal OpenGL mode");
                openGLMode = RENDER_NORMAL_OPENGL;
            }
        }
        if (openGLMode == RENDER_NORMAL_OPENGL)
        {
            if (!checkGLVersion(2, 0))
            {
                logger->log("Fallback to safe OpenGL mode");
                openGLMode = RENDER_SAFE_OPENGL;
            }
        }
        if (openGLMode == RENDER_GLES_OPENGL)
        {
            if (!checkGLVersion(2, 0) && !checkGLesVersion(1, 0))
            {
                logger->log("Fallback to safe OpenGL mode");
                openGLMode = RENDER_SAFE_OPENGL;
            }
        }
        if (openGLMode == RENDER_GLES2_OPENGL)
        {
            // +++ here need check also not implemented gles flag
            if (!checkGLVersion(2, 0))
            {
                logger->log("Fallback to software mode");
                openGLMode = RENDER_SOFTWARE;
            }
        }

        if (openGLMode != oldOpenGLMode)
        {
            deleteRenderers();
            settings.options.renderer = CAST_S32(openGLMode);
            config.setValue("opengl", settings.options.renderer);
            createRenderers();
            setVideoMode();
            detectPixelSize();
        }
    }
#if !defined(ANDROID) && !defined(__APPLE__)
    const std::string str = config.getStringValue("textureSize");
    STD_VECTOR<int> sizes;
    splitToIntVector(sizes, str, ',');
    const size_t pos = CAST_SIZE(openGLMode);
    if (sizes.size() <= pos)
        settings.textureSize = 1024;
    else
        settings.textureSize = sizes[pos];
    logger->log("Detected max texture size: %u", settings.textureSize);
#endif  // !defined(ANDROID) && !defined(__APPLE__)
#endif  // USE_OPENGL
}

#ifdef USE_SDL2
SDL_Window *GraphicsManager::createWindow(const int w, const int h,
                                          const int bpp A_UNUSED,
                                          const int flags)
{
    return SDL_CreateWindow("ManaPlus", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, w, h, flags);
}

SDL_Renderer *GraphicsManager::createRenderer(SDL_Window *const window,
                                              const int flags)
{
    // +++ need use different drivers and different flags
    SDL_Renderer *const renderer = SDL_CreateRenderer(window, -1, flags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return renderer;
}
#else  // USE_SDL2

SDL_Window *GraphicsManager::createWindow(const int w, const int h,
                                          const int bpp, const int flags)
{
    return SDL_SetVideoMode(w, h, bpp, flags);
}
#endif  // USE_SDL2

#ifdef USE_OPENGL
void GraphicsManager::updateExtensions()
{
    mExtensions.clear();
    logger->log1("opengl extensions: ");
    if (checkGLVersion(3, 0))
    {   // get extensions in new way
        assignFunction2(glGetStringi, "glGetStringi");
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
            mglGetString(GL_EXTENSIONS));
        if (extensions)
        {
            logger->log1(extensions);
            splitToStringSet(mExtensions, extensions, ' ');
        }
    }
}

void GraphicsManager::updatePlanformExtensions()
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL::getWindowWMInfo(mainGraphics->getWindow(), &info))
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
#endif  // WIN32
    }
}

bool GraphicsManager::supportExtension(const std::string &ext) const
{
    return mExtensions.find(ext) != mExtensions.end();
}

void GraphicsManager::updateTextureCompressionFormat() const
{
    const int compressionFormat = config.getIntValue("compresstextures");
    // using extensions if can
    if (checkGLVersion(3, 1) ||
        checkGLesVersion(2, 0) ||
        supportExtension("GL_ARB_texture_compression"))
    {
        GLint num = 0;
        mglGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num);
        logger->log("support %d compressed formats", num);
        GLint *const formats = new GLint[num > 10
            ? CAST_SIZE(num) : 10];
        mglGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
        for (int f = 0; f < num; f ++)
            logger->log(" 0x%x", CAST_U32(formats[f]));

        if (compressionFormat)
        {
            for (int f = 0; f < num; f ++)
            {
                switch (formats[f])
                {
                    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                        if (compressionFormat == 1)
                        {
                            delete []formats;
                            useCompression(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
                            return;
                        }
                        break;
                    case GL_COMPRESSED_RGBA_FXT1_3DFX:
                        if (compressionFormat == 2)
                        {
                            delete []formats;
                            useCompression(GL_COMPRESSED_RGBA_FXT1_3DFX);
                            return;
                        }
                        break;
                    case GL_COMPRESSED_RGBA_BPTC_UNORM_ARB:
                        if (compressionFormat == 4)
                        {
                            delete []formats;
                            useCompression(GL_COMPRESSED_RGBA_BPTC_UNORM_ARB);
                            return;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (compressionFormat == 3)
            {
                delete []formats;
                useCompression(GL_COMPRESSED_RGBA_ARB);
                return;
            }

            // workaround for MESA bptc compression detection
            if (compressionFormat == 4
                && supportExtension("GL_ARB_texture_compression_bptc"))
            {
                delete []formats;
                useCompression(GL_COMPRESSED_RGBA_BPTC_UNORM_ARB);
                return;
            }
        }
        delete []formats;
        if (compressionFormat)
            logger->log1("no correct compression format found");
    }
    else
    {
        if (compressionFormat)
            logger->log1("no correct compression format found");
    }
}

void GraphicsManager::updateTextureFormat()
{
    const int renderer = settings.options.renderer;

    // using default formats
    if (renderer == RENDER_MODERN_OPENGL ||
        renderer == RENDER_GLES_OPENGL ||
        renderer == RENDER_GLES2_OPENGL ||
        config.getBoolValue("newtextures"))
    {
        OpenGLImageHelper::setInternalTextureType(GL_RGBA);
#ifndef ANDROID
        SafeOpenGLImageHelper::setInternalTextureType(GL_RGBA);
#endif  // ANDROID

        logger->log1("using RGBA texture format");
    }
    else
    {
        OpenGLImageHelper::setInternalTextureType(4);
#ifndef ANDROID
        SafeOpenGLImageHelper::setInternalTextureType(4);
#endif  // ANDROID

        logger->log1("using 4 texture format");
    }
}

void GraphicsManager::logString(const char *const format, const int num)
{
    const char *str = reinterpret_cast<const char*>(mglGetString(num));
    if (!str)
        logger->log(format, "?");
    else
        logger->log(format, str);
}

std::string GraphicsManager::getGLString(const int num)
{
    const char *str = reinterpret_cast<const char*>(mglGetString(num));
    return str ? str : "";
}

void GraphicsManager::setGLVersion()
{
    mGlVersionString = getGLString(GL_VERSION);
    std::string version = mGlVersionString;
    if (findCutFirst(version, "OpenGL ES "))
        mGles = true;
    sscanf(version.c_str(), "%5d.%5d", &mMajor, &mMinor);
    logger->log("Detected gl version: %d.%d", mMajor, mMinor);
    mGlVendor = getGLString(GL_VENDOR);
    mGlRenderer = getGLString(GL_RENDERER);
    mGlShaderVersionString = getGLString(GL_SHADING_LANGUAGE_VERSION);
    version = mGlShaderVersionString;
    cutFirst(version, "OpenGL ES GLSL ES ");
    cutFirst(version, "OpenGL ES GLSL ");
    cutFirst(version, "OpenGL ES ");
    sscanf(version.c_str(), "%5d.%5d", &mSLMajor, &mSLMinor);
    logger->log("Detected glsl version: %d.%d", mSLMajor, mSLMinor);
#ifdef ANDROID
    if (!mMajor && !mMinor)
    {
        logger->log("Overriding detected OpenGL version on Android to 1.0");
        mGles = true;
        mMajor = 1;
        mMinor = 0;
    }
#endif  // ANDROID
}

void GraphicsManager::logVersion() const
{
    logger->log("gl vendor: " + mGlVendor);
    logger->log("gl renderer: " + mGlRenderer);
    logger->log("gl version: " + mGlVersionString);
    logger->log("glsl version: " + mGlShaderVersionString);
}

bool GraphicsManager::checkGLVersion(const int major, const int minor) const
{
    return mMajor > major || (mMajor == major && mMinor >= minor);
}

bool GraphicsManager::checkSLVersion(const int major, const int minor) const
{
    return mSLMajor > major || (mSLMajor == major && mSLMinor >= minor);
}

bool GraphicsManager::checkGLesVersion(const int major, const int minor) const
{
    return mGles && (mMajor > major || (mMajor == major && mMinor >= minor));
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
#ifdef __native_client__
    emulateFunction(glTextureSubImage2DEXT);
#else  // __native_client__

    const bool is10 = checkGLVersion(1, 0);
    const bool is11 = checkGLVersion(1, 1);
    const bool is12 = checkGLVersion(1, 2);
    const bool is13 = checkGLVersion(1, 3);
    const bool is15 = checkGLVersion(1, 5);
    const bool is20 = checkGLVersion(2, 0);
    const bool is21 = checkGLVersion(2, 1);
    const bool is30 = checkGLVersion(3, 0);
    const bool is33 = checkGLVersion(3, 3);
    const bool is41 = checkGLVersion(4, 1);
    const bool is42 = checkGLVersion(4, 2);
    const bool is43 = checkGLVersion(4, 3);
    const bool is44 = checkGLVersion(4, 4);
    const bool is45 = checkGLVersion(4, 5);
    mSupportModernOpengl = true;

    // Texture sampler
    if (is10 && (is33 || supportExtension("GL_ARB_sampler_objects")))
    {
        logger->log1("found GL_ARB_sampler_objects");
        assignFunction(glGenSamplers);
        assignFunction(glDeleteSamplers);
        assignFunction(glBindSampler);
        assignFunction(glSamplerParameteri);
        if (isGLNotNull(mglGenSamplers)
            && config.getBoolValue("useTextureSampler"))
        {
            mUseTextureSampler &= true;
        }
        else
        {
            mUseTextureSampler = false;
        }
    }
    else
    {
        logger->log1("texture sampler not found");
        mUseTextureSampler = false;
    }

    if (!is11)
    {
        mSupportModernOpengl = false;
        emulateFunction(glTextureSubImage2DEXT);
        return;
    }

/*
    if (findI(mGlVendor, "NVIDIA") != std::string::npos ||
        mGlVersionString.find("Mesa 10.6.") != std::string::npos ||
        mGlVersionString.find("Mesa 11.1.1") != std::string::npos ||
        mGlVersionString.find("Mesa 11.1.2") != std::string::npos ||
        mGlVersionString.find("Mesa 11.1.3") != std::string::npos ||
        mGlVersionString.find("Mesa 11.2") != std::string::npos ||
        (findI(mGlRenderer, "AMD Radeon HD") != std::string::npos &&
        (mGlVersionString.find(
        "Compatibility Profile Context 14.") != std::string::npos ||
        mGlVersionString.find(
        "Compatibility Profile Context 15.") != std::string::npos)))
    {
        logger->log1("Not checked for DSA because on "
            "NVIDIA or AMD or in Mesa it broken");
        emulateFunction(glTextureSubImage2DEXT);
    }
    else
*/
    {   // not for NVIDIA. in NVIDIA atleast in windows drivers DSA is broken
        // Mesa 10.6.3 show support for DSA, but it broken. Works in 10.7 dev
        if (config.getBoolValue("enableDSA") == true)
        {
            if (is45)
            {
                logger->log1("found GL_EXT_direct_state_access");
                assignFunction(glTextureSubImage2D);
            }
            else if (supportExtension("GL_EXT_direct_state_access"))
            {
                logger->log1("found GL_EXT_direct_state_access");
                assignFunctionEmu2(glTextureSubImage2DEXT,
                    "glTextureSubImage2DEXT");
            }
            else if (supportExtension("GL_ARB_direct_state_access"))
            {
                logger->log1("found GL_ARB_direct_state_access");
                logger->log1("GL_EXT_direct_state_access not found");
                assignFunction(glTextureSubImage2D);
            }
            else
            {
                logger->log1("GL_EXT_direct_state_access not found");
                logger->log1("GL_ARB_direct_state_access not found");
                emulateFunction(glTextureSubImage2DEXT);
            }
        }
        else
        {
            logger->log1("Direct state access disabled in settings");
            emulateFunction(glTextureSubImage2DEXT);
        }
    }

    if (is12 && (is42 || supportExtension("GL_ARB_texture_storage")))
    {
        logger->log1("found GL_ARB_texture_storage");
        assignFunction(glTexStorage2D);
    }
    else
    {
        logger->log1("GL_ARB_texture_storage not found");
    }

    if (is13 || supportExtension("GL_ARB_multitexture"))
    {
        logger->log1("found GL_ARB_multitexture or OpenGL 1.3");
        assignFunction(glActiveTexture);
    }
    else
    {
        emulateFunction(glActiveTexture);
        logger->log1("GL_ARB_multitexture not found");
    }

    if (is20 || supportExtension("GL_ARB_explicit_attrib_location"))
    {
        logger->log1("found GL_ARB_explicit_attrib_location or OpenGL 2.0");
        assignFunction(glBindAttribLocation);
    }
    else
    {
        logger->log1("GL_ARB_explicit_attrib_location not found");
    }

    if (is30 || supportExtension("GL_ARB_framebuffer_object"))
    {   // frame buffer supported
        logger->log1("found GL_ARB_framebuffer_object");
        assignFunction(glGenRenderbuffers);
        assignFunction(glBindRenderbuffer);
        assignFunction(glRenderbufferStorage);
        assignFunction(glGenFramebuffers);
        assignFunction(glBindFramebuffer);
        assignFunction(glFramebufferTexture2D);
        assignFunction(glFramebufferRenderbuffer);
        assignFunction(glDeleteFramebuffers);
        assignFunction(glDeleteRenderbuffers);
        assignFunction(glCheckFramebufferStatus);
    }
    else if (supportExtension("GL_EXT_framebuffer_object"))
    {   // old frame buffer extension
        logger->log1("found GL_EXT_framebuffer_object");
        assignFunctionEXT(glGenRenderbuffers);
        assignFunctionEXT(glBindRenderbuffer);
        assignFunctionEXT(glRenderbufferStorage);
        assignFunctionEXT(glGenFramebuffers);
        assignFunctionEXT(glBindFramebuffer);
        assignFunctionEXT(glFramebufferTexture2D);
        assignFunctionEXT(glFramebufferRenderbuffer);
        assignFunctionEXT(glDeleteFramebuffers);
        assignFunctionEXT(glDeleteRenderbuffers);
    }
    else
    {   // no frame buffer support
        logger->log1("GL_ARB_framebuffer_object or "
            "GL_EXT_framebuffer_object not found");
        config.setValue("usefbo", false);
    }

    // debug extensions
    if (is43 || supportExtension("GL_KHR_debug"))
    {
        logger->log1("found GL_KHR_debug");
        assignFunction(glDebugMessageControl);
        assignFunction(glDebugMessageCallback);
        assignFunction(glPushDebugGroup);
        assignFunction(glPopDebugGroup);
        assignFunction(glObjectLabel);
        mSupportDebug = 2;
    }
    else if (supportExtension("GL_ARB_debug_output"))
    {
        logger->log1("found GL_ARB_debug_output");
        assignFunctionARB(glDebugMessageControl);
        assignFunctionARB(glDebugMessageCallback);
        mSupportDebug = 1;
    }
    else
    {
        logger->log1("debug extensions not found");
        mSupportDebug = 0;
    }

    if (supportExtension("GL_GREMEDY_frame_terminator"))
    {
        logger->log1("found GL_GREMEDY_frame_terminator");
        assignFunction2(glFrameTerminator, "glFrameTerminatorGREMEDY");
    }
    else
    {
        logger->log1("GL_GREMEDY_frame_terminator not found");
    }
    if (is44 || supportExtension("GL_EXT_debug_label"))
    {
        logger->log1("found GL_EXT_debug_label");
        assignFunction2(glLabelObject, "glObjectLabel");
        if (isGLNull(mglLabelObject))
            assignFunctionEXT(glLabelObject);
        assignFunctionEXT(glGetObjectLabel);
    }
    else
    {
        logger->log1("GL_EXT_debug_label not found");
    }
    if (supportExtension("GL_GREMEDY_string_marker"))
    {
        logger->log1("found GL_GREMEDY_string_marker");
        assignFunction2(glPushGroupMarker, "glStringMarkerGREMEDY");
    }
    else
    {
        logger->log1("GL_GREMEDY_string_marker not found");
    }
    if (supportExtension("GL_EXT_debug_marker"))
    {
        logger->log1("found GL_EXT_debug_marker");
        assignFunctionEXT(glInsertEventMarker);
        assignFunctionEXT(glPushGroupMarker);
        assignFunctionEXT(glPopGroupMarker);
    }
    else
    {
        logger->log1("GL_EXT_debug_marker not found");
    }
    if (is15 && (is30 || supportExtension("GL_EXT_timer_query")))
    {
        logger->log1("found GL_EXT_timer_query");
        assignFunction(glGenQueries);
        assignFunction(glBeginQuery);
        assignFunction(glEndQuery);
        assignFunction(glDeleteQueries);
        assignFunction(glGetQueryObjectiv);
        assignFunctionEXT(glGetQueryObjectui64v);
    }
    else
    {
        logger->log1("GL_EXT_timer_query not supported");
    }
    if (is20 && (is43 || supportExtension("GL_ARB_invalidate_subdata")))
    {
        logger->log1("found GL_ARB_invalidate_subdata");
        assignFunction(glInvalidateTexImage);
    }
    else
    {
        logger->log1("GL_ARB_invalidate_subdata not supported");
    }
    if (is21 && (is30 || supportExtension("GL_ARB_vertex_array_object")))
    {
        logger->log1("found GL_ARB_vertex_array_object");
        assignFunction(glGenVertexArrays);
        assignFunction(glBindVertexArray);
        assignFunction(glDeleteVertexArrays);
        assignFunction(glVertexAttribPointer);
        assignFunction(glEnableVertexAttribArray);
        assignFunction(glDisableVertexAttribArray);
        assignFunction(glVertexAttribIPointer);
    }
    else
    {
        mSupportModernOpengl = false;
        logger->log1("GL_ARB_vertex_array_object not found");
    }
    if (is20 || supportExtension("GL_ARB_vertex_buffer_object"))
    {
        assignFunction(glGenBuffers);
        assignFunction(glDeleteBuffers);
        assignFunction(glBindBuffer);
        assignFunction(glBufferData);
        assignFunction(glIsBuffer);
    }
    else
    {
        mSupportModernOpengl = false;
        logger->log1("buffers extension not found");
    }
    if (is43 || supportExtension("GL_ARB_copy_image"))
    {
        logger->log1("found GL_ARB_copy_image");
        assignFunction(glCopyImageSubData);
    }
    else
    {
        logger->log1("GL_ARB_copy_image not found");
    }
    if (is44 || supportExtension("GL_ARB_clear_texture"))
    {
        logger->log1("found GL_ARB_clear_texture");
        assignFunction(glClearTexImage);
        assignFunction(glClearTexSubImage);
    }
    else
    {
        logger->log1("GL_ARB_clear_texture not found");
    }
    if (is20 || supportExtension("GL_ARB_shader_objects"))
    {
        logger->log1("found GL_ARB_shader_objects");
        assignFunction(glCreateShader);
        assignFunction(glDeleteShader);
        assignFunction(glGetShaderiv);
        assignFunction(glGetShaderInfoLog);
        assignFunction(glGetShaderSource);
        assignFunction(glShaderSource);
        assignFunction(glCompileShader);
        assignFunction(glLinkProgram);
        assignFunction(glGetProgramInfoLog);
        assignFunction(glDeleteProgram);
        assignFunction(glCreateProgram);
        assignFunction(glAttachShader);
        assignFunction(glDetachShader);
        assignFunction(glGetAttachedShaders);
        assignFunction(glGetUniformLocation);
        assignFunction(glGetActiveUniform);
        assignFunction(glGetProgramiv);
        assignFunction(glUseProgram);
        assignFunction(glValidateProgram);
        assignFunction(glGetAttribLocation);
        assignFunction(glUniform1f);
        assignFunction(glUniform2f);
        assignFunction(glUniform3f);
        assignFunction(glUniform4f);

        if (is30 || supportExtension("GL_EXT_gpu_shader4"))
        {
            logger->log1("found GL_EXT_gpu_shader4");
            assignFunction(glBindFragDataLocation);
        }
        else
        {
            mSupportModernOpengl = false;
            logger->log1("GL_EXT_gpu_shader4 not supported");
        }
        if (is41 || supportExtension("GL_ARB_separate_shader_objects"))
        {
            logger->log1("found GL_ARB_separate_shader_objects");
            assignFunction(glProgramUniform1f);
            assignFunction(glProgramUniform2f);
            assignFunction(glProgramUniform3f);
            assignFunction(glProgramUniform4f);
        }
        else
        {
            logger->log1("GL_ARB_separate_shader_objects not supported");
        }
        if (is43 || supportExtension("GL_ARB_vertex_attrib_binding"))
        {
            logger->log1("found GL_ARB_vertex_attrib_binding");
            assignFunction(glBindVertexBuffer);
            assignFunction(glVertexAttribBinding);
            assignFunction(glVertexAttribFormat);
            assignFunction(glVertexAttribIFormat);
        }
        else
        {
            mSupportModernOpengl = false;
            logger->log1("GL_ARB_vertex_attrib_binding not supported");
        }
        if (is44 || supportExtension("GL_ARB_multi_bind"))
        {
            logger->log1("found GL_ARB_multi_bind");
            assignFunction(glBindVertexBuffers);
        }
        else
        {
            logger->log1("GL_ARB_multi_bind not supported");
        }
    }
    else
    {
        mSupportModernOpengl = false;
        logger->log1("shaders not supported");
    }

#ifdef WIN32
    assignFunctionARB(wglGetExtensionsString);
#endif  // WIN32
#endif  // __native_client__
}

void GraphicsManager::updateLimits()
{
    GLint value = 0;
#ifdef __native_client__
    mMaxVertices = 500;
#else  // __native_client__

    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &value);
    logger->log("GL_MAX_ELEMENTS_VERTICES: %d", value);

    mMaxVertices = value;

    value = 0;
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &value);
    logger->log("GL_MAX_ELEMENTS_INDICES: %d", value);
    if (value < mMaxVertices)
        mMaxVertices = value;
    if (!mMaxVertices)
    {
        logger->log("Got 0 max amount of vertices or indicies. "
            "Overriding to 500");
        mMaxVertices = 500;
    }
#endif  // __native_client__

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
#ifndef ANDROID
    SafeOpenGLImageHelper::setUseTextureSampler(false);
#endif  // ANDROID
}

GLenum GraphicsManager::getLastError()
{
    GLenum tmp = glGetError();
    GLenum error = GL_NO_ERROR;
    while (tmp != GL_NO_ERROR)
    {
        error = tmp;
        mLastError = tmp;
        tmp = glGetError();
    }
    return error;
}

std::string GraphicsManager::errorToString(const GLenum error)
{
    if (error)
    {
        std::string errmsg("Unknown error");
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
        return "OpenGL error: " + errmsg;
    }
    return "";
}

void GraphicsManager::logError()
{
    const GLenum error = GraphicsManager::getLastError();
    if (error != GL_NO_ERROR)
        logger->log(errorToString(error));
}

void GraphicsManager::detectVideoSettings()
{
    config.setValue("videodetected", true);
    TestMain *const test = startDetection();

    if (test)
    {
        const Configuration &conf = test->getConfig();
        int val = conf.getValueInt("opengl", -1);
        if (val >= 0 && val < CAST_S32(RENDER_LAST))
        {
            config.setValue("opengl", val);
            val = conf.getValue("useTextureSampler", -1);
            if (val != -1)
                config.setValue("useTextureSampler", val);
            val = conf.getValue("compresstextures", -1);
            if (val != -1)
                config.setValue("compresstextures", val);
        }
        config.setValue("textureSize", conf.getValue("textureSize",
            "1024,1024,1024,1024,1024,1024"));
        config.setValue("testInfo", conf.getValue("testInfo", ""));
        config.setValue("sound", conf.getValue("sound", 0));
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
            message.append(" PERFORMANCE");
            break;
        case GL_DEBUG_TYPE_OTHER:
            message.append(" OTHER");
            break;
        case GL_DEBUG_TYPE_MARKER:
            message.append(" MARKER");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            message.append(" PUSH GROUP");
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            message.append(" POP GROUP");
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
    char *const buf = new char[CAST_SIZE(length + 1)];
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
#endif  // USE_OPENGL

void GraphicsManager::detectPixelSize()
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL::getWindowWMInfo(mainGraphics->getWindow(), &info))
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
#endif  // WIN32
    }
#if defined ANDROID
#ifdef USE_SDL2
    const int dpi = atoi(getenv("DISPLAY_DPI"));
    if (dpi <= 120)
        mDensity = 0;
    else if (dpi <= 160)
        mDensity = 1;
    else if (dpi <= 213)
        mDensity = 2;
    else if (dpi <= 240)
        mDensity = 3;
    else if (dpi <= 320)
        mDensity = 4;
//    else if (dpi <= 480)
    else
        mDensity = 5;
    mMaxWidth = atoi(getenv("DISPLAY_RESOLUTION_WIDTH"));
    mMaxHeight = atoi(getenv("DISPLAY_RESOLUTION_HEIGHT"));
    mWidthMM = atoi(getenv("DISPLAY_WIDTH_MM"));
    mHeightMM = atoi(getenv("DISPLAY_HEIGHT_MM"));
#else  // USE_SDL2

    SDL_ANDROID_GetMetrics(&mMaxWidth, &mMaxHeight,
        &mWidthMM, &mHeightMM, &mDensity);
#endif  // USE_SDL2
#endif  // defined ANDROID

    logger->log("screen size in pixels: %ux%u", mMaxWidth, mMaxHeight);
    logger->log("screen size in millimeters: %ux%u", mWidthMM, mHeightMM);
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
