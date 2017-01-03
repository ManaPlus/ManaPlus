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

#ifndef RESOURCES_SDL2SOFTWARESCREENSHOTHELPER_H
#define RESOURCES_SDL2SOFTWARESCREENSHOTHELPER_H

#ifdef USE_SDL2

#include "resources/screenshothelper.h"

#include "localconsts.h"

class Sdl2SoftwareScreenshotHelper final : public ScreenshotHelper
{
    public:
        Sdl2SoftwareScreenshotHelper();

        A_DELETE_COPY(Sdl2SoftwareScreenshotHelper)

        ~Sdl2SoftwareScreenshotHelper();

        void prepare() override final;

        SDL_Surface *getScreenshot() override final;
};

#endif  // USE_SDL2
#endif  // RESOURCES_SDL2SOFTWARESCREENSHOTHELPER_H
