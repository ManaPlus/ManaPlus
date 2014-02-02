/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef DROPSHORTCUT_H
#define DROPSHORTCUT_H

#include "localconsts.h"

static const int DROP_SHORTCUT_ITEMS = 16;

class Item;

/**
 * The class which keeps track of the item shortcuts.
 */
class DropShortcut final
{
    public:
        /**
         * Constructor.
         */
        DropShortcut();

        A_DELETE_COPY(DropShortcut)

        /**
         * Destructor.
         */
        ~DropShortcut();

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
        static int getItemCount() A_WARN_UNUSED
        { return DROP_SHORTCUT_ITEMS; }

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
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Remove a item from the shortcut.
         */
        void removeItem(const int index)
        { mItems[index] = -1; save(); }

        /**
         * Try to use the item specified by the index.
         *
         * @param index Index of the item shortcut.
         */
        void useItem(int index);

        /**
         * Equip a item from the shortcut.
         */
        void equipItem(int index);

        /**
         * UnEquip a item from the shortcut.
         */
        void unequipItem(int index);

        /**
         * Drop first item.
         */
        void dropFirst() const;

        /**
         * Drop all items in cicle.
         */
        void dropItems(const int cnt = 1);

        void clear(const bool isSave = true);

    private:
        /**
         * Drop item in cicle.
         */
        bool dropItem(const int cnt = 1);

        int mItems[DROP_SHORTCUT_ITEMS];
        unsigned char mItemColors[DROP_SHORTCUT_ITEMS];
        int mItemSelected;
        unsigned char mItemColorSelected;

        int mLastDropIndex;
};

extern DropShortcut *dropShortcut;

#endif  // DROPSHORTCUT_H
