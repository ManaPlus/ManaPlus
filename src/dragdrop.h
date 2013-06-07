/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef DRAGDROP_H
#define DRAGDROP_H

#include "item.h"

#include "resources/image.h"

#include "localconsts.h"

enum DragDropSource
{
    DRAGDROP_SOURCE_EMPTY = 0,
    DRAGDROP_SOURCE_INVENTORY,
    DRAGDROP_SOURCE_STORAGE,
    DRAGDROP_SOURCE_CART,
    DRAGDROP_SOURCE_TRADE,
    DRAGDROP_SOURCE_OUTFIT,
    DRAGDROP_SOURCE_SPELLS,
    DRAGDROP_SOURCE_GROUND,
    DRAGDROP_SOURCE_DROP,
    DRAGDROP_SOURCE_SHORTCUTS,
    DRAGDROP_SOURCE_CRAFT
};

class DragDrop
{
    public:
        DragDrop(Item *const item, const DragDropSource source) :
            mItem(item ? item->getId() : 0),
            mItemColor(item ? item->getColor() : 1),
            mItemImage(item ? item->getImage() : nullptr),
            mSelItem(0),
            mSelItemColor(1),
            mSource(source)
        {
            if (mItemImage)
                mItemImage->incRef();
        }

        A_DELETE_COPY(DragDrop)

        ~DragDrop()
        {
            if (mItemImage)
                mItemImage->decRef();
        }

        int getItem()
        { return mItem; }

        int getItemColor()
        { return mItemColor; }

        Image *getItemImage()
        { return mItemImage; }

        DragDropSource getSource() const
        { return mSource; }

        void dragItem(Item *const item, const DragDropSource source)
        {
            if (mItemImage)
                mItemImage->decRef();

            if (item)
            {
                mItem = item->getId();
                mItemColor = item->getColor();
                mItemImage = item->getImage();
                if (mItemImage)
                    mItemImage->incRef();
            }
            else
            {
                mItem = 0;
                mItemColor = 1;
                mItemImage = nullptr;
            }
            mSource = source;
        }

        void clear()
        {
            if (mItemImage)
                mItemImage->decRef();
            mItem = 0;
            mItemColor = 1;
            mItemImage = nullptr;
            mSource = DRAGDROP_SOURCE_EMPTY;
        }

        bool isEmpty() const
        { return mSource == DRAGDROP_SOURCE_EMPTY; }

        void select(Item *const item)
        {
            if (item)
            {
                mSelItem = item->getId();
                mSelItemColor = item->getColor();
            }
            else
            {
                mSelItem = 0;
                mSelItemColor = 1;
            }
        }

        void deselect()
        {
            mSelItem = 0;
            mSelItemColor = 1;
        }

        int getSelected()
        { return mSelItem; }

        int getSelectedColor()
        { return mSelItemColor; }

        bool isSelected()
        { return mSelItem > 0; }

        void clearItem(const Item *const item A_UNUSED)
        {
        }

    private:
        int mItem;
        uint8_t mItemColor;
        Image *mItemImage;
        int mSelItem;
        uint8_t mSelItemColor;
        DragDropSource mSource;
};

extern DragDrop dragDrop;

#endif  // DRAGDROP_H
