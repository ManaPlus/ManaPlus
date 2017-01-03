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

#include "beingequipbackend.h"

#include "being/being.h"

#include "net/inventoryhandler.h"

#include "resources/item/item.h"

#include "utils/delete2.h"

#include "debug.h"

BeingEquipBackend::BeingEquipBackend(Being *const being)
{
    memset(mEquipment, 0, sizeof(mEquipment));
    if (being)
    {
        const size_t sz = being->mSprites.size();

        for (size_t f = 0; f < sz; f ++)
        {
            const int idx = inventoryHandler->
                convertFromServerSlot(CAST_S32(f));
            const BeingSlot &slot = being->mSlots[f];
            const int id = slot.spriteId;
            if (id > 0 && idx >= 0 && idx < EQUIPMENT_SIZE)
            {
                Item *const item = new Item(id,
                    ItemType::Unknown,
                    1,
                    0,
                    slot.colorId,
                    Identified_true,
                    Damaged_false,
                    Favorite_false,
                    Equipm_true,
                    Equipped_true);
                const CardsList &cards = slot.cardsId;
                if (!cards.isEmpty())
                    item->setCards(&cards.cards[0], maxCards);
                mEquipment[idx] = item;
            }
        }
    }
}

BeingEquipBackend::~BeingEquipBackend()
{
    for (int i = 0; i < EQUIPMENT_SIZE; i++)
        delete2(mEquipment[i])
}

void BeingEquipBackend::clear()
{
    for (int i = 0; i < EQUIPMENT_SIZE; i++)
        delete2(mEquipment[i])
}

void BeingEquipBackend::setEquipment(const int index,
                                     const Item *const item)
{
    mEquipment[index] = item;
}

const Item *BeingEquipBackend::getEquipment(const int index) const
{
    if (index < 0 || index >= EQUIPMENT_SIZE)
        return nullptr;
    return mEquipment[index];
}
