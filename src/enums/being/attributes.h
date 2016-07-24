/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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
    PLAYER_LEVEL        = 0,
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
    MAX_ATTRIBUTE
}
enumEnd(Attributes);

#endif  // ENUMS_BEING_ATTRIBUTES_H
