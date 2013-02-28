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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "main.h"

#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
// hack to hide warnings
#undef GL_GLEXT_VERSION
#undef GL_GLEXT_PROTOTYPES
#endif

#endif

#include "utils/stringvector.h"

#include <set>
#include <string>

#include "localconsts.h"

class Graphics;
class TestMain;

struct FBOInfo;

class GraphicsManager final
{
    public:
        GraphicsManager();

        A_DELETE_COPY(GraphicsManager)

        virtual ~GraphicsManager();

        void initGraphics(const bool noOpenGL);

        void setVideoMode();

        Graphics *createGraphics() A_WARN_UNUSED;

        bool getAllVideoModes(StringVect &modeList);

#ifdef USE_OPENGL
        TestMain *startDetection() A_WARN_UNUSED;

        int detectGraphics() A_WARN_UNUSED;

        bool supportExtension(const std::string &ext) const A_WARN_UNUSED;

        void updateTextureFormat();

        bool checkGLVersion(const int major, const int minor)
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

        int getMaxVertices() const A_WARN_UNUSED
        { return mMaxVertices; }

        bool getUseAtlases() const A_WARN_UNUSED
        { return mUseAtlases; }

        void logVersion() const;

        void setGLVersion();

        static std::string getGLString(const int num) A_WARN_UNUSED;

        static void logString(const char *const format, const int num);

        void detectVideoSettings();

        void createTextureSampler();

        bool isUseTextureSampler() const A_WARN_UNUSED
        { return mUseTextureSampler; }

        static GLenum getLastError() A_WARN_UNUSED;

        static std::string errorToString(const GLenum error) A_WARN_UNUSED;

        void updateDebugLog() const;
#endif

    private:
        std::set<std::string> mExtensions;

        std::set<std::string> mPlatformExtensions;

        std::string mGlVersionString;

        std::string mGlVendor;

        std::string mGlRenderer;

        int mMinor;

        int mMajor;

        int mPlatformMinor;

        int mPlatformMajor;

        int mMaxVertices;

        int mMaxFboSize;

#ifdef USE_OPENGL
        bool mUseTextureSampler;

        GLuint mTextureSampler;

        int mSupportDebug;
#endif
        bool mUseAtlases;
};

extern GraphicsManager graphicsManager;

#endif
