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

#include "graphicsvertexes.h"

#include "main.h"

#include "utils/dtor.h"

#ifdef USE_OPENGL
int GraphicsVertexes::mUseOpenGL = 0;
const unsigned int vertexBufSize = 500;
#endif


SDLGraphicsVertexes::SDLGraphicsVertexes()
{

}

SDLGraphicsVertexes::~SDLGraphicsVertexes()
{
    delete_all(mList);
}


OpenGLGraphicsVertexes::OpenGLGraphicsVertexes() :
    mFloatTexArray(0),
    mIntTexArray(0),
    mIntVertArray(0)
{
}

OpenGLGraphicsVertexes::~OpenGLGraphicsVertexes()
{
    clear();
}

void OpenGLGraphicsVertexes::clear()
{
    delete_all(mFloatTexPool);
    mFloatTexPool.clear();
    delete_all(mIntVertPool);
    mIntVertPool.clear();
    delete_all(mIntTexPool);
    mIntTexPool.clear();
    mVp.clear();
}

void OpenGLGraphicsVertexes::init()
{
    clear();
}

GLfloat *OpenGLGraphicsVertexes::switchFloatTexArray()
{
    mFloatTexArray = new GLfloat[vertexBufSize * 4 + 30];
    mFloatTexPool.push_back(mFloatTexArray);
    return mFloatTexArray;
}

GLint *OpenGLGraphicsVertexes::switchIntVertArray()
{
    mIntVertArray = new GLint[vertexBufSize * 4 + 30];
    mIntVertPool.push_back(mIntVertArray);
    return mIntVertArray;
}

GLint *OpenGLGraphicsVertexes::switchIntTexArray()
{
    mIntTexArray = new GLint[vertexBufSize * 4 + 30];
    mIntTexPool.push_back(mIntTexArray);
    return mIntTexArray;
}

void OpenGLGraphicsVertexes::switchVp(int n)
{
    mVp.push_back(n);
}

GraphicsVertexes::GraphicsVertexes() :
    mX(0), mY(0),
    mW(0), mH(0),
    mPtr(0)
{
}

GraphicsVertexes::~GraphicsVertexes()
{
}

void GraphicsVertexes::init(int x, int y, int w, int h)
{
    mPtr = 0;
    mX = x;
    mY = y;
    mW = w;
    mH = h;
    for (int f = 0; f < 10; f ++)
    {
        sdl[mPtr].mList.clear();
        ogl[mPtr].init();
    }
}

void GraphicsVertexes::setLoadAsOpenGL(int useOpenGL)
{
    mUseOpenGL = useOpenGL;
}

void GraphicsVertexes::pushSDL(SDL_Rect r1, SDL_Rect r2)
{
    DoubleRect *r = new DoubleRect();
    r->src = r1;
    r->dst = r2;
    sdl[mPtr].mList.push_back(r);
}

void GraphicsVertexes::clearSDL()
{
    sdl[mPtr].mList.clear();
}

std::list<DoubleRect*> *GraphicsVertexes::getRectsSDL()
{
    return &sdl[mPtr].mList;
}
