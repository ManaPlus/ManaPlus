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

#ifndef ENUMS_INPUT_INPUTTYPE_H
#define ENUMS_INPUT_INPUTTYPE_H

#include "enums/simpletypes/enumdefines.h"

// hack to avoid conflicts with windows headers.
#ifdef KEYBOARD
#undef KEYBOARD
#endif  // KEYBOARD
#ifdef MOUSE
#undef MOUSE
#endif  // MOUSE

enumStart(InputType)
{
    UNKNOWN  = 0,
    KEYBOARD = 1,
    MOUSE    = 2,
    JOYSTICK = 3
}
enumEnd(InputType);

#endif  // ENUMS_INPUT_INPUTTYPE_H
