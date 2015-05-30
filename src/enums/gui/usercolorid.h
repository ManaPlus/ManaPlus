/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <dougaboffey@netscape.net>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

namespace UserColorId
{
    /** List of all colors that are configurable. */
    enum T
    {
        BEING = 0,
        FRIEND,
        DISREGARDED,
        IGNORED,
        ERASED,
        PC,
        SELF,
        GM,
        NPC,
        MONSTER,
        MONSTER_HP,
        MONSTER_HP2,
        PARTY,
        GUILD,
        PARTICLE,
        PICKUP_INFO,
        EXP_INFO,
        PLAYER_HP,
        PLAYER_HP2,
        HIT_PLAYER_MONSTER,
        HIT_MONSTER_PLAYER,
        HIT_PLAYER_PLAYER,
        HIT_CRITICAL,
        HIT_LOCAL_PLAYER_MONSTER,
        HIT_LOCAL_PLAYER_CRITICAL,
        HIT_LOCAL_PLAYER_MISS,
        MISS,
        PORTAL_HIGHLIGHT,
        COLLISION_HIGHLIGHT,
        AIR_COLLISION_HIGHLIGHT,
        WATER_COLLISION_HIGHLIGHT,
        GROUNDTOP_COLLISION_HIGHLIGHT,
        WALKABLE_HIGHLIGHT,
        ATTACK_RANGE,
        ATTACK_RANGE_BORDER,
        MONSTER_ATTACK_RANGE,
        FLOOR_ITEM_TEXT,
        HOME_PLACE,
        HOME_PLACE_BORDER,
        ROAD_POINT,
        NET,
        PET,
        MERCENARY,
        HOMUNCULUS,
        USER_COLOR_LAST
    };
}  // namespace UserColorId

#endif  // ENUMS_GUI_USERCOLORID_H
