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

#ifndef RESOURCES_SCREENSHOTHELPER_H
#define RESOURCES_SCREENSHOTHELPER_H

#include "localconsts.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

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
