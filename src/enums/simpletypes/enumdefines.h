/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2017  The ManaPlus Developers
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

#ifndef ENUMS_SIMPLETYPES_ENUMDEFINES_H
#define ENUMS_SIMPLETYPES_ENUMDEFINES_H

#include "localconsts.h"

#ifdef ADVGCC

#define enumStart(name) enum class name
#define enumStartT(name, type) enum class name : type
#define enumEnd(name) ; \
    typedef name name##T

#else  // ADVGCC

#define enumStart(name) \
    namespace name \
    { \
        enum T

#define enumStartT(name, type) \
    namespace name \
    { \
        enum T

#define enumEnd(name) \
        ;\
    } \
    typedef name::T name##T

#endif  // ADVGCC

#define enum2Start(name) \
    namespace name \
    { \
        enum T

#define enum2End(name) \
        ;\
    } \
    typedef name::T name##T

#endif  // ENUMS_SIMPLETYPES_ENUMDEFINES_H
