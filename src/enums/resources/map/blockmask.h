/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef ENUMS_RESOURCES_MAP_BLOCKMASK_H
#define ENUMS_RESOURCES_MAP_BLOCKMASK_H

namespace BlockMask
{
    enum BlockMask
    {
        WALL        = 0x80,  // 1000 0000
        AIR         = 0x04,  // 0000 0100
        WATER       = 0x08,  // 0000 1000
        GROUND      = 0x10,  // 0001 0000
        GROUNDTOP   = 0x20,  // 0010 0000
        PLAYERWALL  = 0x40,  // 0100 0000
        MONSTERWALL = 0x02   // 0000 0010
    };
}  // namespace BlockMask

#endif  // ENUMS_RESOURCES_MAP_BLOCKMASK_H
