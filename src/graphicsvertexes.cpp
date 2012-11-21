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

#include "graphicsvertexes.h"

#include "utils/dtor.h"

#include "debug.h"

#ifdef USE_OPENGL
unsigned int vertexBufSize = 500;
int GraphicsVertexes::mUseOpenGL = 0;
#endif

SDLGraphicsVertexes::SDLGraphicsVertexes()
{
    mList.reserve(30);
}

SDLGraphicsVertexes::~SDLGraphicsVertexes()
{
    delete_all(mList);
}

#ifdef USE_OPENGL
NormalOpenGLGraphicsVertexes::NormalOpenGLGraphicsVertexes() :
    ptr(0),
    mFloatTexArray(nullptr),
    mIntTexArray(nullptr),
    mIntVertArray(nullptr),
    mShortVertArray(nullptr)
{
    mFloatTexPool.reserve(30);
    mIntVertPool.reserve(30);
    mShortVertPool.reserve(30);
    mIntTexPool.reserve(30);
    mVp.reserve(30);
}

NormalOpenGLGraphicsVertexes::~NormalOpenGLGraphicsVertexes()
{
    clear();
}

void NormalOpenGLGraphicsVertexes::clear()
{
    for (std::vector<GLfloat*>::iterator it = mFloatTexPool.begin();
        it != mFloatTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    mFloatTexPool.clear();

    for (std::vector<GLint*>::iterator it = mIntVertPool.begin();
        it != mIntVertPool.end(); ++ it)
    {
        delete [] (*it);
    }
    mIntVertPool.clear();

    for (std::vector<GLshort*>::iterator it = mShortVertPool.begin();
        it != mShortVertPool.end(); ++ it)
    {
        delete [] (*it);
    }
    mShortVertPool.clear();

    for (std::vector<GLint*>::iterator it = mIntTexPool.begin();
        it != mIntTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    mIntTexPool.clear();

    mVp.clear();
    if (ptr)
    {
        ptr = 0;
        delete []mFloatTexArray;
        delete []mIntTexArray;
        delete []mIntVertArray;
        delete []mShortVertArray;
    }
}

void NormalOpenGLGraphicsVertexes::init()
{
    clear();
}

GLfloat *NormalOpenGLGraphicsVertexes::switchFloatTexArray()
{
    mFloatTexArray = new GLfloat[vertexBufSize * 4 + 30];
    mFloatTexPool.push_back(mFloatTexArray);
    return mFloatTexArray;
}

GLint *NormalOpenGLGraphicsVertexes::switchIntVertArray()
{
    mIntVertArray = new GLint[vertexBufSize * 4 + 30];
    mIntVertPool.push_back(mIntVertArray);
    return mIntVertArray;
}

GLshort *NormalOpenGLGraphicsVertexes::switchShortVertArray()
{
    mShortVertArray = new GLshort[vertexBufSize * 4 + 30];
    mShortVertPool.push_back(mShortVertArray);
    return mShortVertArray;
}

GLint *NormalOpenGLGraphicsVertexes::switchIntTexArray()
{
    mIntTexArray = new GLint[vertexBufSize * 4 + 30];
    mIntTexPool.push_back(mIntTexArray);
    return mIntTexArray;
}

void NormalOpenGLGraphicsVertexes::switchVp(int n)
{
    mVp.push_back(n);
}

int NormalOpenGLGraphicsVertexes::continueVp()
{
    if (mVp.empty())
    {
        return 0;
    }
    else
    {
        const int val = mVp.back();
        mVp.pop_back();
        return val;
    }
}

void NormalOpenGLGraphicsVertexes::updateVp(int n)
{
    if (!mVp.empty())
        mVp.pop_back();
    mVp.push_back(n);
}

GLfloat *NormalOpenGLGraphicsVertexes::continueFloatTexArray()
{
    if (mFloatTexPool.empty())
    {
        mFloatTexArray = new GLfloat[vertexBufSize * 4 + 30];
        mFloatTexPool.push_back(mFloatTexArray);
    }
    else
    {
        mFloatTexArray = mFloatTexPool.back();
    }
    return mFloatTexArray;
}

GLint *NormalOpenGLGraphicsVertexes::continueIntVertArray()
{
    if (mIntVertPool.empty())
    {
        mIntVertArray = new GLint[vertexBufSize * 4 + 30];
        mIntVertPool.push_back(mIntVertArray);
    }
    else
    {
        mIntVertArray = mIntVertPool.back();
    }
    return mIntVertArray;
}

GLshort *NormalOpenGLGraphicsVertexes::continueShortVertArray()
{
    if (mShortVertPool.empty())
    {
        mShortVertArray = new GLshort[vertexBufSize * 4 + 30];
        mShortVertPool.push_back(mShortVertArray);
    }
    else
    {
        mShortVertArray = mShortVertPool.back();
    }
    return mShortVertArray;
}

GLint *NormalOpenGLGraphicsVertexes::continueIntTexArray()
{
    if (mIntTexPool.empty())
    {
        mIntTexArray = new GLint[vertexBufSize * 4 + 30];
        mIntTexPool.push_back(mIntTexArray);
    }
    else
    {
        mIntTexArray = mIntTexPool.back();
    }
    return mIntTexArray;
}
#endif

GraphicsVertexes::GraphicsVertexes() :
    mX(0), mY(0),
    mW(0), mH(0),
    mPtr(0)
{
}

GraphicsVertexes::~GraphicsVertexes()
{
}

void GraphicsVertexes::init(const int x, const int y, const int w, const int h)
{
    mPtr = 0;
    mX = x;
    mY = y;
    mW = w;
    mH = h;
    for (int f = 0; f < 10; f ++)
    {
        delete_all(sdl[mPtr].mList);
        sdl[mPtr].mList.clear();
#ifdef USE_OPENGL
        ogl[mPtr].init();
#endif
    }
}

#ifdef USE_OPENGL
void GraphicsVertexes::setLoadAsOpenGL(int useOpenGL)
{
    mUseOpenGL = useOpenGL;
}
#endif

void GraphicsVertexes::pushSDL(const SDL_Rect &r1, const SDL_Rect &r2)
{
    DoubleRect *const r = new DoubleRect();
    r->src = r1;
    r->dst = r2;
    sdl[mPtr].mList.push_back(r);
}

void GraphicsVertexes::clearSDL()
{
    delete_all(sdl[mPtr].mList);
    sdl[mPtr].mList.clear();
}

std::vector<DoubleRect*> *GraphicsVertexes::getRectsSDL()
{
    return &sdl[mPtr].mList;
}

const std::vector<DoubleRect*> *GraphicsVertexes::getRectsSDLconst() const
{
    return &sdl[mPtr].mList;
}

ImageVertexes::ImageVertexes() :
    image(nullptr)
{
    sdl.reserve(30);
}

ImageVertexes::~ImageVertexes()
{
    delete_all(sdl);
    sdl.clear();
}
