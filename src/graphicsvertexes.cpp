/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifdef USE_OPENGL
#include "render/graphics.h"
#endif

#include "utils/dtor.h"

#include "debug.h"

#ifdef USE_OPENGL
unsigned int vertexBufSize = 500;
#endif

SDLGraphicsVertexes::SDLGraphicsVertexes() :
    mList()
{
    mList.reserve(30);
}

SDLGraphicsVertexes::~SDLGraphicsVertexes()
{
    delete_all(mList);
}

#ifdef USE_OPENGL
OpenGLGraphicsVertexes::OpenGLGraphicsVertexes() :
    ptr(0),
    mFloatTexArray(nullptr),
    mIntTexArray(nullptr),
    mIntVertArray(nullptr),
    mShortVertArray(nullptr),
    mVp(),
    mFloatTexPool(),
    mIntVertPool(),
    mShortVertPool(),
    mIntTexPool(),
    mVbo()
{
    mFloatTexPool.reserve(30);
    mIntVertPool.reserve(30);
    mShortVertPool.reserve(30);
    mIntTexPool.reserve(30);
    mVp.reserve(30);
    mVbo.reserve(30);
}

OpenGLGraphicsVertexes::~OpenGLGraphicsVertexes()
{
    clear();
}

void OpenGLGraphicsVertexes::clear() restrict2
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

    const int sz = static_cast<int>(mVbo.size());
    if (sz > 0)
    {
        mainGraphics->removeArray(sz, &mVbo[0]);
        mVbo.clear();
    }

    mVp.clear();
    if (ptr)
    {
        ptr = 0;
        delete []mFloatTexArray;
        mFloatTexArray = nullptr;
        delete []mIntTexArray;
        mIntTexArray = nullptr;
        delete []mIntVertArray;
        mIntVertArray = nullptr;
        delete []mShortVertArray;
        mShortVertArray = nullptr;
    }
}

void OpenGLGraphicsVertexes::init() restrict2
{
    clear();
}

GLfloat *OpenGLGraphicsVertexes::switchFloatTexArray() restrict2
{
    mFloatTexArray = new GLfloat[static_cast<size_t>(vertexBufSize * 4 + 30)];
    mFloatTexPool.push_back(mFloatTexArray);
    return mFloatTexArray;
}

GLint *OpenGLGraphicsVertexes::switchIntVertArray() restrict2
{
    mIntVertArray = new GLint[static_cast<size_t>(vertexBufSize * 4 + 30)];
    mIntVertPool.push_back(mIntVertArray);
    return mIntVertArray;
}

GLshort *OpenGLGraphicsVertexes::switchShortVertArray() restrict2
{
    mShortVertArray = new GLshort[static_cast<size_t>(vertexBufSize * 4 + 30)];
    mShortVertPool.push_back(mShortVertArray);
    return mShortVertArray;
}

GLint *OpenGLGraphicsVertexes::switchIntTexArray() restrict2
{
    mIntTexArray = new GLint[static_cast<size_t>(vertexBufSize * 4 + 30)];
    mIntTexPool.push_back(mIntTexArray);
    return mIntTexArray;
}

void OpenGLGraphicsVertexes::switchVp(const int n) restrict2
{
    mVp.push_back(n);
}

int OpenGLGraphicsVertexes::continueVp() restrict2
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

void OpenGLGraphicsVertexes::updateVp(const int n) restrict2
{
    if (!mVp.empty())
        mVp.pop_back();
    mVp.push_back(n);
}

GLfloat *OpenGLGraphicsVertexes::continueFloatTexArray() restrict2
{
    if (mFloatTexPool.empty())
    {
        mFloatTexArray = new GLfloat[static_cast<size_t>(
            vertexBufSize * 4 + 30)];
        mFloatTexPool.push_back(mFloatTexArray);
    }
    else
    {
        mFloatTexArray = mFloatTexPool.back();
    }
    return mFloatTexArray;
}

GLint *OpenGLGraphicsVertexes::continueIntVertArray() restrict2
{
    if (mIntVertPool.empty())
    {
        mIntVertArray = new GLint[static_cast<size_t>(vertexBufSize * 4 + 30)];
        mIntVertPool.push_back(mIntVertArray);
    }
    else
    {
        mIntVertArray = mIntVertPool.back();
    }
    return mIntVertArray;
}

GLshort *OpenGLGraphicsVertexes::continueShortVertArray() restrict2
{
    if (mShortVertPool.empty())
    {
        mShortVertArray = new GLshort[static_cast<size_t>(
            vertexBufSize * 4 + 30)];
        mShortVertPool.push_back(mShortVertArray);
    }
    else
    {
        mShortVertArray = mShortVertPool.back();
    }
    return mShortVertArray;
}

GLint *OpenGLGraphicsVertexes::continueIntTexArray() restrict2
{
    if (mIntTexPool.empty())
    {
        mIntTexArray = new GLint[static_cast<size_t>(vertexBufSize * 4 + 30)];
        mIntTexPool.push_back(mIntTexArray);
    }
    else
    {
        mIntTexArray = mIntTexPool.back();
    }
    return mIntTexArray;
}
#endif

ImageVertexes::ImageVertexes() :
    image(nullptr),
#ifdef USE_OPENGL
    ogl(),
#endif
    sdl()
{
    sdl.reserve(30);
}

ImageVertexes::~ImageVertexes()
{
    delete_all(sdl);
    sdl.clear();
}

ImageCollection::ImageCollection() :
#ifdef USE_OPENGL
    currentGLImage(0),
#endif
    currentImage(nullptr),
    currentVert(nullptr),
    draws()
{
}

ImageCollection::~ImageCollection()
{
    clear();
}

void ImageCollection::clear() restrict2
{
#ifdef USE_OPENGL
    currentGLImage = 0;
#endif
    currentImage = nullptr;
    currentVert = nullptr;

    delete_all(draws);
    draws.clear();
}
