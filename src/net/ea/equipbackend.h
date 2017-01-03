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

#ifndef NET_EA_EQUIPBACKEND_H
#define NET_EA_EQUIPBACKEND_H

#include "resources/item/item.h"

#include "being/playerinfo.h"

#include "const/equipment.h"

#include "gui/windows/inventorywindow.h"

namespace Ea
{

class EquipBackend final : public Equipment::Backend
{
    public:
        EquipBackend()
        {
            memset(mEquipment, -1, sizeof(mEquipment));
        }

        A_DELETE_COPY(EquipBackend)

        Item *getEquipment(const int index) const override final A_WARN_UNUSED
        {
            int invyIndex = mEquipment[index];
            if (invyIndex == -1)
                return nullptr;

            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
                return inv->getItem(invyIndex);
            else
                return nullptr;
        }

        void clear() override final
        {
            Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;
            for (int i = 0; i < EQUIPMENT_SIZE; i++)
            {
                if (mEquipment[i] != -1)
                {
                    Item* item = inv->getItem(i);
                    if (item)
                        item->setEquipped(Equipped_false);
                }

                mEquipment[i] = -1;
            }
        }

        void setEquipment(const int index, const int inventoryIndex)
        {
            Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            if (index < 0 || index >= EQUIPMENT_SIZE)
                return;

            // Unequip existing item
            Item *item = inv->getItem(mEquipment[index]);

            if (item)
                item->setEquipped(Equipped_false);

            // not checking index because it must be safe
            mEquipment[index] = inventoryIndex;

            item = inv->getItem(inventoryIndex);
            if (item)
                item->setEquipped(Equipped_true);

            if (inventoryWindow)
                inventoryWindow->updateButtons();
        }

    private:
        int mEquipment[EQUIPMENT_SIZE];
};

}  // namespace Ea

#endif  // NET_EA_EQUIPBACKEND_H
