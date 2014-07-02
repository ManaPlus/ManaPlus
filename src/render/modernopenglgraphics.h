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

#ifndef RENDER_MODERNOPENGLGRAPHICS_H
#define RENDER_MODERNOPENGLGRAPHICS_H

#include "main.h"
#if defined USE_OPENGL && !defined ANDROID

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

class OpenGLGraphicsVertexes;
class ShaderProgram;

class ModernOpenGLGraphics final : public Graphics
{
    public:
        ModernOpenGLGraphics();

        A_DELETE_COPY(ModernOpenGLGraphics)

        ~ModernOpenGLGraphics();

        void postInit() override final;

        void setColor(const Color &color) override final;

        void setColorAll(const Color &color,
                         const Color &color2) override final;

        void screenResized() override final;

        void finalize(ImageCollection *const col) override final;

        void finalize(ImageVertexes *const vert) override final;

        void testDraw() override final;

        void removeArray(const uint32_t id,
                         uint32_t *const arr) override final;

        #include "render/graphicsdef.hpp"

        #include "render/openglgraphicsdef.hpp"

        #include "render/openglgraphicsdefadvanced.hpp"

    protected:
        virtual void createGLContext() override final;

    private:
        void deleteGLObjects();

        inline void drawQuad(const Image *const image,
                             const int srcX, const int srcY,
                             const int dstX, const int dstY,
                             const int width, const int height);

        inline void drawRescaledQuad(const Image *const image,
                                     const int srcX, const int srcY,
                                     const int dstX, const int dstY,
                                     const int width, const int height,
                                     const int desiredWidth,
                                     const int desiredHeight);

        inline void drawTriangleArray(const int size);

        inline void drawTriangleArray(const GLint *const array,
                                      const int size);

        inline void drawLineArrays(const int size);

        inline void bindArrayBuffer(const GLuint vbo);

        inline void bindArrayBufferAndAttributes(const GLuint vbo);

        inline void bindAttributes();

        inline void bindElementBuffer(const GLuint ebo);

        GLint *mIntArray;
        GLint *mIntArrayCached;
        ShaderProgram *mProgram;
        float mAlphaCached;
        int mVpCached;

        float mFloatColor;
        int mMaxVertices;
        GLuint mProgramId;
        GLuint mSimpleColorUniform;
        GLint mPosAttrib;
        GLint mTextureColorUniform;
        GLuint mScreenUniform;
        GLuint mDrawTypeUniform;
        GLuint mVao;
        GLuint mVbo;
        GLuint mEbo;
        GLuint mVboCached;
        GLuint mEboCached;
        GLuint mAttributesCached;
        bool mColorAlpha;
        bool mTextureDraw;
#ifdef DEBUG_BIND_TEXTURE
        std::string mOldTexture;
        unsigned mOldTextureId;
#endif
        FBOInfo mFbo;
};
#endif

#endif  // RENDER_MODERNOPENGLGRAPHICS_H
