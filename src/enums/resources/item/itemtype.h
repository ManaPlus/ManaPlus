/*
 *  The ManaPlus Client
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

#ifndef ENUMS_RESOURCES_ITEM_ITEMTYPE_H
#define ENUMS_RESOURCES_ITEM_ITEMTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(ItemType)
{
    Healing  = 0,
    Unknown  = 1,
    Usable   = 2,
    Etc      = 3,
    Weapon   = 4,
    Armor    = 5,
    Card     = 6,
    PetEgg   = 7,
    PetArmor = 8,
    Unknown2 = 9,
    Ammon    = 10,
    DelayConsume = 11,
    Cash     = 18
}
enumEnd(ItemType);

#endif  // ENUMS_RESOURCES_ITEM_ITEMTYPE_H
