/*
 *  The ManaPlus Client
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "graphics.h"

#ifdef USE_OPENGL
#define NO_SDL_GLEXT

#include <SDL_opengl.h>
#endif

#include <string>
#include <list>
#include <vector>

//include <guichan/sdl/sdlgraphics.hpp>


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

        std::list<DoubleRect*> mList;
};


class OpenGL1GraphicsVertexes
{
};

class OpenGLGraphicsVertexes
{
    public:
        OpenGLGraphicsVertexes();

        ~OpenGLGraphicsVertexes();

        GLfloat *switchFloatTexArray();

        GLint *switchIntVertArray();

        GLint *switchIntTexArray();

        void init();

        void clear();

        std::vector<GLfloat*> &getFloatTexPool()
        { return mFloatTexPool; }

        std::vector<GLint*> &getIntVertPool()
        { return mIntVertPool; }

        std::vector<GLint*> &getIntTexPool()
        { return mIntTexPool; }

        void switchVp(int n);

        std::vector<int> &getVp()
        { return mVp; }

    private:
        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;
        std::vector<GLfloat*> mFloatTexPool;
        std::vector<GLint*> mIntVertPool;
        std::vector<GLint*> mIntTexPool;
        std::vector<int> mVp;
};

class GraphicsVertexes
{
    public:
        GraphicsVertexes();

        ~GraphicsVertexes();

        static void setLoadAsOpenGL(int useOpenGL);

        SDLGraphicsVertexes sdl[5];

        OpenGL1GraphicsVertexes ogl1[5];

        OpenGLGraphicsVertexes ogl[5];

        void init(int x, int y, int w, int h);

        void pushSDL(SDL_Rect r1, SDL_Rect r2);

        void clearSDL();

        std::list<DoubleRect*> *getRectsSDL();

        void incPtr(int num = 1)
        { mPtr += num; }

        void setPtr(int num)
        { mPtr = num; }

        OpenGLGraphicsVertexes* getOGL()
        { return &ogl[mPtr]; }

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
        static int mUseOpenGL;
};

#endif // GRAPHICSVERTEXES_H
