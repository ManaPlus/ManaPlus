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

#ifndef OPENGL1GRAPHICS_H
#define OPENGL1GRAPHICS_H

#ifdef USE_OPENGL
#include "main.h"

#include "graphics.h"

//#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
#include <GL/glext.h>

class SafeOpenGLGraphics : public Graphics
{
    public:
        SafeOpenGLGraphics();

        ~SafeOpenGLGraphics();

        bool setVideoMode(int w, int h, int bpp, bool fs,
                          bool hwaccel, bool resize, bool noFrame);

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

        void drawImagePattern(const Image *image,
                              int x, int y,
                              int w, int h);

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        void drawRescaledImagePattern(Image *image,
                                      int x, int y, int w, int h,
                                      int scaledWidth, int scaledHeight);

        bool calcImageRect(GraphicsVertexes* vert,
                           int x, int y, int w, int h,
                           Image *topLeft, Image *topRight,
                           Image *bottomLeft, Image *bottomRight,
                           Image *top, Image *right,
                           Image *bottom, Image *left,
                           Image *center);

        void drawImageRect2(GraphicsVertexes* vert, const ImageRect &imgRect);

        void calcTile(ImageVertexes *vert, int x, int y);

        void drawTile(ImageVertexes *vert);

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

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        SDL_Surface *getScreenshot();

        void prepareScreenshot();

        static void bindTexture(GLenum target, GLuint texture);

        static GLuint mLastImage;

    protected:
        bool drawImage2(const Image *image,
                        int srcX, int srcY,
                        int dstX, int dstY,
                        int width, int height,
                        bool useColor);

        void setTexturingAndBlending(bool enable);

    private:
        bool mAlpha, mTexture;
        bool mColorAlpha;
        GLuint mFboId;
        GLuint mTextureId;
        GLuint mRboId;
};
#endif

#endif
