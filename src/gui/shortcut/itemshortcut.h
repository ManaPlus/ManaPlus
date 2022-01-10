/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_SHORTCUT_ITEMSHORTCUT_H
#define GUI_SHORTCUT_ITEMSHORTCUT_H

#include "const/itemshortcut.h"

#include "enums/simpletypes/itemcolor.h"

#include <string>

#include "localconsts.h"

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
        explicit ItemShortcut(const size_t number);

        A_DELETE_COPY(ItemShortcut)

        /**
         * Destructor.
         */
        ~ItemShortcut();

        /**
         * Load the configuration information.
         */
        void load();

        /**
         * Save the configuration information.
         */
        void save() const;

        /**
         * Returns the shortcut item ID specified by the index.
         *
         * @param index Index of the shortcut item.
         */
        int getItem(const size_t index) const A_WARN_UNUSED
        { return mItems[index]; }

        ItemColor getItemColor(const size_t index) const A_WARN_UNUSED
        { return mItemColors[index]; }

        void setItemData(const size_t index,
                         const std::string &data)
        { mItemData[index] = data; }

        std::string getItemData(const size_t index) const A_WARN_UNUSED
        { return mItemData[index]; }

        /**
         * Returns the amount of shortcut items.
         */
        constexpr static int getItemCount() A_WARN_UNUSED
        { return SHORTCUT_ITEMS; }

        /**
         * Returns the item ID that is currently selected.
         */
        int getItemSelected() const noexcept2 A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * Adds the selected item ID to the items specified by the index.
         *
         * @param index Index of the items.
         */
        void setItem(const size_t index);

        void setItem(const size_t index,
                     const int item,
                     const ItemColor color);

        void setItemFast(const size_t index,
                         const int item,
                         const ItemColor color);

        /**
         * Adds an item to the items store specified by the index.
         *
         * @param index Index of the item.
         * @param itemId ID of the item.
         */
        void setItems(const size_t index,
                      const int itemId,
                      const ItemColor color)
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
        int getSelectedItem() const noexcept2 A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const noexcept2 A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Remove a item from the shortcut.
         */
        void removeItem(const size_t index)
        { mItems[index] = -1;  save(); }

        /**
         * Try to use the item specified by the index.
         *
         * @param index Index of the item shortcut.
         */
        void useItem(const size_t index) const;

        /**
         * Equip a item from the shortcut.
         */
        void equipItem(const size_t index) const;

        /**
         * UnEquip a item from the shortcut.
         */
        void unequipItem(const size_t index) const;

        void swap(const size_t index1,
                  const size_t index2);

        void clear();

        size_t getFreeIndex() const A_WARN_UNUSED;

    private:
        int mItems[SHORTCUT_ITEMS];             /**< The items. */
        ItemColor mItemColors[SHORTCUT_ITEMS];  /**< The item colors. */
        std::string mItemData[SHORTCUT_ITEMS];
        size_t mNumber;
        int mItemSelected;
        ItemColor mItemColorSelected;
};

extern ItemShortcut *itemShortcut[SHORTCUT_TABS];

#endif  // GUI_SHORTCUT_ITEMSHORTCUT_H
