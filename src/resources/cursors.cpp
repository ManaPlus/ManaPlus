/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/cursors.h"

#include "debug.h"

namespace Cursors
{
    static const StrToCursor hoverCursors[] =
    {
        {"select", Cursor::CURSOR_POINTER},
        {"pointer", Cursor::CURSOR_POINTER},
        {"lr", Cursor::CURSOR_RESIZE_ACROSS},
        {"rl", Cursor::CURSOR_RESIZE_ACROSS},
        {"resizeAcross", Cursor::CURSOR_RESIZE_ACROSS},
        {"ud", Cursor::CURSOR_RESIZE_DOWN},
        {"du", Cursor::CURSOR_RESIZE_DOWN},
        {"resizeDown", Cursor::CURSOR_RESIZE_DOWN},
        {"ldru", Cursor::CURSOR_RESIZE_DOWN_LEFT},
        {"ruld", Cursor::CURSOR_RESIZE_DOWN_LEFT},
        {"ld", Cursor::CURSOR_RESIZE_DOWN_LEFT},
        {"ru", Cursor::CURSOR_RESIZE_DOWN_LEFT},
        {"resizeDownLeft", Cursor::CURSOR_RESIZE_DOWN_LEFT},
        {"lurd", Cursor::CURSOR_RESIZE_DOWN_RIGHT},
        {"rdlu", Cursor::CURSOR_RESIZE_DOWN_RIGHT},
        {"rd", Cursor::CURSOR_RESIZE_DOWN_RIGHT},
        {"lu", Cursor::CURSOR_RESIZE_DOWN_RIGHT},
        {"resizeDownRight", Cursor::CURSOR_RESIZE_DOWN_RIGHT},
        {"attack", Cursor::CURSOR_FIGHT},
        {"fight", Cursor::CURSOR_FIGHT},
        {"take", Cursor::CURSOR_PICKUP},
        {"pickup", Cursor::CURSOR_PICKUP},
        {"talk", Cursor::CURSOR_TALK},
        {"action", Cursor::CURSOR_ACTION},
        {"left", Cursor::CURSOR_LEFT},
        {"up", Cursor::CURSOR_UP},
        {"right", Cursor::CURSOR_RIGHT},
        {"down", Cursor::CURSOR_DOWN}
    };

    CursorT stringToCursor(const std::string &name)
    {
        for (size_t f = 0; f < sizeof(hoverCursors) / sizeof(StrToCursor);
             f ++)
        {
            if (hoverCursors[f].str == name)
                return hoverCursors[f].cursor;
        }
        return Cursor::CURSOR_POINTER;
    }
}  // namespace Cursors
