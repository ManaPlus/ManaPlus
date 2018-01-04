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

#ifndef ENUMS_SIMPLETYPES_INTDEFINES_H
#define ENUMS_SIMPLETYPES_INTDEFINES_H

#include "localconsts.h"

#ifdef ADVGCC

#define defIntEnum(name, type) \
    enum class name : type \
    { \
    }; \
    const name name##_zero = static_cast<name>(0); \
    const name name##_one = static_cast<name>(1)

#define fromInt(val, name) static_cast<name>(val)
#define toInt(val, name) static_cast<name>(val)
#define defIntEnumNeg(name) const name name##_negOne = static_cast<name>(-1)

#else  // ADVGCC

#define defIntEnum(name, type) \
    typedef type name; \
    const name name##_zero = 0; \
    const name name##_one = 1
#define fromInt(val, name) static_cast<name>(val)
#define toInt(val, name) (val)
#define defIntEnumNeg(name) const name name##_negOne = -1

#endif  // ADVGCC

#endif  // ENUMS_SIMPLETYPES_INTDEFINES_H
