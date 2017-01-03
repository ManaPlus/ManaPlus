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

#ifndef BEING_FLOORITEM_H
#define BEING_FLOORITEM_H

#include "const/resources/item/cards.h"

#include "enums/resources/cursor.h"

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/damaged.h"
#include "enums/simpletypes/identified.h"
#include "enums/simpletypes/itemcolor.h"

#include "being/actorsprite.h"

class ItemInfo;

/**
 * An item lying on the floor.
 */
class FloorItem final : public ActorSprite
{
    public:
        /**
         * Constructor.
         *
         * @param id     the unique ID of this item instance
         * @param itemId the item ID
         * @param x      the x position in tiles
         * @param y      the y position in tiles
         * @param amount the item amount
         * @param color  the item color
         */
        FloorItem(const BeingId id,
                  const int itemId,
                  const int x, const int y,
                  const ItemTypeT itemType,
                  const int amount,
                  const int refine,
                  const ItemColor color,
                  const Identified identified,
                  const Damaged damaged,
                  const int *const cards);

        A_DELETE_COPY(FloorItem)

        void postInit(Map *const map, int subX, int subY);

        ActorTypeT getType() const noexcept2 override final A_WARN_UNUSED
        { return ActorType::FloorItem; }

        void draw(Graphics *const graphics,
                  const int offsetX, const int offsetY)
                  const override final A_NONNULL(2);

        /**
         * Returns the item ID.
         */
        int getItemId() const noexcept2 A_WARN_UNUSED
        { return mItemId; }

        /**
         * Returns the item info for this floor item. Useful for adding an item
         * link for the floor item to chat.
         */
        const ItemInfo &getInfo() const A_WARN_UNUSED;

        std::string getName() const A_WARN_UNUSED;

        int getTileX() const override final A_WARN_UNUSED
        { return mX; }

        int getTileY() const override final A_WARN_UNUSED
        { return mY; }

        void incrementPickup() noexcept2
        { mPickupCount ++; }

        unsigned getPickupCount() const noexcept2 A_WARN_UNUSED
        { return mPickupCount; }

        ItemColor getColor() const noexcept2 A_WARN_UNUSED
        { return mColor; }

        bool getShowMsg() const noexcept2 A_WARN_UNUSED
        { return mShowMsg; }

        void setShowMsg(const bool n) noexcept2
        { mShowMsg = n; }

        void disableHightlight() noexcept2
        { mHighlight = false; }

        CursorT getHoverCursor() const noexcept2 A_WARN_UNUSED
        { return mCursor; }

        void setCards(const int *const cards, int sz);

        int getCard(const int index) const;

        int getRefine() const noexcept2 A_WARN_UNUSED
        { return mRefine; }

        ItemTypeT getItemType() const noexcept2 A_WARN_UNUSED
        { return mItemType; }

        Identified getIdentified() const noexcept2 A_WARN_UNUSED
        { return mIdentified; }

        Damaged getDamaged() const noexcept2 A_WARN_UNUSED
        { return mDamaged; }

    private:
        int mCards[maxCards];
        int mItemId;
        int mX, mY;
        time_t mDropTime;
        int mAmount;
        int mRefine;
        int mHeightPosDiff;
        ItemTypeT mItemType;
        unsigned int mPickupCount;
        CursorT mCursor;
        ItemColor mColor;
        Identified mIdentified;
        Damaged mDamaged;
        bool mShowMsg;
        bool mHighlight;
};

#endif  // BEING_FLOORITEM_H
