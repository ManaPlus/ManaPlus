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
            mItem(item),
            mSource(source)
        {
        }

        Item *getItem()
        { return mItem; }

        DragDropSource getSource() const
        { return mSource; }

        void dragItem(Item *const item, const DragDropSource source)
        {
            mItem = item;
            mSource = source;
        }

        void clear()
        {
            mItem = nullptr;
            mSource = DRAGDROP_SOURCE_EMPTY;
        }

        bool isEmpty() const
        { return mSource == DRAGDROP_SOURCE_EMPTY; }

    private:
        Item *mItem;
        DragDropSource mSource;
};

extern DragDrop dragDrop;

#endif  // DRAGDROP_H
