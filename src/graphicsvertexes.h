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

#ifndef GRAPHICSVERTEXES_H
#define GRAPHICSVERTEXES_H

#include "main.h"

#include "localconsts.h"

#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
#else
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL_opengl.h>
#endif

#endif

#include <SDL_video.h>

#include <vector>

class Image;

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
class OpenGLGraphicsVertexes final
{
    public:
        OpenGLGraphicsVertexes();

        A_DELETE_COPY(OpenGLGraphicsVertexes)

        ~OpenGLGraphicsVertexes();

        GLfloat *switchFloatTexArray() restrict2;

        GLint *switchIntVertArray() restrict2;

        GLint *switchIntTexArray() restrict2;

        GLshort *switchShortVertArray() restrict2;

        std::vector<GLfloat*> *getFloatTexPool() restrict2 A_WARN_UNUSED
        { return &mFloatTexPool; }

        std::vector<GLint*> *getIntVertPool() restrict2 A_WARN_UNUSED
        { return &mIntVertPool; }

        std::vector<GLshort*> *getShortVertPool() restrict2 A_WARN_UNUSED
        { return &mShortVertPool; }

        std::vector<GLint*> *getIntTexPool() restrict2 A_WARN_UNUSED
        { return &mIntTexPool; }

        void switchVp(const int n) restrict2;

        GLfloat *continueFloatTexArray() restrict2 RETURNS_NONNULL;

        GLint *continueIntVertArray() restrict2 RETURNS_NONNULL;

        GLshort *continueShortVertArray() restrict2 RETURNS_NONNULL;

        GLint *continueIntTexArray() restrict2 RETURNS_NONNULL;

        int continueVp() restrict2;

        void updateVp(const int n) restrict2;

        std::vector<int> *getVp() restrict2 A_WARN_UNUSED
        { return &mVp; }

        void init() restrict2;

        void clear() restrict2;

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
        std::vector<GLuint> mVbo;
};
#endif

typedef std::vector<DoubleRect*> DoubleRects;

class ImageVertexes final
{
    public:
        ImageVertexes();

        A_DELETE_COPY(ImageVertexes)

        ~ImageVertexes();

        const Image *restrict image;
#ifdef USE_OPENGL
        OpenGLGraphicsVertexes ogl;
#endif
        DoubleRects sdl;
};

typedef std::vector<ImageVertexes*> ImageVertexesVector;
typedef ImageVertexesVector::iterator ImageCollectionIter;
typedef ImageVertexesVector::const_iterator ImageCollectionCIter;

class ImageCollection final
{
    public:
        ImageCollection();

        A_DELETE_COPY(ImageCollection)

        ~ImageCollection();

        void clear() restrict2;

#ifdef USE_OPENGL
        GLuint currentGLImage;
#endif
        const Image *restrict currentImage;

        ImageVertexes *restrict currentVert;

        ImageVertexesVector draws;
};

#ifdef USE_OPENGL
extern unsigned int vertexBufSize;
#endif

#endif  // GRAPHICSVERTEXES_H
