/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef RENDER_NULLOPENGLGRAPHICS_H
#define RENDER_NULLOPENGLGRAPHICS_H

#include "main.h"
#if defined USE_OPENGL

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

class NullOpenGLGraphics final : public Graphics
{
    public:
        NullOpenGLGraphics();

        A_DELETE_COPY(NullOpenGLGraphics)

        ~NullOpenGLGraphics();

        bool setVideoMode(const int w, const int h,
                          const int bpp,
                          const bool fs,
                          const bool hwaccel,
                          const bool resize,
                          const bool noFrame) override final;


        /**
         * Draws a resclaled version of the image
         */
        bool drawRescaledImage(const Image *const image, int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth, const int desiredHeight,
                               const bool useColor) override final;

        /**
         * Used to get the smooth rescale option over the standard function.
         */
        bool drawRescaledImage(const Image *const image, int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth, const int desiredHeight,
                               const bool useColor, bool smooth);

        void drawImagePattern(const Image *const image,
                              const int x, const int y,
                              const int w, const int h) override final;

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        void drawRescaledImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const int scaledWidth,
                                      const int scaledHeight) override final;

        void calcImagePattern(ImageVertexes* const vert,
                              const Image *const image,
                              const int x, const int y,
                              const int w, const int h) const override final;

        void calcImagePattern(ImageCollection* const vert,
                              const Image *const image,
                              const int x, const int y,
                              const int w, const int h) const override final;

        void calcTileVertexes(ImageVertexes *const vert,
                              const Image *const image,
                              int x, int y) const override final;

        void calcTileCollection(ImageCollection *const vertCol,
                                const Image *const image,
                                int x, int y) override final;

        void drawTileCollection(const ImageCollection *const vertCol)
                                override final;

        void drawTileVertexes(const ImageVertexes *const vert) override final;

        bool calcWindow(ImageCollection *const vertCol,
                        const int x, const int y,
                        const int w, const int h,
                        const ImageRect &imgRect) override final;

        void updateScreen() override final;

        void _beginDraw() override final;

        void _endDraw() override final;

        bool pushClipArea(gcn::Rectangle area) override final;

        void popClipArea() override final;

        void setColor(const gcn::Color &color) override final
        {
            mColor = color;
            mColor2 = color;
            mColorAlpha = (color.a != 255);
        }

        void setColorAll(const gcn::Color &color, const gcn::Color &color2)
        {
            mColor = color;
            mColor2 = color2;
            mColorAlpha = (color.a != 255);
        }

        void drawPoint(int x, int y) override final;

        void drawLine(int x1, int y1, int x2, int y2) override final;

        void drawRectangle(const gcn::Rectangle &rect,
                           const bool filled);

        void drawRectangle(const gcn::Rectangle &rect) override final;

        void fillRectangle(const gcn::Rectangle &rect) override final;

        inline void drawQuadArrayfi(const int size);

        inline void drawQuadArrayfi(const GLint *const intVertArray,
                                    const GLfloat *const floatTexArray,
                                    const int size);

        inline void drawQuadArrayii(const int size);

        inline void drawQuadArrayii(const GLint *const intVertArray,
                                    const GLint *const intTexArray,
                                    const int size);

        inline void drawLineArrayi(const int size);

        inline void drawLineArrayf(const int size);

        inline void drawVertexes(const NormalOpenGLGraphicsVertexes &ogl);

        void initArrays() override final;

        static void dumpSettings();

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        SDL_Surface *getScreenshot() override final A_WARN_UNUSED;

        void prepareScreenshot() override final;

        bool drawNet(const int x1, const int y1, const int x2, const int y2,
                     const int width, const int height) override final;

        int getMemoryUsage() A_WARN_UNUSED;

        void updateTextureFormat();

        bool drawImage2(const Image *const image,
                        int srcX, int srcY,
                        int dstX, int dstY,
                        const int width, const int height,
                        const bool useColor) override final;

#ifdef DEBUG_DRAW_CALLS
        unsigned int getDrawCalls() const
        { return mLastDrawCalls; }

        static unsigned int mDrawCalls;

        static unsigned int mLastDrawCalls;
#endif

        static void bindTexture(const GLenum target, const GLuint texture);

        static GLuint mLastImage;

    protected:
        void setTexturingAndBlending(const bool enable);

        void debugBindTexture(const Image *const image);

    private:
        void inline setColorAlpha(const float alpha);

        void inline restoreColor();

        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;
        bool mTexture;

        bool mIsByteColor;
        gcn::Color mByteColor;
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

#endif  // RENDER_NULLOPENGLGRAPHICS_H
