/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef ENUMS_GUI_USERCOLORID_H
#define ENUMS_GUI_USERCOLORID_H

#include "enums/simpletypes/enumdefines.h"

enumStart(UserColorId)
{
    NO_COLOR = -1,
    LABEL_BEING = 0,
    BEING,
    FRIEND,
    DISREGARDED,
    IGNORED,
    ERASED,
    ENEMY,
    PC,
    SELF,
    GM,
    NPC,
    MONSTER,
    PET,
    MERCENARY,
    HOMUNCULUS,
    SKILLUNIT,
    PARTY,
    GUILD,
    TEAM1,
    TEAM2,
    TEAM3,
    LABEL_PARTICLES,
    PARTICLE,
    PICKUP_INFO,
    EXP_INFO,
    LABEL_HP,
    PLAYER_HP,
    PLAYER_HP2,
    MONSTER_HP,
    MONSTER_HP2,
    HOMUN_HP,
    HOMUN_HP2,
    MERC_HP,
    MERC_HP2,
    ELEMENTAL_HP,
    ELEMENTAL_HP2,
    LABEL_HITS,
    HIT_PLAYER_MONSTER,
    HIT_MONSTER_PLAYER,
    HIT_PLAYER_PLAYER,
    HIT_CRITICAL,
    HIT_LOCAL_PLAYER_MONSTER,
    HIT_LOCAL_PLAYER_CRITICAL,
    HIT_LOCAL_PLAYER_MISS,
    MISS,
    LABEL_TILES,
    PORTAL_HIGHLIGHT,
    COLLISION_HIGHLIGHT,
    AIR_COLLISION_HIGHLIGHT,
    WATER_COLLISION_HIGHLIGHT,
    MONSTER_COLLISION_HIGHLIGHT,
    GROUNDTOP_COLLISION_HIGHLIGHT,
    WALKABLE_HIGHLIGHT,
    NET,
    LABEL_RANGES,
    ATTACK_RANGE,
    ATTACK_RANGE_BORDER,
    MONSTER_ATTACK_RANGE,
    SKILL_ATTACK_RANGE,
    LABEL_OTHER,
    FLOOR_ITEM_TEXT,
    HOME_PLACE,
    HOME_PLACE_BORDER,
    ROAD_POINT,
    USER_COLOR_LAST
}
enumEnd(UserColorId);

#endif  // ENUMS_GUI_USERCOLORID_H
