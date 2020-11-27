/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "localconsts.h"

#ifdef USE_OPENGL
#ifdef ANDROID
#include <GLES/gl.h>
#else  // ANDROID
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif  // USE_SDL2
#ifdef HAVE_GLEXT
#define NO_SDL_GLEXT
#endif  // HAVE_GLEXT
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
// ignore -Wredundant-decls for SDL 1.2
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <SDL_opengl.h>
PRAGMA45(GCC diagnostic pop)
PRAGMA48(GCC diagnostic pop)
#ifdef HAVE_GLEXT
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <GL/glext.h>
PRAGMA45(GCC diagnostic pop)
#endif  // HAVE_GLEXT
// hack to hide warnings
#undef GL_GLEXT_VERSION
#undef GL_GLEXT_PROTOTYPES
#endif  // ANDROID

#endif  // USE_OPENGL

#include "sdlshared.h"

#include "utils/stringvector.h"

#include <set>

class TestMain;

struct FBOInfo;
#ifdef USE_SDL2
struct SDL_Renderer;
#endif  // USE_SDL2
struct SDL_Window;

class GraphicsManager final
{
    public:
        GraphicsManager();

        A_DELETE_COPY(GraphicsManager)

        ~GraphicsManager();

        void createRenderers();

        static void deleteRenderers();

        void initGraphics();

        static void setVideoMode();

        static SDL_Window *createWindow(const int w, const int h,
                                        const int bpp, const int flags);

#ifdef USE_SDL2
        SDL_Renderer *createRenderer(SDL_Window *const window,
                                     const int flags);
#endif  // USE_SDL2

        bool getAllVideoModes(StringVect &modeList);

        void detectPixelSize();

        std::string getDensityString() const;

        int getDensity() const
        { return mDensity; }

#ifdef USE_OPENGL
        TestMain *startDetection() A_WARN_UNUSED;

        int detectGraphics() A_WARN_UNUSED;

        bool supportExtension(const std::string &ext) const A_WARN_UNUSED;

        static void updateTextureFormat();

        void updateTextureCompressionFormat() const;

        bool checkGLVersion(const int major, const int minor)
                            const A_WARN_UNUSED;

        bool checkGLesVersion(const int major, const int minor)
                              const A_WARN_UNUSED;

        bool checkSLVersion(const int major, const int minor)
                            const A_WARN_UNUSED;

        bool checkPlatformVersion(const int major, const int minor)
                                  const A_WARN_UNUSED;

        static void createFBO(const int width, const int height,
                              FBOInfo *const fbo);

        static void deleteFBO(FBOInfo *const fbo);

        void initOpenGLFunctions();

        void updateExtensions();

        void updatePlanformExtensions();

        void initOpenGL();

        void updateLimits();

        int getMaxVertices() const noexcept2 A_WARN_UNUSED
        { return mMaxVertices; }

        bool getUseAtlases() const noexcept2 A_WARN_UNUSED
        { return mUseAtlases; }

        void logVersion() const;

        void setGLVersion();

        static std::string getGLString(const int num) A_WARN_UNUSED;

        static void logString(const char *const format,
                              const int num) A_NONNULL(1);

        void detectVideoSettings();

        void createTextureSampler();

        bool isUseTextureSampler() const noexcept2 A_WARN_UNUSED
        { return mUseTextureSampler; }

        static GLenum getLastError();

        static std::string errorToString(const GLenum error) A_WARN_UNUSED;

        static void logError();

        void updateDebugLog() const;

        std::string getGLVersion() const
        { return mGlVersionString; }

        static GLenum getLastErrorCached()
        { return mLastError; }

        constexpr2 static void resetCachedError() noexcept2
        { mLastError = GL_NO_ERROR; }
#endif  // USE_OPENGL

    private:
        std::set<std::string> mExtensions;

        std::set<std::string> mPlatformExtensions;

        std::string mGlVersionString;

        std::string mGlVendor;

        std::string mGlRenderer;

        std::string mGlShaderVersionString;
#ifdef USE_OPENGL
        static GLenum mLastError;
#endif  // USE_OPENGL

        int mMinor;

        int mMajor;

        int mSLMinor;

        int mSLMajor;

        int mPlatformMinor;

        int mPlatformMajor;

        int mMaxVertices;

        int mMaxFboSize;

        uint32_t mMaxWidth;

        uint32_t mMaxHeight;

        uint32_t mWidthMM;

        uint32_t mHeightMM;

        int32_t mDensity;

#ifdef USE_OPENGL
        bool mUseTextureSampler;

        GLuint mTextureSampler;

        int mSupportDebug;

        bool mSupportModernOpengl;

        bool mGles;
#endif  // USE_OPENGL

        bool mUseAtlases;
};

extern GraphicsManager graphicsManager;

#endif  // GRAPHICSMANAGER_H
