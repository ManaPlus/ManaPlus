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

#ifndef INPUT_INPUTITEM_H
#define INPUT_INPUTITEM_H

#include "enums/input/inputtype.h"

#include "localconsts.h"

struct InputItem final
{
    InputItem() :
        type(InputType::UNKNOWN),
        value(-1)
    { }

    InputItem(const InputTypeT type0,
              const int value0) :
        type(type0),
        value(value0)
    { }

    A_DEFAULT_COPY(InputItem)

    InputTypeT type;
    int value;
};

#endif  // INPUT_INPUTITEM_H
