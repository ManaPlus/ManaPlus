/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

        void switchVp(const int n);

        GLfloat *continueFloatTexArray();

        GLint *continueIntVertArray();

        GLshort *continueShortVertArray();

        GLint *continueIntTexArray();

        int continueVp();

        void updateVp(const int n);

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

        const Image *image;
#ifdef USE_OPENGL
        NormalOpenGLGraphicsVertexes ogl;
#endif
        DoubleRects sdl;
};

typedef std::vector<ImageVertexes*> ImageVertexesVector;
typedef ImageVertexesVector::const_iterator ImageCollectionCIter;

class ImageCollection final
{
    public:
        ImageCollection();

        A_DELETE_COPY(ImageCollection)

        ~ImageCollection();

        void clear();

#ifdef USE_OPENGL
        GLuint currentGLImage;
#endif
        const Image *currentImage;

        ImageVertexes *currentVert;

        ImageVertexesVector draws;
};

#ifdef USE_OPENGL
extern unsigned int vertexBufSize;
#endif

#endif  // GRAPHICSVERTEXES_H
