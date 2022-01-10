/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_BROWSERLINK_H
#define GUI_BROWSERLINK_H

#include <string>

#include "localconsts.h"

struct BrowserLink final
{
    BrowserLink() :
        x1(0),
        x2(0),
        y1(0),
        y2(0),
        link(),
        caption()
    {
    }

    A_DEFAULT_COPY(BrowserLink)

    int x1;
    int x2;
    int y1;
    int y2;
    std::string link;
    std::string caption;
};

#endif  // GUI_BROWSERLINK_H
