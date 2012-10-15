/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "actorsprite.h"

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
         * @param map    the map this item is on
         * @param amount the item amount
         * @param color  the item color
         * @param subX   the x pixel relative position
         * @param subY   the y pixel relative position
         */
        FloorItem(const int id, const int itemId, const int x, const int y,
                  Map *const map, const int amount, const unsigned char color,
                  int subX, int subY);

        A_DELETE_COPY(FloorItem)

        Type getType() const override
        { return FLOOR_ITEM; }

        bool draw(Graphics *graphics, int offsetX, int offsetY) const override;

        /**
         * Returns the item ID.
         */
        int getItemId() const
        { return mItemId; }

        /**
         * Returns the item info for this floor item. Useful for adding an item
         * link for the floor item to chat.
         */
        const ItemInfo &getInfo() const;

        std::string getName() const;

        virtual int getTileX() const override
        { return mX; }

        virtual int getTileY() const override
        { return mY; }

        void incrementPickup()
        { mPickupCount ++; }

        unsigned getPickupCount() const
        { return mPickupCount; }

        unsigned char getColor() const
        { return mColor; }

        bool getShowMsg() const
        { return mShowMsg; }

        void setShowMsg(const bool n)
        { mShowMsg = n; }

        void disableHightlight()
        { mHighlight = false; }

    private:
        int mItemId;
        int mX, mY;
//        Item *mItem;
        Map *mMap;
//        float mAlpha;
        int mDropTime;
        int mAmount;
        unsigned mPickupCount;
        unsigned char mColor;
        bool mShowMsg;
        bool mHighlight;
};

#endif
