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

#ifndef NET_EA_INVENTORYITEM_H
#define NET_EA_INVENTORYITEM_H

#include "const/resources/item/cards.h"

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/damaged.h"
#include "enums/simpletypes/equipm.h"
#include "enums/simpletypes/favorite.h"
#include "enums/simpletypes/identified.h"
#include "enums/simpletypes/itemcolor.h"

#include "resources/item/itemoptionslist.h"

#include <vector>

#include "localconsts.h"

namespace Ea
{

/**
 * Used to cache storage data until we get size data for it.
 */
class InventoryItem final
{
    public:
        int slot;
        int id;
        ItemTypeT type;
        int cards[maxCards];
        ItemOptionsList *options;
        int quantity;
        uint8_t refine;
        ItemColor color;
        Identified identified;
        Damaged damaged;
        Favorite favorite;
        Equipm equip;

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
                      Equipm equip0) :
            slot(slot0),
            id(id0),
            type(type0),
            cards(),
            options(ItemOptionsList::copy(options0)),
            quantity(quantity0),
            refine(refine0),
            color(color0),
            identified(identified0),
            damaged(damaged0),
            favorite(favorite0),
            equip(equip0)
        {
            if (!cards0)
                return;
            for (int f = 0; f < 4; f ++)
                cards[f] = cards0[f];
        }

        InventoryItem(const InventoryItem &c) :
            slot(c.slot),
            id(c.id),
            type(c.type),
            cards(),
            options(ItemOptionsList::copy(c.options)),
            quantity(c.quantity),
            refine(c.refine),
            color(c.color),
            identified(c.identified),
            damaged(c.damaged),
            favorite(c.favorite),
            equip(c.equip)
        {
            if (!c.cards)
                return;
            for (int f = 0; f < 4; f ++)
                cards[f] = c.cards[f];
        }

        A_DEFAULT_COPY(InventoryItem)

        ~InventoryItem()
        {
            delete options;
        }
};

typedef std::vector<InventoryItem> InventoryItems;

}  // namespace Ea

#endif  // NET_EA_INVENTORYITEM_H
