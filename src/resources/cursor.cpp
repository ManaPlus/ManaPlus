/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "resources/cursor.h"

namespace Cursor
{
    static const StrToCursor hoverCursors[] =
    {
        {"select", CURSOR_POINTER},
        {"pointer", CURSOR_POINTER},
        {"lr", CURSOR_RESIZE_ACROSS},
        {"rl", CURSOR_RESIZE_ACROSS},
        {"resizeAcross", CURSOR_RESIZE_ACROSS},
        {"ud", CURSOR_RESIZE_DOWN},
        {"du", CURSOR_RESIZE_DOWN},
        {"resizeDown", CURSOR_RESIZE_DOWN},
        {"ldru", CURSOR_RESIZE_DOWN_LEFT},
        {"ruld", CURSOR_RESIZE_DOWN_LEFT},
        {"ld", CURSOR_RESIZE_DOWN_LEFT},
        {"ru", CURSOR_RESIZE_DOWN_LEFT},
        {"resizeDownLeft", CURSOR_RESIZE_DOWN_LEFT},
        {"lurd", CURSOR_RESIZE_DOWN_RIGHT},
        {"rdlu", CURSOR_RESIZE_DOWN_RIGHT},
        {"rd", CURSOR_RESIZE_DOWN_RIGHT},
        {"lu", CURSOR_RESIZE_DOWN_RIGHT},
        {"resizeDownRight", CURSOR_RESIZE_DOWN_RIGHT},
        {"attack", CURSOR_FIGHT},
        {"fight", CURSOR_FIGHT},
        {"take", CURSOR_PICKUP},
        {"pickup", CURSOR_PICKUP},
        {"talk", CURSOR_TALK},
        {"action", CURSOR_ACTION},
        {"left", CURSOR_LEFT},
        {"up", CURSOR_UP},
        {"right", CURSOR_RIGHT},
        {"down", CURSOR_DOWN}
    };

    Cursor stringToCursor(const std::string &name)
    {
        for (size_t f = 0; f < sizeof(hoverCursors) / sizeof(StrToCursor);
             f ++)
        {
            if (hoverCursors[f].str == name)
                return hoverCursors[f].cursor;
        }
        return CURSOR_POINTER;
    }
}  // namespace Cursor
