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

#include "inventory.h"

#include "item.h"
#include "logger.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"

#include <algorithm>
#include <functional>
#include <string>

#include "debug.h"

struct SlotUsed final : public std::unary_function<Item*, bool>
{
    bool operator()(const Item *const item) const
    {
        return item && item->mId >= 0 && item->mQuantity > 0;
    }
};

Inventory::Inventory(const int type, const int size) :
    mType(type),
    mSize(size == -1 ? static_cast<unsigned>(
          Net::getInventoryHandler()->getSize(type))
          : static_cast<unsigned>(size)),
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
        if (mItems[i] && mItems[i]->mId == itemId)
        {
            if (color == 0 || mItems[i]->mColor == color
                || (color == 1 && mItems[i]->mColor <= 1))
            {
                return mItems[i];
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

    if (!mItems[index] && id > 0)
    {
        Item *const item = new Item(id, quantity, refine, color, equipment);
        item->setInvIndex(index);
        mItems[index] = item;
        mUsed++;
        distributeSlotsChangedEvent();
    }
    else if (id > 0 && mItems[index])
    {
        mItems[index]->setId(id, color);
        mItems[index]->setQuantity(quantity);
        mItems[index]->setRefine(refine);
        mItems[index]->setEquipment(equipment);
    }
    else if (mItems[index])
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
        if (mItems[i] && mItems[i]->mId == id)
            removeItemAt(i);
    }
}

void Inventory::removeItemAt(const int index)
{
    delete mItems[index];
    mItems[index] = nullptr;
    mUsed--;
    if (mUsed < 0) // Already at 0, no need to distribute event
        mUsed = 0;
    else
        distributeSlotsChangedEvent();
}

bool Inventory::contains(const Item *const item) const
{
    if (!item)
        return false;

    for (unsigned i = 0; i < mSize; i++)
    {
        if (mItems[i] && mItems[i]->mId == item->mId)
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

Item *Inventory::findItemBySprite(std::string spritePath,
                                  const Gender gender, const int race) const
{
    spritePath = removeSpriteIndex(spritePath);
//    logger->log1("Inventory::FindItemBySprite sprite: " + spritePath);

    std::string spritePathShort = extractNameFromSprite(spritePath);
//    logger->log1("Inventory::FindItemBySprite spriteShort: " + spritePathShort);
    int partialIndex = -1;

    for (unsigned i = 0; i < mSize; i++)
    {
        if (mItems[i])
        {
            std::string path = mItems[i]->getInfo().getSprite(gender, race);
            if (!path.empty())
            {
                path = removeSpriteIndex(path);

//                logger->log("Inventory::FindItemBySprite normal: " + path);

                if (spritePath == path)
                    return mItems[i];

                path = extractNameFromSprite(path);
//                logger->log("Inventory::FindItemBySprite short: " + path);
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
            return N_("Inventory");
        case STORAGE:
            return N_("Storage");
        case CART:
            return N_("Cart");
    }
}
