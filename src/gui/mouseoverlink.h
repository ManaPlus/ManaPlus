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

#ifndef GUI_MOUSEOVERLINK_H
#define GUI_MOUSEOVERLINK_H

#include "gui/browserlink.h"

#include "localconsts.h"

struct MouseOverLink final
{
    MouseOverLink(const int x,
                  const int y) :
        mX(x),
        mY(y)
    { }

    A_DEFAULT_COPY(MouseOverLink)

    bool operator() (const BrowserLink &link) const
    {
        return (mX >= link.x1 &&
            mX < link.x2 &&
            mY >= link.y1 &&
            mY < link.y2);
    }
    const int mX;
    const int mY;
};

#endif  // GUI_MOUSEOVERLINK_H
