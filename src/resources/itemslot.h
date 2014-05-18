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

#ifndef RESOURCES_ITEMSLOT_H
#define RESOURCES_ITEMSLOT_H

namespace ItemSlot
{
    enum Type
    {
        // Equipment rules:
        // 1 Brest equipment
        TORSO_SLOT = 0,
        // 1 arms equipment
        ARMS_SLOT = 1,
        // 1 head equipment
        HEAD_SLOT = 2,
        // 1 legs equipment
        LEGS_SLOT = 3,
        // 1 feet equipment
        FEET_SLOT = 4,
        // 2 rings
        RING1_SLOT = 5,
        RING2_SLOT = 6,
        // 1 necklace
        NECKLACE_SLOT = 7,
        // Fight:
        //   2 one-handed weapons
        //   or 1 two-handed weapon
        //   or 1 one-handed weapon + 1 shield.
        FIGHT1_SLOT = 8,
        FIGHT2_SLOT = 9,
        // Projectile:
        //   this item does not amount to one, it only
        //   indicates the chosen projectile.
        PROJECTILE_SLOT = 10
    };
}

#endif  // RESOURCES_ITEMSLOT_H
