/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2017  The ManaPlus Developers
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

#include "itemcolormanager.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "resources/item/cardslist.h"

#include "debug.h"

ItemColor ItemColorManager::getColorFromCards(const int *const cards)
{
    if (!cards)
        return ItemColor_one;
    if (cards[0] == CARD0_FORGE ||
        cards[0] == CARD0_CREATE ||
        cards[0] == CARD0_PET)
    {
        return ItemColor_one;
    }
    for (int f = 0; f < maxCards; f ++)
    {
        const int id = cards[f];
        if (id == 0)
            continue;
        const ItemInfo &info = ItemDB::get(id);
        const ItemColor &color = info.getCardColor();
        if (color != ItemColor_zero)
            return color;
    }
    return ItemColor_one;
}

ItemColor ItemColorManager::getColorFromCards(const CardsList &cards)
{
    return getColorFromCards(&cards.cards[0]);
}
