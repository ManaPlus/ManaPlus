/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef BEING_ATTRIBUTES_H
#define BEING_ATTRIBUTES_H

namespace Attributes
{
    /**
     * Standard attributes for players.
     */
    enum Attributes
    {
        LEVEL = 0,
        HP,
        MAX_HP,
        MP,
        MAX_MP,
        EXP,
        EXP_NEEDED,
        MONEY,
        TOTAL_WEIGHT,
        MAX_WEIGHT,
        SKILL_POINTS,
        CHAR_POINTS,
        CORR_POINTS,
        LUK = 18,
        ATTACK_DELAY = 100,
        ATTACK_RANGE = 101,
        WALK_SPEED = 102,
        ATTACK_SPEED = 103,
        KARMA,
        MANNER,
        CRIT,
        FLEE,
        HIT,
        MDEF,
        MATK,
        DEF,
        ATK
    };
}  // namespace Attributes

#endif  // BEING_ATTRIBUTES_H
