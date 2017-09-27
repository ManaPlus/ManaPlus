/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "enums/being/attributesstrings.h"

#include "enums/simpletypes/stringdefines.h"

#include <map>

#include "debug.h"

namespace AttributesEnum
{
    stringEnumStart(AttributesT)
        // player attributes
        strEnumDef2(Attributes, PLAYER_BASE_LEVEL, "PLAYER_LEVEL")
        strEnumDef(Attributes, PLAYER_HP)
        strEnumDef(Attributes, PLAYER_MAX_HP)
        strEnumDef(Attributes, PLAYER_MP)
        strEnumDef(Attributes, PLAYER_MAX_MP)
        strEnumDef(Attributes, PLAYER_EXP)
        strEnumDef(Attributes, PLAYER_EXP_NEEDED)
        strEnumDef(Attributes, MONEY)
        strEnumDef(Attributes, TOTAL_WEIGHT)
        strEnumDef(Attributes, MAX_WEIGHT)
        strEnumDef(Attributes, PLAYER_JOB)
        strEnumDef(Attributes, PLAYER_JOB_EXP)
        strEnumDef(Attributes, PLAYER_JOB_EXP_NEEDED)
        strEnumDef(Attributes, PLAYER_STR)
        strEnumDef(Attributes, PLAYER_AGI)
        strEnumDef(Attributes, PLAYER_VIT)
        strEnumDef(Attributes, PLAYER_INT)
        strEnumDef(Attributes, PLAYER_DEX)
        strEnumDef(Attributes, PLAYER_LUK)
        strEnumDef(Attributes, PLAYER_SKILL_POINTS)
        strEnumDef(Attributes, PLAYER_CHAR_POINTS)
        strEnumDef(Attributes, PLAYER_CORR_POINTS)
        strEnumDef(Attributes, PLAYER_ATTACK_DELAY)
        strEnumDef(Attributes, PLAYER_ATTACK_RANGE)
        strEnumDef(Attributes, PLAYER_WALK_SPEED)
        strEnumDef(Attributes, PLAYER_ATTACK_SPEED)
        strEnumDef(Attributes, PLAYER_KARMA)
        strEnumDef(Attributes, PLAYER_MANNER)
        strEnumDef(Attributes, PLAYER_CRIT)
        strEnumDef(Attributes, PLAYER_FLEE)
        strEnumDef(Attributes, PLAYER_HIT)
        strEnumDef(Attributes, PLAYER_MDEF)
        strEnumDef(Attributes, PLAYER_MATK)
        strEnumDef(Attributes, PLAYER_DEF)
        strEnumDef(Attributes, PLAYER_ATK)
        strEnumDef(Attributes, CART_TOTAL_WEIGHT)
        strEnumDef(Attributes, CART_MAX_WEIGHT)

        // homunculus attributes
        strEnumDef(Attributes, HOMUN_LEVEL)
        strEnumDef(Attributes, HOMUN_HP)
        strEnumDef(Attributes, HOMUN_MAX_HP)
        strEnumDef(Attributes, HOMUN_MP)
        strEnumDef(Attributes, HOMUN_MAX_MP)
        strEnumDef(Attributes, HOMUN_EXP)
        strEnumDef(Attributes, HOMUN_EXP_NEEDED)
        strEnumDef(Attributes, HOMUN_SKILL_POINTS)
        strEnumDef(Attributes, HOMUN_ATTACK_DELAY)
        strEnumDef(Attributes, HOMUN_ATTACK_RANGE)
        strEnumDef(Attributes, HOMUN_ATTACK_SPEED)
        strEnumDef(Attributes, HOMUN_CRIT)
        strEnumDef(Attributes, HOMUN_FLEE)
        strEnumDef(Attributes, HOMUN_HIT)
        strEnumDef(Attributes, HOMUN_MDEF)
        strEnumDef(Attributes, HOMUN_MATK)
        strEnumDef(Attributes, HOMUN_DEF)
        strEnumDef(Attributes, HOMUN_ATK)

        // mercenary attributes
        strEnumDef(Attributes, MERC_LEVEL)
        strEnumDef(Attributes, MERC_HP)
        strEnumDef(Attributes, MERC_MAX_HP)
        strEnumDef(Attributes, MERC_MP)
        strEnumDef(Attributes, MERC_MAX_MP)
        strEnumDef(Attributes, MERC_ATTACK_DELAY)
        strEnumDef(Attributes, MERC_ATTACK_RANGE)
        strEnumDef(Attributes, MERC_ATTACK_SPEED)
        strEnumDef(Attributes, MERC_CRIT)
        strEnumDef(Attributes, MERC_FLEE)
        strEnumDef(Attributes, MERC_HIT)
        strEnumDef(Attributes, MERC_MDEF)
        strEnumDef(Attributes, MERC_MATK)
        strEnumDef(Attributes, MERC_DEF)
        strEnumDef(Attributes, MERC_ATK)
        strEnumDef(Attributes, MERC_EXPIRE)
        strEnumDef(Attributes, MERC_FAITH)
        strEnumDef(Attributes, MERC_CALLS)
        strEnumDef(Attributes, MERC_KILLS)

        // elemental attributes
        strEnumDef(Attributes, ELEMENTAL_HP)
        strEnumDef(Attributes, ELEMENTAL_MAX_HP)
        strEnumDef(Attributes, ELEMENTAL_MP)
        strEnumDef(Attributes, ELEMENTAL_MAX_MP)
    stringEnumEnd
}  // namespace AttributesEnum
