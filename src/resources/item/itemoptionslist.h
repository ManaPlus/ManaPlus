/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_ITEM_ITEMOPTIONSLIST_H
#define RESOURCES_ITEM_ITEMOPTIONSLIST_H

#include "const/resources/item/itemoptions.h"

#include "resources/item/itemoption.h"

#include "localconsts.h"

struct ItemOptionsList final
{
    explicit ItemOptionsList(const size_t amount0) :
        options(nullptr),
        amount(amount0),
        pointer(0U)
    {
        options = new ItemOption[amount];
    }

    ItemOptionsList() :
        options(nullptr),
        amount(maxItemOptions),
        pointer(0U)
    {
        options = new ItemOption[amount];
    }

    A_DELETE_COPY(ItemOptionsList)

    ~ItemOptionsList()
    {
        delete [] options;
        options = nullptr;
    }

    void add(const uint16_t index,
             const uint16_t value)
    {
        if (pointer >= amount)
            return;
        options[pointer].index = index;
        options[pointer].value = value;
        pointer ++;
    }

    static ItemOptionsList *copy(const ItemOptionsList *const options0)
    {
        if (options0 == nullptr)
            return nullptr;

        const size_t amount0 = options0->amount;
        ItemOptionsList *const obj = new ItemOptionsList(amount0);
        for (size_t f = 0; f < amount0; f ++)
        {
            obj->options[f].index = options0->options[f].index;
            obj->options[f].value = options0->options[f].value;
        }
        obj->amount = options0->amount;
        obj->pointer = options0->pointer;
        return obj;
    }

    size_t size() const
    {
        if (pointer < amount)
            return pointer;
        return amount;
    }

    const ItemOption &get(const size_t index) const
    {
        return options[index];
    }

    ItemOption *options;
    size_t amount;
    size_t pointer;
};

#endif  // RESOURCES_ITEM_ITEMOPTIONSLIST_H
