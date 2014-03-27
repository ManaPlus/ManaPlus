/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "inventory.h"

#include "item.h"
#include "logger.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <algorithm>
#include <functional>
#include <string>

#include "debug.h"

struct SlotUsed final
{
    bool operator()(const Item *const item) const
    {
        return item && item->mId >= 0 && item->mQuantity > 0;
    }
    typedef Item *argument_type;
};

Inventory::Inventory(const int type, const int size1) :
    mInventoryListeners(),
    mType(type),
    mSize(size1 == -1 ? static_cast<unsigned>(
          Net::getInventoryHandler()->getSize(type))
          : static_cast<unsigned>(size1)),
    mItems(new Item*[mSize]),
    mUsed(0)
{
    std::fill_n(mItems, mSize, static_cast<Item*>(nullptr));
}

Inventory::~Inventory()
{
    for (unsigned i = 0; i < mSize; i++)
        delete mItems[i];

    delete [] mItems;
    mItems = nullptr;
}

Item *Inventory::getItem(const int index) const
{
    if (index < 0 || index >= static_cast<int>(mSize) || !mItems[index]
        || mItems[index]->mQuantity <= 0)
    {
        return nullptr;
    }

    return mItems[index];
}

Item *Inventory::findItem(const int itemId, const unsigned char color) const
{
    for (unsigned i = 0; i < mSize; i++)
    {
        Item *const item = mItems[i];
        if (item && item->mId == itemId)
        {
            if (color == 0 || item->mColor == color
                || (color == 1 && item->mColor <= 1))
            {
                return item;
            }
        }
    }

    return nullptr;
}

void Inventory::addItem(const int id, const int quantity, const int refine,
                        const unsigned char color, const bool equipment)
{
    setItem(getFreeSlot(), id, quantity, refine, color, equipment);
}

void Inventory::setItem(const int index, const int id, const int quantity,
                        const int refine, const unsigned char color,
                        const bool equipment)
{
    if (index < 0 || index >= static_cast<int>(mSize))
    {
        logger->log("Warning: invalid inventory index: %d", index);
        return;
    }

    Item *const item1 = mItems[index];
    if (!item1 && id > 0)
    {
        Item *const item = new Item(id, quantity, refine, color, equipment);
        item->setInvIndex(index);
        mItems[index] = item;
        mUsed++;
        distributeSlotsChangedEvent();
    }
    else if (id > 0 && item1)
    {
        item1->setId(id, color);
        item1->setQuantity(quantity);
        item1->setRefine(refine);
        item1->setEquipment(equipment);
    }
    else if (item1)
    {
        removeItemAt(index);
    }
}

void Inventory::clear()
{
    for (unsigned i = 0; i < mSize; i++)
        removeItemAt(i);
}

void Inventory::removeItem(const int id)
{
    for (unsigned i = 0; i < mSize; i++)
    {
        const Item *const item = mItems[i];
        if (item && item->mId == id)
            removeItemAt(i);
    }
}

void Inventory::removeItemAt(const int index)
{
    delete2(mItems[index]);
    mUsed--;
    if (mUsed < 0)  // Already at 0, no need to distribute event
        mUsed = 0;
    else
        distributeSlotsChangedEvent();
}

bool Inventory::contains(const Item *const item) const
{
    if (!item)
        return false;

    const int id = item->mId;
    for (unsigned i = 0; i < mSize; i++)
    {
        const Item *const item1 = mItems[i];
        if (item1 && item1->mId == id)
            return true;
    }

    return false;
}

int Inventory::getFreeSlot() const
{
    Item **const i = std::find_if(mItems, mItems + mSize,
        std::not1(SlotUsed()));
    return (i == mItems + static_cast<int>(mSize)) ? -1
        : static_cast<int>(i - mItems);
}

int Inventory::getLastUsedSlot() const
{
    for (int i = mSize - 1; i >= 0; i--)
    {
        if (SlotUsed()(mItems[i]))
            return i;
    }

    return -1;
}

void Inventory::addInventoyListener(InventoryListener* const listener)
{
    mInventoryListeners.push_back(listener);
}

void Inventory::removeInventoyListener(InventoryListener* const listener)
{
    mInventoryListeners.remove(listener);
}

void Inventory::distributeSlotsChangedEvent()
{
    FOR_EACH (InventoryListenerList::const_iterator, i, mInventoryListeners)
        (*i)->slotsChanged(this);
}

const Item *Inventory::findItemBySprite(std::string spritePath,
                                        const Gender gender,
                                        const int race) const
{
    spritePath = removeSpriteIndex(spritePath);

    const std::string spritePathShort = extractNameFromSprite(spritePath);
    int partialIndex = -1;

    for (unsigned i = 0; i < mSize; i++)
    {
        const Item *const item = mItems[i];
        if (item)
        {
            std::string path = item->getInfo().getSprite(gender, race);
            if (!path.empty())
            {
                path = removeSpriteIndex(path);
                if (spritePath == path)
                    return item;

                path = extractNameFromSprite(path);
                if (spritePathShort == path)
                    partialIndex = i;
            }
        }
    }
    if (partialIndex != -1)
        return mItems[partialIndex];

    return nullptr;
}

std::string Inventory::getName() const
{
    switch (mType)
    {
        case INVENTORY:
        default:
        {
            // TRANSLATORS: inventory type name
            return N_("Inventory");
        }
        case STORAGE:
        {
            // TRANSLATORS: inventory type name
            return N_("Storage");
        }
        case CART:
        {
            // TRANSLATORS: inventory type name
            return N_("Cart");
        }
    }
}

void Inventory::resize(const unsigned int newSize)
{
    clear();
    if (mSize == newSize)
        return;

    for (unsigned i = 0; i < mSize; i++)
        delete mItems[i];
    delete [] mItems;

    mSize = newSize;
    mItems = new Item*[mSize];
    std::fill_n(mItems, mSize, static_cast<Item*>(nullptr));
}
