/*
 *  The ManaPlus Client
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

#include "net/useragent.h"

#include "configuration.h"
#include "settings.h"
#include "main.h"

#include "render/graphics.h"

#include "debug.h"

void UserAgent::update()
{
    std::string renderName;
    if (mainGraphics != nullptr)
        renderName = mainGraphics->getName();
    else
        renderName = "unknown";

    settings.userAgent = strprintf(PACKAGE_EXTENDED_VERSION,
        branding.getStringValue("appName").c_str(),
        renderName.c_str());
}
