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

#ifndef FLOORITEM_H
#define FLOORITEM_H

#include "being/actorsprite.h"

#include "resources/cursor.h"

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
        FloorItem(const int id, const int itemId, const int x, const int y,
                  const int amount, const unsigned char color);

        A_DELETE_COPY(FloorItem)

        void postInit(Map *const map, int subX, int subY);

        Type getType() const override final A_WARN_UNUSED
        { return FLOOR_ITEM; }

        void draw(Graphics *const graphics,
                  const int offsetX, const int offsetY) const override final;

        /**
         * Returns the item ID.
         */
        int getItemId() const A_WARN_UNUSED
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

        void incrementPickup()
        { mPickupCount ++; }

        unsigned getPickupCount() const A_WARN_UNUSED
        { return mPickupCount; }

        unsigned char getColor() const A_WARN_UNUSED
        { return mColor; }

        bool getShowMsg() const A_WARN_UNUSED
        { return mShowMsg; }

        void setShowMsg(const bool n)
        { mShowMsg = n; }

        void disableHightlight()
        { mHighlight = false; }

        Cursor::Cursor getHoverCursor() const A_WARN_UNUSED
        { return mCursor; }

    private:
        int mItemId;
        int mX, mY;
        int mDropTime;
        int mAmount;
        unsigned int mPickupCount;
        Cursor::Cursor mCursor;
        unsigned char mColor;
        bool mShowMsg;
        bool mHighlight;
};

#endif  // FLOORITEM_H
