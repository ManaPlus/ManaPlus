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

#ifdef ANDROID
#include <GLES/gl.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL/SDL_opengl.h>
#endif

#include <SDL/SDL.h>

#include "mobileopenglgraphics.h"
#include "normalopenglgraphics.h"
#include "safeopenglgraphics.h"
#endif

#include "resources/image.h"

#include <string>
#include <list>
#include <vector>

struct DoubleRect final
{
    SDL_Rect src;
    SDL_Rect dst;
};

class SDLGraphicsVertexes final
{
    public:
        SDLGraphicsVertexes();

        A_DELETE_COPY(SDLGraphicsVertexes)

        ~SDLGraphicsVertexes();

        std::vector<DoubleRect*> mList;
};

#ifdef USE_OPENGL
class SafeOpenGLGraphicsVertexes final
{
};

class NormalOpenGLGraphicsVertexes final
{
    public:
        NormalOpenGLGraphicsVertexes();

        A_DELETE_COPY(NormalOpenGLGraphicsVertexes)

        ~NormalOpenGLGraphicsVertexes();

        GLfloat *switchFloatTexArray();

        GLint *switchIntVertArray();

        GLint *switchIntTexArray();

        GLshort *switchShortVertArray();

        std::vector<GLfloat*> *getFloatTexPool() A_WARN_UNUSED
        { return &mFloatTexPool; }

        std::vector<GLint*> *getIntVertPool() A_WARN_UNUSED
        { return &mIntVertPool; }

        std::vector<GLshort*> *getShortVertPool() A_WARN_UNUSED
        { return &mShortVertPool; }

        std::vector<GLint*> *getIntTexPool() A_WARN_UNUSED
        { return &mIntTexPool; }

        void switchVp(int n);

        GLfloat *continueFloatTexArray();

        GLint *continueIntVertArray();

        GLshort *continueShortVertArray();

        GLint *continueIntTexArray();

        int continueVp();

        void updateVp(int n);

        std::vector<int> *getVp() A_WARN_UNUSED
        { return &mVp; }

        void init();

        void clear();

        int ptr;

        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;
        GLshort *mShortVertArray;
        std::vector<int> mVp;
        std::vector<GLfloat*> mFloatTexPool;
        std::vector<GLint*> mIntVertPool;
        std::vector<GLshort*> mShortVertPool;
        std::vector<GLint*> mIntTexPool;
};
#endif

typedef std::vector<DoubleRect*> DoubleRects;

class ImageVertexes final
{
    public:
        ImageVertexes();

        A_DELETE_COPY(ImageVertexes)

        ~ImageVertexes();

        Image *image;
        DoubleRects sdl;

#ifdef USE_OPENGL
        NormalOpenGLGraphicsVertexes ogl;
#endif
};

class GraphicsVertexes final
{
    public:
        GraphicsVertexes();

        A_DELETE_COPY(GraphicsVertexes)

        ~GraphicsVertexes();

        static void setLoadAsOpenGL(int useOpenGL);

        SDLGraphicsVertexes sdl[5];

#ifdef USE_OPENGL
//        SafeOpenGLGraphicsVertexes ogl1[5];

        NormalOpenGLGraphicsVertexes ogl[5];
#endif

        void init(const int x, const int y, const int w, const int h);

        void pushSDL(const SDL_Rect &r1, const SDL_Rect &r2);

        void clearSDL();

        std::vector<DoubleRect*> *getRectsSDL();

        void incPtr(const int num = 1)
        { mPtr += num; }

        void setPtr(const int num)
        { mPtr = num; }

#ifdef USE_OPENGL
        NormalOpenGLGraphicsVertexes &getOGL() A_WARN_UNUSED
        { return ogl[mPtr]; }
#endif

        int getX() const A_WARN_UNUSED
        { return mX; }

        int getY() const A_WARN_UNUSED
        { return mY; }

        int getW() const A_WARN_UNUSED
        { return mW; }

        int getH() const A_WARN_UNUSED
        { return mH; }

        int mX, mY, mW, mH;
        int mPtr;

    private:
#ifdef USE_OPENGL
        static int mUseOpenGL;
#endif
};

#ifdef USE_OPENGL
extern unsigned int vertexBufSize;
#endif

#endif // GRAPHICSVERTEXES_H
