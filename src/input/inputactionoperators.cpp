/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2018  The ManaPlus Developers
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

#include "input/inputactionoperators.h"

#include "utils/cast.h"

#include "debug.h"

InputActionT operator+(InputActionT action, const int& i)
{
    action = static_cast<InputActionT>(CAST_S32(action) + i);
    return action;
}

InputActionT operator+(InputActionT action, const unsigned int& i)
{
    action = static_cast<InputActionT>(CAST_U32(action) + i);
    return action;
}

int operator-(const InputActionT &action1, const InputActionT &action2)
{
    return CAST_S32(action1) - CAST_S32(action2);
}
