/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef INVENTORY_H
#define INVENTORY_H

#include "being/being.h"

#include <list>
#include <string>

class Inventory;
class Item;

class InventoryListener
{
    public:
        virtual ~InventoryListener()
        { }

        virtual void slotsChanged(Inventory *const inventory) = 0;

    protected:
        InventoryListener()
        { }
};

class Inventory final
{
    public:
        A_DELETE_COPY(Inventory)

        static const int NO_SLOT_INDEX = -1; /**< Slot has no index. */

        enum
        {
            INVENTORY = 0,
            STORAGE,
            CART,
            TRADE,
            NPC,
            TYPE_END
        };


        /**
         * Constructor.
         *
         * @param size the number of items that fit in the inventory
         */
        explicit Inventory(const int type, const int size = -1);

        /**
         * Destructor.
         */
        ~Inventory();

        /**
         * Returns the size that this instance is configured for.
         */
        unsigned getSize() const A_WARN_UNUSED
        { return mSize; }

        /**
         * Returns the item at the specified index.
         */
        Item *getItem(const int index) const A_WARN_UNUSED;

        /**
         * Searches for the specified item by it's id.
         *
         * @param itemId The id of the item to be searched.
         * @param color The color of the item to be searched.
         * @return Item found on success, NULL on failure.
         */
        Item *findItem(const int itemId,
                       const unsigned char color) const A_WARN_UNUSED;

        /**
         * Adds a new item in a free slot.
         */
        void addItem(const int id, const int quantity, const int refine,
                     const unsigned char color, const bool equipment = false);

        /**
         * Sets the item at the given position.
         */
        void setItem(const int index, const int id, const int quantity,
                     const int refine, const unsigned char color,
                     const bool equipment = false);

        /**
         * Remove a item from the inventory.
         */
        void removeItem(const int id);

        /**
         * Remove the item at the specified index from the inventory.
         */
        void removeItemAt(const int index);

        /**
         * Checks if the given item is in the inventory.
         */
        bool contains(const Item *const item) const A_WARN_UNUSED;

        /**
         * Returns id of next free slot or -1 if all occupied.
         */
        int getFreeSlot() const A_WARN_UNUSED;

        /**
         * Reset all item slots.
         */
        void clear();

        /**
         * Get the number of slots filled with an item
         */
        int getNumberOfSlotsUsed() const A_WARN_UNUSED
        { return mUsed; }

        /**
         * Returns the index of the last occupied slot or 0 if none occupied.
         */
        int getLastUsedSlot() const A_WARN_UNUSED;

        void addInventoyListener(InventoryListener *const listener);

        void removeInventoyListener(InventoryListener *const listener);

        int getType() const A_WARN_UNUSED
        { return mType; }

        bool isMainInventory() const A_WARN_UNUSED
        { return mType == INVENTORY; }

        const Item *findItemBySprite(std::string spritePath,
                                     const Gender gender,
                                     const int race) const A_WARN_UNUSED;

        std::string getName() const A_WARN_UNUSED;

        void resize(const unsigned int newSize);

    protected:
        typedef std::list<InventoryListener*> InventoryListenerList;
        InventoryListenerList mInventoryListeners;

        void distributeSlotsChangedEvent();

        int mType;
        unsigned mSize; /**< The max number of inventory items */
        Item **mItems;  /**< The holder of items */
        int mUsed;      /**< THe number of slots in use */
};

#endif  // INVENTORY_H
