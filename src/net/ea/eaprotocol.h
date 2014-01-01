/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EA_EAPROTOCOL_H
#define NET_EA_EAPROTOCOL_H

enum
{
    EA_JOB = 0xa,

    EA_STR = 0xd,
    EA_AGI,
    EA_VIT,
    EA_INT,
    EA_DEX,
    EA_LUK,

    EA_ATK,
    EA_DEF,
    EA_MATK,
    EA_MDEF,
    EA_HIT,
    EA_FLEE,
    EA_CRIT

//    KARMA,
//    MANNER
};

enum
{
    EA_SPRITE_BASE = 0,
    EA_SPRITE_SHOE,
    EA_SPRITE_BOTTOMCLOTHES,
    EA_SPRITE_TOPCLOTHES,
    EA_SPRITE_MISC1,
    EA_SPRITE_MISC2,
    EA_SPRITE_HAIR,
    EA_SPRITE_HAT,
    EA_SPRITE_CAPE,
    EA_SPRITE_GLOVES,
    EA_SPRITE_WEAPON,
    EA_SPRITE_SHIELD,
    EA_SPRITE_EVOL1,
    EA_SPRITE_EVOL2,
    EA_SPRITE_VECTOREND
};

static const int INVENTORY_OFFSET = 2;
static const int STORAGE_OFFSET = 1;

#endif  // NET_EA_EAPROTOCOL_H
