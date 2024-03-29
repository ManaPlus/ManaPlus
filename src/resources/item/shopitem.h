/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef RESOURCES_ITEM_SHOPITEM_H
#define RESOURCES_ITEM_SHOPITEM_H

#include "resources/item/item.h"

#include <stack>

#include "localconsts.h"

/**
 * Represents an item in a shop inventory. It can store quantity and inventory
 * indices of duplicate entries in the shop as well.
 */
class ShopItem final : public Item
{
    public:
        /**
         * Constructor. Creates a new ShopItem.
         *
         * @param inventoryIndex the inventory index of the item
         * @param id the id of the item
         * @param quantity number of available copies of the item
         * @param price price of the item
         */
        ShopItem(const int inventoryIndex,
                 const int id,
                 const ItemTypeT type,
                 const ItemColor color,
                 const int quantity,
                 const int price,
                 const std::string &currency);

        /**
         * Constructor. Creates a new ShopItem. Inventory index will be set to
         * -1 and quantity to 0.
         *
         * @param id the id of the item
         * @param price price of the item
         */
        ShopItem(const int id,
                 const ItemTypeT type,
                 const ItemColor color,
                 const int price,
                 const std::string &currency);

        A_DELETE_COPY(ShopItem)

        /**
         * Destructor.
         */
        ~ShopItem();

        /**
         * Add a duplicate. Id and price will be taken from this item.
         *
         * @param inventoryIndex the inventory index of the item
         * @param quantity number of available copies of the item
         */
        void addDuplicate(const int inventoryIndex, const int quantity);

        /**
         * Add a duplicate. Id and price will be taken from this item.
         * Needed for compatibility with ShopDuplicateItems (see) class
         * documentation).
         */
        void addDuplicate();

        void update();

        /**
         * Gets the quantity of the currently topmost duplicate.
         *
         * @return the quantity of the currently topmost duplicate
         */
        int getCurrentQuantity() const A_WARN_UNUSED
        {
            return mDuplicates.empty() ? 0 : mDuplicates.top()->quantity;
        }

        /**
         * Gets the inventory index of the currently topmost duplicate.
         *
         * @return the inventory index of the currently topmost duplicate
         */
        int getCurrentInvIndex() const A_WARN_UNUSED
        {
            return mDuplicates.empty() ? mInvIndex :
                   mDuplicates.top()->inventoryIndex;
        }

        /**
         * Reduces the quantity of the topmost duplicate by the specified
         * amount. Also reduces the total quantity of this DuplicateItem.
         * Empty duplicates are automatically removed.
         *
         * If the amount is bigger than the quantity of the current topmost,
         * only sell as much as possible. Returns the amount actually sold (do
         * not ignore the return value!)
         *
         * @return the amount, that actually was sold.
         */
        int sellCurrentDuplicate(const int quantity);

        /**
         * Gets the price of the item.
         *
         * @return the price of the item
         */
        int getPrice() const noexcept2 A_WARN_UNUSED
        { return mPrice; }

        /**
         * Gets the display name for the item in the shop list.
         *
         * @return the display name for the item in the shop list
         */
        const std::string &getDisplayName() const noexcept2 A_WARN_UNUSED
        { return mDisplayName; }

        void setVisible(const bool b) noexcept2
        { mVisible = b; }

        bool isVisible() const noexcept2 A_WARN_UNUSED
        { return mVisible; }

        void increaseUsedQuantity(const int amount);

        int getUsedQuantity() const noexcept2 A_WARN_UNUSED
        { return mUsedQuantity; }

        void setDisabled(const bool b) noexcept2
        { mDisabled = b; }

        bool getDisabled() const noexcept2 A_WARN_UNUSED
        { return mDisabled; }

    protected:
        void updateDisplayName(const int quantity);

        std::string mDisplayName;
        std::string mCurrency;

        /**
         * Struct to keep track of duplicates.
         */
        typedef struct
        {
            int inventoryIndex;
            int quantity;
        } DuplicateItem;
        std::stack<DuplicateItem*> mDuplicates; /** <-- Stores duplicates */
        int mPrice;
        int mUsedQuantity;
        bool mShowQuantity;
        bool mVisible;
        bool mDisabled;
};

#endif  // RESOURCES_ITEM_SHOPITEM_H
