/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_EA_INVENTORYITEM_H
#define NET_EA_INVENTORYITEM_H

#include "const/resources/item/cards.h"

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/damaged.h"
#include "enums/simpletypes/equipm.h"
#include "enums/simpletypes/favorite.h"
#include "enums/simpletypes/identified.h"
#include "enums/simpletypes/itemcolor.h"

#include "utils/vector.h"

#include "resources/item/itemoptionslist.h"

#include "localconsts.h"

namespace Ea
{

/**
 * Used to cache storage data until we get size data for it.
 */
class InventoryItem final
{
    public:
        int cards[maxCards];
        ItemOptionsList *options;
        int slot;
        int id;
        int quantity;
        int equipIndex;
        ItemTypeT type;
        ItemColor color;
        Identified identified;
        Damaged damaged;
        Favorite favorite;
        Equipm equip;
        uint8_t refine;

        InventoryItem(const int slot0,
                      const int id0,
                      const ItemTypeT type0,
                      const int *const cards0,
                      ItemOptionsList *options0,
                      const int quantity0,
                      const uint8_t refine0,
                      const ItemColor color0,
                      const Identified identified0,
                      const Damaged damaged0,
                      const Favorite favorite0,
                      Equipm equip0,
                      int equipIndex0) :
            cards(),
            options(ItemOptionsList::copy(options0)),
            slot(slot0),
            id(id0),
            quantity(quantity0),
            equipIndex(equipIndex0),
            type(type0),
            color(color0),
            identified(identified0),
            damaged(damaged0),
            favorite(favorite0),
            equip(equip0),
            refine(refine0)
        {
            if (cards0 == nullptr)
                return;
            for (int f = 0; f < 4; f ++)
                cards[f] = cards0[f];
        }

        InventoryItem(const InventoryItem &c) :
            cards(),
            options(ItemOptionsList::copy(c.options)),
            slot(c.slot),
            id(c.id),
            quantity(c.quantity),
            equipIndex(c.equipIndex),
            type(c.type),
            color(c.color),
            identified(c.identified),
            damaged(c.damaged),
            favorite(c.favorite),
            equip(c.equip),
            refine(c.refine)
        {
            for (int f = 0; f < 4; f ++)
                cards[f] = c.cards[f];
        }

        A_DEFAULT_COPY(InventoryItem)

        ~InventoryItem()
        {
            delete options;
        }
};

typedef STD_VECTOR<InventoryItem> InventoryItems;

}  // namespace Ea

#endif  // NET_EA_INVENTORYITEM_H
