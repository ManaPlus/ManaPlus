/*
 *  The ManaPlus Client
 *  Copyright (C) 2015  The ManaPlus Developers
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

#ifndef ENUMS_SIMPLETYPES_H
#define ENUMS_SIMPLETYPES_H

#include "localconsts.h"

#ifdef ADVGCC

#define defBoolEnum(name) \
    enum class name : bool \
    { \
        False = false, \
        True = true \
    }; \
    const name name##_true = name::True; \
    const name name##_false = name::False
#define fromBool(val, name) \
    (val) ? name::True : name::False

#else  // ADVGCC

#define defBoolEnum(name) \
    const bool name##_true = true; \
    const bool name##_false = false; \
    typedef bool name
#define fromBool(val, name) \
    (val) ? true : false

#endif  // ADVGCC

defBoolEnum(Identified);
defBoolEnum(Damaged);
defBoolEnum(Favorite);
defBoolEnum(Equipm);
defBoolEnum(Equipped);
defBoolEnum(Modal);
defBoolEnum(ShowCenter);
defBoolEnum(Notify);
defBoolEnum(Trading);
defBoolEnum(Sfx);
defBoolEnum(Keep);
defBoolEnum(Modifiable);
defBoolEnum(AllPlayers);
defBoolEnum(AllowSort);

#endif  // ENUMS_SIMPLETYPES_H
