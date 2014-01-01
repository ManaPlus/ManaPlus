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

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

static const int EQUIPMENT_SIZE = 13;

#include "localconsts.h"

class Item;

class Equipment final
{
    public:
        /**
         * Constructor.
         */
        Equipment(): mBackend(nullptr)
        { }

        A_DELETE_COPY(Equipment)

        /**
         * Destructor.
         */
        ~Equipment()
        { mBackend = nullptr; }

        enum Slot
        {
            EQUIP_TORSO_SLOT = 0,
            EQUIP_GLOVES_SLOT = 1,
            EQUIP_HEAD_SLOT = 2,
            EQUIP_LEGS_SLOT = 3,
            EQUIP_FEET_SLOT = 4,
            EQUIP_RING1_SLOT = 5,
            EQUIP_RING2_SLOT = 6,
            EQUIP_NECK_SLOT = 7,
            EQUIP_FIGHT1_SLOT = 8,
            EQUIP_FIGHT2_SLOT = 9,
            EQUIP_PROJECTILE_SLOT = 10,
            EQUIP_EVOL_RING1_SLOT = 11,
            EQUIP_EVOL_RING2_SLOT = 12,
            EQUIP_VECTOREND
        };

        class Backend
        {
            public:
                virtual Item *getEquipment(const int index)
                                           const A_WARN_UNUSED = 0;

                virtual void clear() = 0;

                virtual ~Backend()
                { }
        };

        /**
         * Get equipment at the given slot.
         */
        Item *getEquipment(const int index) const A_WARN_UNUSED
        { return mBackend ? mBackend->getEquipment(index) : nullptr; }

        /**
         * Clears equipment.
         */
        void clear()
        { if (mBackend) mBackend->clear(); }

        /**
         * Set equipment at the given slot.
         */
        void setEquipment(int index, int id, int quantity = 0);

        void setBackend(Backend *const backend)
        { mBackend = backend; }

        const Backend *getBackend() const A_WARN_UNUSED
        { return mBackend; }

    private:
        Backend *mBackend;
};

#endif  // EQUIPMENT_H
