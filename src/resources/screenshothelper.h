/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_SCREENSHOTHELPER_H
#define RESOURCES_SCREENSHOTHELPER_H

#ifdef USE_SDL2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#endif  // USE_SDL2
#include <SDL_stdinc.h>
#ifdef USE_SDL2
#pragma GCC diagnostic pop
#endif  // USE_SDL2

_SDL_stdinc_h
#include <SDL_video.h>

#include "localconsts.h"

class ScreenshotHelper notfinal
{
    public:
        ScreenshotHelper()
        { }

        A_DELETE_COPY(ScreenshotHelper)

        virtual ~ScreenshotHelper()
        { }

        virtual void prepare() = 0;

        virtual SDL_Surface *getScreenshot() = 0;
};

extern ScreenshotHelper *screenshortHelper;

#endif  // RESOURCES_SCREENSHOTHELPER_H
