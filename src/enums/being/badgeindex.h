/*
 *  The ManaPlus Client
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

#ifndef ENUMS_BEING_BADGEINDEX_H
#define ENUMS_BEING_BADGEINDEX_H

#include "enums/simpletypes/enumdefines.h"

enum2Start(BadgeIndex)
{
    Away     = 0,
    Inactive = 1,
    Team     = 2,
    Shop     = 3,
    Gm       = 4,
    Guild    = 5,
    Party    = 6,
    Name     = 7,
    Lang     = 8,

    BadgeIndexSize
}
enum2End(BadgeIndex);

#endif  // ENUMS_BEING_BADGEINDEX_H
