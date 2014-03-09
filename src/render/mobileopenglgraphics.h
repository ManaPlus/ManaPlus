/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RENDER_MOBILEOPENGLGRAPHICS_H
#define RENDER_MOBILEOPENGLGRAPHICS_H

#include "main.h"
#ifdef USE_OPENGL

#include "localconsts.h"
#include "render/graphics.h"

#include "resources/fboinfo.h"

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#else
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL_opengl.h>
#include <GL/glext.h>
#endif

#include <set>

class NormalOpenGLGraphicsVertexes;

class MobileOpenGLGraphics final : public Graphics
{
    public:
        MobileOpenGLGraphics();

        A_DELETE_COPY(MobileOpenGLGraphics)

        ~MobileOpenGLGraphics();

        inline void drawTriangleArrayfs(const GLshort *const shortVertArray,
                                        const GLfloat *const floatTexArray,
                                        const int size);

        inline void drawTriangleArrayfs(const int size);

        inline void drawTriangleArrayfsCached(const int size);

        inline void drawLineArrays(const int size);

        #include "render/graphicsdef.hpp"

        #include "render/openglgraphicsdef.hpp"

        #include "render/openglgraphicsdefadvanced.hpp"

    private:
        GLfloat *mFloatTexArray;
        GLshort *mShortVertArray;
        GLfloat *mFloatTexArrayCached;
        GLshort *mShortVertArrayCached;
        float mAlphaCached;
        int mVpCached;
        bool mTexture;

        bool mIsByteColor;
        Color mByteColor;
        GLuint mImageCached;
        float mFloatColor;
        int mMaxVertices;
        bool mColorAlpha;
#ifdef DEBUG_BIND_TEXTURE
        std::string mOldTexture;
        unsigned mOldTextureId;
#endif
        FBOInfo mFbo;
};
#endif

#endif  // RENDER_MOBILEOPENGLGRAPHICS_H
