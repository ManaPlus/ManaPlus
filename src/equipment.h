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

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "localconsts.h"

class Item;

class Equipment final
{
    public:
        /**
         * Constructor.
         */
        Equipment() :
            mBackend(nullptr)
        { }

        A_DELETE_COPY(Equipment)

        /**
         * Destructor.
         */
        ~Equipment()
        { mBackend = nullptr; }

        class Backend notfinal
        {
            public:
                Backend()
                { }

                A_DELETE_COPY(Backend)

                virtual const Item *getEquipment(const int index)
                                                 const A_WARN_UNUSED = 0;

                virtual void clear() = 0;

                virtual ~Backend()
                { }
        };

        /**
         * Get equipment at the given slot.
         */
        const Item *getEquipment(const int index) const A_WARN_UNUSED
        {
            return mBackend != nullptr ?
                mBackend->getEquipment(index) : nullptr;
        }

        /**
         * Clears equipment.
         */
        void clear()
        { if (mBackend != nullptr) mBackend->clear(); }

        /**
         * Set equipment at the given slot.
         */
        void setEquipment(int index, int id, int quantity = 0);

        void setBackend(Backend *const backend)
        { mBackend = backend; }

        const Backend *getBackend() const noexcept2 A_WARN_UNUSED
        { return mBackend; }

    private:
        Backend *mBackend;
};

#endif  // EQUIPMENT_H
