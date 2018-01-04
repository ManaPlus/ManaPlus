/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifdef USE_OPENGL

#include "resources/openglscreenshothelper.h"

#include "configuration.h"
#include "graphicsmanager.h"

#include "render/graphics.h"
#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "debug.h"

OpenGLScreenshotHelper::OpenGLScreenshotHelper() :
    ScreenshotHelper(),
    mFbo()
{
}

OpenGLScreenshotHelper::~OpenGLScreenshotHelper()
{
}

void OpenGLScreenshotHelper::prepare()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mainGraphics->mWidth,
            mainGraphics->mHeight,
            &mFbo);
}

SDL_Surface *OpenGLScreenshotHelper::getScreenshot()
{
    const int h = mainGraphics->mHeight;
    const int w = mainGraphics->mWidth - (mainGraphics->mWidth % 4);
    GLint pack = 1;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h, 24,
        0xff0000, 0x00ff00, 0x0000ff, 0x000000);

    if (screenshot == nullptr)
        return nullptr;

    if (SDL_MUSTLOCK(screenshot))
        SDL_LockSurface(screenshot);

    const size_t lineSize = 3 * w;
    GLubyte *const buf = new GLubyte[lineSize];

    // Grap the pixel buffer and write it to the SDL surface
    mglGetIntegerv(GL_PACK_ALIGNMENT, &pack);
    mglPixelStorei(GL_PACK_ALIGNMENT, 1);
    mglReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, screenshot->pixels);

    // Flip the screenshot, as OpenGL has 0,0 in bottom left
    const int h2 = h / 2;
    for (int i = 0; i < h2; i++)
    {
        GLubyte *const top = static_cast<GLubyte*>(
            screenshot->pixels) + lineSize * i;
        GLubyte *const bot = static_cast<GLubyte*>(
            screenshot->pixels) + lineSize * (h - 1 - i);

        memcpy(buf, top, lineSize);
        memcpy(top, bot, lineSize);
        memcpy(bot, buf, lineSize);
    }

    delete [] buf;

    if (config.getBoolValue("usefbo"))
        graphicsManager.deleteFBO(&mFbo);

    mglPixelStorei(GL_PACK_ALIGNMENT, pack);

    if (SDL_MUSTLOCK(screenshot))
        SDL_UnlockSurface(screenshot);

    return screenshot;
}

#endif  // USE_OPENGL
