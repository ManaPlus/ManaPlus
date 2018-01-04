/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_CURSOR_H
#define ENUMS_RESOURCES_CURSOR_H

#include "enums/simpletypes/enumdefines.h"

enumStart(Cursor)
{
    CURSOR_POINTER = 0,
    CURSOR_RESIZE_ACROSS,
    CURSOR_RESIZE_DOWN,
    CURSOR_RESIZE_DOWN_LEFT,
    CURSOR_RESIZE_DOWN_RIGHT,
    CURSOR_FIGHT,
    CURSOR_PICKUP,
    CURSOR_TALK,
    CURSOR_ACTION,
    CURSOR_LEFT,
    CURSOR_UP,
    CURSOR_RIGHT,
    CURSOR_DOWN,
    CURSOR_TOTAL
}
enumEnd(Cursor);

#endif  // ENUMS_RESOURCES_CURSOR_H
