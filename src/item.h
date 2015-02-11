/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef ITEM_H
#define ITEM_H

#include "enums/simpletypes.h"

#include "resources/db/itemdb.h"

#include <map>

#include "localconsts.h"

class Image;

const int maxCards = 4;

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
             const int type,
             const int quantity,
             const uint8_t refine,
             const uint8_t color,
             const Identified identified,
             const Damaged damaged,
             const Favorite favorite,
             const Equipm equipment,
             const bool equipped);

        A_DELETE_COPY(Item)

        /**
         * Destructor.
         */
        virtual ~Item();

        /**
         * Sets the item id, color the item type.
         */
        void setId(const int id, const unsigned char color);

        /**
         * Returns the item id.
         */
        int getId() const A_WARN_UNUSED
        { return mId; }

        /**
         * Returns the item image.
         */
        Image *getImage() const A_WARN_UNUSED
        { return mImage; }

        /**
         * Sets the number of items.
         */
        void setQuantity(const int quantity)
        { mQuantity = quantity; }

        /**
         * Increases the number of items by the given amount.
         */
        void increaseQuantity(const int amount)
        { mQuantity += amount; }

        /**
         * Returns the number of items.
         */
        int getQuantity() const A_WARN_UNUSED
        { return mQuantity; }

        /**
         * Sets whether this item is considered equipment.
         */
        void setEquipment(const Equipm equipment)
        { mEquipment = equipment; }

        /**
         * Returns whether this item is considered equipment.
         */
        Equipm isEquipment() const A_WARN_UNUSED
        { return mEquipment; }

        /**
         * Sets whether this item is equipped.
         */
        void setEquipped(const bool equipped)
        { mEquipped = equipped; }

        /**
         * Returns whether this item is equipped.
         */
        bool isEquipped() const A_WARN_UNUSED
        { return mEquipped; }

        /**
         * Sets this item refine level.
         */
        void setRefine(const uint8_t refine)
        { mRefine = refine; }

        /**
         * Returns this item refine level.
         */
        uint8_t getRefine() const A_WARN_UNUSED
        { return mRefine; }

        /**
         * Sets whether this item is in equipment.
         */
        void setInEquipment(const bool inEquipment)
        { mInEquipment = inEquipment; }

        /**
         * Returns whether this item is in equipment.
         */
        bool isInEquipment() const A_WARN_UNUSED
        { return mInEquipment; }

        /**
         * Sets the inventory index of this item.
         */
        void setInvIndex(const int index)
        { mInvIndex = index; }

        /**
         * Returns the inventory index of this item.
         */
        int getInvIndex() const A_WARN_UNUSED
        { return mInvIndex; }

        /**
         * Returns information about this item type.
         */
        const ItemInfo &getInfo() const A_WARN_UNUSED
        { return ItemDB::get(mId); }

        std::string getName() const A_WARN_UNUSED;

        static Image *getImage(const int id,
                               const unsigned char color) A_WARN_UNUSED;

        bool isHaveTag(const int tagId) const A_WARN_UNUSED;

        unsigned char getColor() const A_WARN_UNUSED
        { return mColor; }

        const std::string &getDescription() const A_WARN_UNUSED
        { return mDescription; }

        void setIdentified(const Identified b)
        { mIdentified = b; }

        Identified getIdentified() const A_WARN_UNUSED
        { return mIdentified; }

        void setDamaged(const Damaged b)
        { mDamaged = b; }

        Damaged getDamaged() const A_WARN_UNUSED
        { return mDamaged; }

        void setFavorite(const Favorite b)
        { mFavorite = b; }

        Favorite getFavorite() const A_WARN_UNUSED
        { return mFavorite; }

        void setCard(const int index, const int id);

        int getCard(const int index) const;

        void setCards(const int *const cards, const int size);

        const int *getCards() const
        { return mCards; }

        void setType(const int type)
        { mType = type; }

        int getType() const A_WARN_UNUSED
        { return mType; }

        int mId;              /**< Item type id. */
        unsigned char mColor;
        int mQuantity;        /**< Number of items. */

    protected:
        Image *mImage;        /**< Item image. */
        std::string mDescription;
        std::map <int, int> mTags;
        int mCards[maxCards];
        uint8_t mRefine;      /**< Item refine level. */
        int mInvIndex;        /**< Inventory index. */
        int mType;            /**< Item type. */
        Equipm mEquipment;    /**< Item is equipment. */
        bool mEquipped;       /**< Item is equipped. */
        bool mInEquipment;    /**< Item is in equipment */
        Identified mIdentified;
        Damaged mDamaged;
        Favorite mFavorite;
};

#endif  // ITEM_H
