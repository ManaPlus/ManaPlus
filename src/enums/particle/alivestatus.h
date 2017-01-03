/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef ENUMS_PARTICLE_ALIVESTATUS_H
#define ENUMS_PARTICLE_ALIVESTATUS_H

#include "enums/simpletypes/enumdefines.h"

enumStart(AliveStatus)
{
    ALIVE         = 0,
    DEAD_TIMEOUT  = 1,
    DEAD_FLOOR    = 2,
    DEAD_SKY      = 4,
    DEAD_IMPACT   = 8,
    DEAD_OTHER    = 16,
    DEAD_LONG_AGO = 128
}
enumEnd(AliveStatus);

#endif  // ENUMS_PARTICLE_ALIVESTATUS_H
