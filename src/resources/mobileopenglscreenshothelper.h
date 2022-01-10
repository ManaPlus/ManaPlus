/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_MOBILEOPENGLSCREENSHOTHELPER_H
#define RESOURCES_MOBILEOPENGLSCREENSHOTHELPER_H

#if defined(USE_OPENGL) && !defined(ANDROID)

#include "resources/screenshothelper.h"

#include "resources/fboinfo.h"

#include "localconsts.h"

class MobileOpenGLScreenshotHelper final : public ScreenshotHelper
{
    public:
        MobileOpenGLScreenshotHelper();

        A_DELETE_COPY(MobileOpenGLScreenshotHelper)

        ~MobileOpenGLScreenshotHelper() override final;

        void prepare() override final;

        SDL_Surface *getScreenshot() override final;

    private:
        FBOInfo mFbo;
};

#endif  // defined(USE_OPENGL) && !defined(ANDROID)
#endif  // RESOURCES_MOBILEOPENGLSCREENSHOTHELPER_H
