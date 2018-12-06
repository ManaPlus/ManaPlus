/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef ENUMS_BEING_ATTACKTYPE_H
#define ENUMS_BEING_ATTACKTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(AttackType)
{
    HIT           = 0,
    PICKUP        = 1,
    SIT           = 2,
    STAND         = 3,
    REFLECT       = 4,
    SPLASH        = 5,
    SKILL         = 6,
    REPEATE       = 7,
    MULTI         = 8,
    MULTI_REFLECT = 9,
    CRITICAL      = 10,
    FLEE          = 11,
    TOUCH_SKILL   = 12,
    MISS          = 0xffff,  // pseudo value for miss attacks
    SKILLMISS     = 0x10000  // pseudo value for skill miss attacks
}
enumEnd(AttackType);

#endif  // ENUMS_BEING_ATTACKTYPE_H
