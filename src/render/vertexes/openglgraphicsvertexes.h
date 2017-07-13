/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RENDER_VERTEXES_OPENGLGRAPHICSVERTEXES_H
#define RENDER_VERTEXES_OPENGLGRAPHICSVERTEXES_H

#ifdef USE_OPENGL

#include "localconsts.h"

#ifdef ANDROID
#include <GLES/gl.h>
#else  // ANDROID
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_opengl.h>
PRAGMA48(GCC diagnostic pop)
#endif  // ANDROID

#include "utils/vector.h"

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

extern unsigned int vertexBufSize;
#endif  // USE_OPENGL

#endif  // RENDER_VERTEXES_OPENGLGRAPHICSVERTEXES_H
