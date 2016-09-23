/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "resources/sdlscreenshothelper.h"

#include "render/graphics.h"

#include "utils/sdlcheckutils.h"

#include <SDL_endian.h>

#include "debug.h"

#ifndef SDL_BYTEORDER
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

SdlScreenshotHelper::SdlScreenshotHelper() :
    ScreenshotHelper()
{
}

SdlScreenshotHelper::~SdlScreenshotHelper()
{
}

void SdlScreenshotHelper::prepare()
{
}

SDL_Surface *SdlScreenshotHelper::getScreenshot()
{
    if (!mainGraphics)
        return nullptr;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const int amask = 0x00000000;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(SDL_SWSURFACE,
        mainGraphics->mWidth, mainGraphics->mHeight,
        24,
        rmask, gmask, bmask, amask);

#ifndef USE_SDL2
    if (screenshot)
        SDL_BlitSurface(mainGraphics->mWindow, nullptr, screenshot, nullptr);
#endif  // USE_SDL2

    return screenshot;
}
