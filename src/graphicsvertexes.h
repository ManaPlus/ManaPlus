/*
 *  The ManaPlus Client
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

#ifndef GRAPHICSVERTEXES_H
#define GRAPHICSVERTEXES_H


#include "main.h"

#include "localconsts.h"

#ifdef USE_OPENGL
//#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>

#include "safeopenglgraphics.h"
#include "normalopenglgraphics.h"
#endif

#include <string>
#include <list>
#include <vector>

struct DoubleRect
{
    SDL_Rect src;
    SDL_Rect dst;
};

class SDLGraphicsVertexes
{
    public:
        SDLGraphicsVertexes();

        ~SDLGraphicsVertexes();

        std::vector<DoubleRect*> mList;
};

#ifdef USE_OPENGL
class SafeOpenGLGraphicsVertexes
{
};

class NormalOpenGLGraphicsVertexes
{
    public:
        NormalOpenGLGraphicsVertexes();

        ~NormalOpenGLGraphicsVertexes();

        GLfloat *switchFloatTexArray();

        GLint *switchIntVertArray();

        GLint *switchIntTexArray();

        std::vector<GLfloat*> *getFloatTexPool()
        { return &mFloatTexPool; }

        std::vector<GLint*> *getIntVertPool()
        { return &mIntVertPool; }

        std::vector<GLint*> *getIntTexPool()
        { return &mIntTexPool; }

        void switchVp(int n);

        GLfloat *continueFloatTexArray();

        GLint *continueIntVertArray();

        GLint *continueIntTexArray();

        int continueVp();

        std::vector<int> *getVp()
        { return &mVp; }

        void init();

        void clear();

        int ptr;

        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;

    private:
        std::vector<GLfloat*> mFloatTexPool;
        std::vector<GLint*> mIntVertPool;
        std::vector<GLint*> mIntTexPool;
        std::vector<int> mVp;
};
#endif

typedef std::vector<DoubleRect*> DoubleRects;

class ImageVertexes
{
    public:
        ImageVertexes();

        ~ImageVertexes();

        Image *image;
        DoubleRects sdl;

#ifdef USE_OPENGL
        NormalOpenGLGraphicsVertexes *ogl;
#endif
};

class GraphicsVertexes
{
    public:
        GraphicsVertexes();

        ~GraphicsVertexes();

        static void setLoadAsOpenGL(int useOpenGL);

        SDLGraphicsVertexes sdl[5];

#ifdef USE_OPENGL
//        SafeOpenGLGraphicsVertexes ogl1[5];

        NormalOpenGLGraphicsVertexes ogl[5];
#endif

        void init(int x, int y, int w, int h);

        void pushSDL(SDL_Rect r1, SDL_Rect r2);

        void clearSDL();

        std::vector<DoubleRect*> *getRectsSDL();

        void incPtr(int num = 1)
        { mPtr += num; }

        void setPtr(int num)
        { mPtr = num; }

#ifdef USE_OPENGL
        NormalOpenGLGraphicsVertexes* getOGL()
        { return &ogl[mPtr]; }
#endif

        int getX()
        { return mX; }

        int getY()
        { return mY; }

        int getW()
        { return mW; }

        int getH()
        { return mH; }

        int mX, mY, mW, mH;
        int mPtr;

    private:
#ifdef USE_OPENGL
        static int mUseOpenGL;
#endif
};

#endif // GRAPHICSVERTEXES_H
