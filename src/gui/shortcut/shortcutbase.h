/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef GUI_SHORTCUT_SHORTCUTBASE_H
#define GUI_SHORTCUT_SHORTCUTBASE_H

#include "enums/simpletypes/itemcolor.h"

#include "utils/cast.h"

#include <string>

#include "localconsts.h"

class Item;

/**
 * The class which keeps track of the item shortcuts.
 */
class ShortcutBase notfinal
{
    public:
        /**
         * Constructor.
         */
        ShortcutBase(const std::string &itemName,
                     const std::string &colorName,
                     const int maxSize);

        A_DELETE_COPY(ShortcutBase)

        /**
         * Destructor.
         */
        virtual ~ShortcutBase();

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

        /**
         * Returns the amount of shortcut items.
         */
        int getItemCount() const noexcept2 A_WARN_UNUSED
        { return CAST_S32(mMaxSize); }

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
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const noexcept2 A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Remove a item from the shortcut.
         */
        void removeItem(const size_t index)
        { mItems[index] = -1; save(); }

        void clear(const bool isSave);

    private:
        int *mItems A_NONNULLPOINTER;
        ItemColor *mItemColors A_NONNULLPOINTER;
        std::string mItemName;
        std::string mColorName;
        size_t mMaxSize;
        int mItemSelected;
        ItemColor mItemColorSelected;
};

#endif  // GUI_SHORTCUT_SHORTCUTBASE_H
