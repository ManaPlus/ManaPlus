/*
 *  The ManaPlus Client
 *  Copyright (C) 2015  The ManaPlus Developers
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

#include "debug.h"

ItemColor ItemColorManager::getColorFromCards(const int *const cards)
{
    if (!cards)
        return ItemColor_one;
    for (int f = 0; f < 4; f ++)
    {
        const ItemInfo &info = ItemDB::get(cards[f]);
        const ItemColor &color = info.getCardColor();
        if (color != ItemColor_zero)
            return color;
    }
    return ItemColor_one;
}
