/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_MAP_COLLISIONTYPE_H
#define ENUMS_RESOURCES_MAP_COLLISIONTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(CollisionType)
{
    COLLISION_EMPTY        = 0,  // no collision
    COLLISION_WALL         = 1,  // full collison
    COLLISION_AIR          = 2,  // air units can walk
    COLLISION_WATER        = 3,  // water units can walk
    COLLISION_GROUNDTOP    = 4,  // no collision (chair, bed, etc)
    COLLISION_PLAYER_WALL  = 5,  // full collision for player
    COLLISION_MONSTER_WALL = 6,  // full collision for monster
    COLLISION_MAX          = 7   // count index
}
enumEnd(CollisionType);

#endif  // ENUMS_RESOURCES_MAP_COLLISIONTYPE_H
