/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#ifndef ITEMSHORTCUT_H
#define ITEMSHORTCUT_H

#include "localconsts.h"

const unsigned int SHORTCUT_ITEMS = 20;
const unsigned int SHORTCUT_TABS = 3;

class Item;

/**
 * The class which keeps track of the item shortcuts.
 */
class ItemShortcut final
{
    public:
        /**
         * Constructor.
         */
        explicit ItemShortcut(const int number);

        A_DELETE_COPY(ItemShortcut)

        /**
         * Destructor.
         */
        ~ItemShortcut();

        /**
         * Load the configuration information.
         */
        void load(const bool oldConfig = false);

        /**
         * Save the configuration information.
         */
        void save() const;

        /**
         * Returns the shortcut item ID specified by the index.
         *
         * @param index Index of the shortcut item.
         */
        int getItem(const int index) const A_WARN_UNUSED
        { return mItems[index]; }

        unsigned char getItemColor(const int index) const A_WARN_UNUSED
        { return mItemColors[index]; }

        /**
         * Returns the amount of shortcut items.
         */
        int getItemCount() const A_WARN_UNUSED
        { return SHORTCUT_ITEMS; }

        /**
         * Returns the item ID that is currently selected.
         */
        int getItemSelected() const A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * Adds the selected item ID to the items specified by the index.
         *
         * @param index Index of the items.
         */
        void setItem(const int index);

        void setItem(const int index, const int item,
                     const unsigned char color);

        /**
         * Adds an item to the items store specified by the index.
         *
         * @param index Index of the item.
         * @param itemId ID of the item.
         */
        void setItems(const int index, const int itemId,
                      const unsigned char color)
        { mItems[index] = itemId; mItemColors[index] = color; save(); }

        /**
         * Set the item that is selected.
         *
         * @param itemId The ID of the item that is to be assigned.
         */
        void setItemSelected(const int itemId)
        { mItemSelected = itemId; }

        void setItemSelected(const Item *const item);

        /**
         * Returns selected shortcut item ID.
         */
        int getSelectedItem() const A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Remove a item from the shortcut.
         */
        void removeItem(const int index)
        { mItems[index] = -1;  save(); }

        /**
         * Try to use the item specified by the index.
         *
         * @param index Index of the item shortcut.
         */
        void useItem(const int index) const;

        /**
         * Equip a item from the shortcut.
         */
        void equipItem(const int index) const;

        /**
         * UnEquip a item from the shortcut.
         */
        void unequipItem(const int index) const;

        void swap(const int index1, const int index2);

    private:
        int mItems[SHORTCUT_ITEMS];                /**< The items. */
        unsigned char mItemColors[SHORTCUT_ITEMS]; /**< The item colors. */
        int mItemSelected;
        unsigned char mItemColorSelected;
        int mNumber;
};

extern ItemShortcut *itemShortcut[SHORTCUT_TABS];

#endif  // ITEMSHORTCUT_H
