/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef RESOURCES_ITEM_ITEM_H
#define RESOURCES_ITEM_ITEM_H

#include "const/resources/item/cards.h"

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/damaged.h"
#include "enums/simpletypes/equipm.h"
#include "enums/simpletypes/equipped.h"
#include "enums/simpletypes/favorite.h"
#include "enums/simpletypes/identified.h"
#include "enums/simpletypes/itemcolor.h"

#include "resources/db/itemdb.h"

#include "localconsts.h"

class Image;

struct ItemOptionsList;

/**
 * Represents one or more instances of a certain item type.
 */
class Item notfinal
{
    public:
        /**
         * Constructor.
         */
        Item(const int id,
             const ItemTypeT type,
             const int quantity,
             const uint8_t refine,
             const ItemColor color,
             const Identified identified,
             const Damaged damaged,
             const Favorite favorite,
             const Equipm equipment,
             const Equipped equipped);

        A_DELETE_COPY(Item)

        /**
         * Destructor.
         */
        virtual ~Item();

        /**
         * Sets the item id, color the item type.
         */
        void setId(const int id, const ItemColor color);

        /**
         * Returns the item id.
         */
        int getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        /**
         * Returns the item image.
         */
        Image *getImage() const noexcept2 A_WARN_UNUSED
        { return mImage; }

        /**
         * Sets the number of items.
         */
        void setQuantity(const int quantity) noexcept2
        { mQuantity = quantity; }

        /**
         * Increases the number of items by the given amount.
         */
        void increaseQuantity(const int amount) noexcept2
        { mQuantity += amount; }

        /**
         * Returns the number of items.
         */
        int getQuantity() const noexcept2 A_WARN_UNUSED
        { return mQuantity; }

        /**
         * Sets whether this item is considered equipment.
         */
        void setEquipment(const Equipm equipment) noexcept2
        { mEquipment = equipment; }

        /**
         * Returns whether this item is considered equipment.
         */
        Equipm isEquipment() const noexcept2 A_WARN_UNUSED
        { return mEquipment; }

        /**
         * Sets whether this item is equipped.
         */
        void setEquipped(const Equipped equipped) noexcept2
        { mEquipped = equipped; }

        /**
         * Returns whether this item is equipped.
         */
        Equipped isEquipped() const noexcept2 A_WARN_UNUSED
        { return mEquipped; }

        /**
         * Sets this item refine level.
         */
        void setRefine(const uint8_t refine) noexcept2
        { mRefine = refine; }

        /**
         * Returns this item refine level.
         */
        uint8_t getRefine() const noexcept2 A_WARN_UNUSED
        { return mRefine; }

        /**
         * Sets whether this item is in equipment.
         */
        void setInEquipment(const bool inEquipment) noexcept2
        { mInEquipment = inEquipment; }

        /**
         * Returns whether this item is in equipment.
         */
        bool isInEquipment() const noexcept2 A_WARN_UNUSED
        { return mInEquipment; }

        /**
         * Sets the inventory index of this item.
         */
        void setInvIndex(const int index) noexcept2
        { mInvIndex = index; }

        /**
         * Returns the inventory index of this item.
         */
        int getInvIndex() const noexcept2 A_WARN_UNUSED
        { return mInvIndex; }

        /**
         * Returns information about this item type.
         */
        const ItemInfo &getInfo() const A_WARN_UNUSED
        { return ItemDB::get(mId); }

        std::string getName() const A_WARN_UNUSED;

        static Image *getImage(const int id,
                               const ItemColor color) A_WARN_UNUSED;

        bool isHaveTag(const int tagId) const A_WARN_UNUSED;

        ItemColor getColor() const noexcept2 A_WARN_UNUSED
        { return mColor; }

        void setColor(const ItemColor color) noexcept2
        { mColor = color; }

        const std::string &getDescription() const noexcept2 A_WARN_UNUSED
        { return mDescription; }

        void setIdentified(const Identified b) noexcept2
        { mIdentified = b; }

        Identified getIdentified() const noexcept2 A_WARN_UNUSED
        { return mIdentified; }

        void setDamaged(const Damaged b) noexcept2
        { mDamaged = b; }

        Damaged getDamaged() const noexcept2 A_WARN_UNUSED
        { return mDamaged; }

        void setFavorite(const Favorite b) noexcept2
        { mFavorite = b; }

        Favorite getFavorite() const noexcept2 A_WARN_UNUSED
        { return mFavorite; }

        void setCard(const int index, const int id);

        int getCard(const int index) const;

        void setCards(const int *const cards, const int size);

        const int *getCards() const noexcept2 A_WARN_UNUSED
        { return mCards; }

        void setOptions(const ItemOptionsList *const options);

        const ItemOptionsList *getOptions() const noexcept2 A_WARN_UNUSED
        { return mOptions; }

        void setType(const ItemTypeT type) noexcept2
        { mType = type; }

        ItemTypeT getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void setTag(const int tag) noexcept2
        { mTag = tag; }

        int getTag() const noexcept2 A_WARN_UNUSED
        { return mTag; }

        void addCard(const int card);

        void updateColor();

        static constexpr bool isItem(const int id) noexcept2 A_WARN_UNUSED
        {
            return id != 0 &&
                id != CARD0_FORGE &&
                id != CARD0_CREATE &&
                id != CARD0_PET;
        }

        int mId;              /**< Item id. */
        ItemColor mColor;
        int mQuantity;        /**< Number of items. */
        int mTag;

    protected:
        Image *mImage;        /**< Item image. */
        std::string mDescription;
        std::map <int, int> mTags;
        int mCards[maxCards];
        const ItemOptionsList *mOptions;
        uint8_t mRefine;      /**< Item refine level. */
        int mInvIndex;        /**< Inventory index. */
        ItemTypeT mType;      /**< Item type. */
        Equipm mEquipment;    /**< Item is equipment. */
        Equipped mEquipped;   /**< Item is equipped. */
        bool mInEquipment;    /**< Item is in equipment */
        Identified mIdentified;
        Damaged mDamaged;
        Favorite mFavorite;
};

#endif  // RESOURCES_ITEM_ITEM_H
