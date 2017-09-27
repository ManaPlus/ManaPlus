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

#ifndef ENUMS_BEING_ATTRIBUTES_H
#define ENUMS_BEING_ATTRIBUTES_H

#include "enums/simpletypes/enumdefines.h"

enumStart(Attributes)
{
    // player attributes
    PLAYER_BASE_LEVEL   = 0,
    PLAYER_HP,
    PLAYER_MAX_HP,
    PLAYER_MP,
    PLAYER_MAX_MP,
    PLAYER_EXP,
    PLAYER_EXP_NEEDED,
    MONEY,
    TOTAL_WEIGHT,
    MAX_WEIGHT,
    PLAYER_JOB          = 10,
    PLAYER_STR          = 13,
    PLAYER_AGI          = 14,
    PLAYER_VIT          = 15,
    PLAYER_INT          = 16,
    PLAYER_DEX          = 17,
    PLAYER_LUK          = 18,
    PLAYER_SKILL_POINTS,
    PLAYER_CHAR_POINTS,
    PLAYER_CORR_POINTS,
    PLAYER_ATTACK_DELAY = 100,
    PLAYER_ATTACK_RANGE = 101,
    PLAYER_WALK_SPEED   = 102,
    PLAYER_ATTACK_SPEED = 103,
    PLAYER_KARMA,
    PLAYER_MANNER,
    PLAYER_CRIT,
    PLAYER_FLEE,
    PLAYER_HIT,
    PLAYER_MDEF,
    PLAYER_MATK,
    PLAYER_DEF,
    PLAYER_ATK,
    CART_TOTAL_WEIGHT = 1000,
    CART_MAX_WEIGHT,
    PLAYER_JOB_EXP,
    PLAYER_JOB_EXP_NEEDED,

    // homunculus attributes
    HOMUN_LEVEL,
    HOMUN_HP,
    HOMUN_MAX_HP,
    HOMUN_MP,
    HOMUN_MAX_MP,
    HOMUN_EXP,
    HOMUN_EXP_NEEDED,
    HOMUN_SKILL_POINTS,
    HOMUN_ATTACK_DELAY,
    HOMUN_ATTACK_RANGE,
    HOMUN_ATTACK_SPEED,
    HOMUN_CRIT,
    HOMUN_FLEE,
    HOMUN_HIT,
    HOMUN_MDEF,
    HOMUN_MATK,
    HOMUN_DEF,
    HOMUN_ATK,

    // mercenary attributes
    MERC_LEVEL,
    MERC_HP,
    MERC_MAX_HP,
    MERC_MP,
    MERC_MAX_MP,
    MERC_ATTACK_DELAY,
    MERC_ATTACK_RANGE,
    MERC_ATTACK_SPEED,
    MERC_CRIT,
    MERC_FLEE,
    MERC_HIT,
    MERC_MDEF,
    MERC_MATK,
    MERC_DEF,
    MERC_ATK,
    MERC_EXPIRE,
    MERC_FAITH,
    MERC_CALLS,
    MERC_KILLS,

    // elemental attributes
    ELEMENTAL_HP,
    ELEMENTAL_MAX_HP,
    ELEMENTAL_MP,
    ELEMENTAL_MAX_MP,

    MAX_ATTRIBUTE
}
enumEnd(Attributes);

#endif  // ENUMS_BEING_ATTRIBUTES_H
