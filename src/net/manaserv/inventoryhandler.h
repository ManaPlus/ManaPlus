/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_MANASERV_INVENTORYHANDLER_H
#define NET_MANASERV_INVENTORYHANDLER_H

#include "equipment.h"

#include "net/inventoryhandler.h"

#include "net/manaserv/messagehandler.h"

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

namespace ManaServ
{

class EquipBackend : public Equipment::Backend
{
    public:
        EquipBackend()
        { memset(mEquipment, 0, sizeof(mEquipment)); }

        Item *getEquipment(int index) const
        { return mEquipment[index]; }

        void clear()
        {
            for (int i = 0; i < EQUIPMENT_SIZE; ++i)
                delete mEquipment[i];

            std::fill_n(mEquipment, EQUIPMENT_SIZE, (Item*) 0);
        }

        void setEquipment(unsigned int slot, unsigned int used, int reference)
        {
            printf("Equip: %d at %dx%d\n", reference, slot, used);
        }

        void addEquipment(unsigned int slot, int reference)
        {
            printf("Equip: %d at %d\n", reference, slot);
        }

    private:
        Item *mEquipment[EQUIPMENT_SIZE];
};

class InventoryHandler : public MessageHandler, Net::InventoryHandler
{
    public:
        InventoryHandler();

        void handleMessage(Net::MessageIn &msg);

        void equipItem(const Item *item);

        void unequipItem(const Item *item);

        void useItem(const Item *item);

        void dropItem(const Item *item, int amount);

        bool canSplit(const Item *item) const;

        void splitItem(const Item *item, int amount);

        void moveItem(int oldIndex, int newIndex);

        void openStorage(int type);

        void closeStorage(int type);

        void moveItem(int source, int slot, int amount,
                      int destination);

        size_t getSize(int type) const;

        int convertFromServerSlot(int eAthenaSlot) const;

    private:
        EquipBackend mEquips;
};

} // namespace ManaServ

#endif // NET_MANASERV_INVENTORYHANDLER_H
