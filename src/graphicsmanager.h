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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "main.h"

#ifdef USE_OPENGL
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
// hack to hide warnings
#undef GL_GLEXT_VERSION
#undef GL_GLEXT_PROTOTYPES
#endif

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

        void initGraphics(bool noOpenGL);

        void setVideoMode();

        Graphics *createGraphics();

#ifdef USE_OPENGL
        TestMain *startDetection();

        int detectGraphics();

        bool supportExtension(const std::string &ext);

        void updateTextureFormat();

        bool checkGLVersion(int major, int minor) const;

        bool checkPlatformVersion(int major, int minor) const;

        void createFBO(int width, int height, FBOInfo *fbo);

        void deleteFBO(FBOInfo *fbo);

        void initOpenGLFunctions();

        void updateExtensions();

        void updatePlanformExtensions();

        void initOpenGL();

        void updateLimits();

        int getMaxVertices() const
        { return mMaxVertices; }

        bool getUseAtlases() const
        { return mUseAtlases; }

        void logVersion();

        void setGLVersion();

        std::string getGLString(int num) const;

        void logString(const char *format, int num);

        void detectVideoSettings();

        void createTextureSampler();

        bool isUseTextureSampler() const
        { return mUseTextureSampler; }

        unsigned int getLastError();
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
#endif
        bool mUseAtlases;

};

extern GraphicsManager graphicsManager;

#endif
