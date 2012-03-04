/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef OPENGLGRAPHICS_H
#define OPENGLGRAPHICS_H

#ifdef USE_OPENGL

#include "main.h"
#include "graphics.h"

//#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
//#include <GL/glext.h>

class OpenGLGraphics : public Graphics
{
    public:
        OpenGLGraphics();

        ~OpenGLGraphics();

        /**
         * Sets whether vertical refresh syncing is enabled. Takes effect after
         * the next call to setVideoMode(). Only implemented on MacOS for now.
         */
        void setSync(bool sync);

        bool getSync() const
        { return mSync; }

        bool setVideoMode(int w, int h, int bpp, bool fs,
                          bool hwaccel, bool resize, bool noFrame);

        bool drawImage(Image *image,
                       int srcX, int srcY,
                       int dstX, int dstY,
                       int width, int height,
                       bool useColor);

        /**
         * Draws a resclaled version of the image
         */
        bool drawRescaledImage(Image *image, int srcX, int srcY,
                               int dstX, int dstY,
                               int width, int height,
                               int desiredWidth, int desiredHeight,
                               bool useColor);

        /**
         * Used to get the smooth rescale option over the standard function.
         */
        bool drawRescaledImage(Image *image, int srcX, int srcY,
                               int dstX, int dstY,
                               int width, int height,
                               int desiredWidth, int desiredHeight,
                               bool useColor, bool smooth);

        void drawImagePattern(Image *image,
                              int x, int y,
                              int w, int h);

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        void drawRescaledImagePattern(Image *image,
                                      int x, int y, int w, int h,
                                      int scaledWidth, int scaledHeight);

        void calcImagePattern(GraphicsVertexes* vert, Image *image,
                              int x, int y, int w, int h);

        void calcTile(ImageVertexes *vert, int x, int y);

        void drawTile(ImageVertexes *vert);

        void drawImagePattern2(GraphicsVertexes *vert, Image *image);

        void updateScreen();

        void _beginDraw();
        void _endDraw();

        bool pushClipArea(gcn::Rectangle area);
        void popClipArea();

        void setColor(const gcn::Color &color);

        void drawPoint(int x, int y);

        void drawLine(int x1, int y1, int x2, int y2);

        void drawRectangle(const gcn::Rectangle &rect, bool filled);

        void drawRectangle(const gcn::Rectangle &rect);

        void fillRectangle(const gcn::Rectangle &rect);

        void setTargetPlane(int width, int height);

        void drawQuadArrayfi(int size);

        void drawQuadArrayfi(GLint *intVertArray, GLfloat *floatTexArray,
                             int size);

        void drawQuadArrayii(int size);

        void drawQuadArrayii(GLint *intVertArray, GLint *intTexArray,
                             int size);

        void drawLineArrayi(int size);

        void drawLineArrayf(int size);

        static void dumpSettings();

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        SDL_Surface *getScreenshot();

        void prepareScreenshot();

        bool drawNet(int x1, int y1, int x2, int y2, int width, int height);

        static void bindTexture(GLenum target, GLuint texture);

        static GLuint mLastImage;

    protected:
        void setTexturingAndBlending(bool enable);

    private:
        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;
        bool mAlpha, mTexture;
        bool mColorAlpha;
        bool mSync;
        GLuint mFboId;
        GLuint mTextureId;
        GLuint mRboId;
};
#endif

#endif
