/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "log.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"

#include <algorithm>
#include <string>

struct SlotUsed : public std::unary_function<Item*, bool>
{
    bool operator()(const Item *item) const
    {
        return item && item->getId() >= 0 && item->getQuantity() > 0;
    }
};

Inventory::Inventory(int type, int size):
    mType(type),
    mSize(size == -1 ? Net::getInventoryHandler()->getSize(type)
          : static_cast<unsigned>(size)),
    mUsed(0)
{
    mItems = new Item*[mSize];
    std::fill_n(mItems, mSize, static_cast<Item*>(0));
}

Inventory::~Inventory()
{
    for (unsigned i = 0; i < mSize; i++)
        delete mItems[i];

    delete [] mItems;
    mItems = 0;
}

Item *Inventory::getItem(int index) const
{
    if (index < 0 || index >= static_cast<int>(mSize) || !mItems[index]
        || mItems[index]->getQuantity() <= 0)
    {
        return 0;
    }

    return mItems[index];
}

Item *Inventory::findItem(int itemId) const
{
    for (unsigned i = 0; i < mSize; i++)
    {
        if (mItems[i] && mItems[i]->getId() == itemId)
            return mItems[i];
    }

    return 0;
}

void Inventory::addItem(int id, int quantity, int refine,
                        unsigned char color, bool equipment)
{
    setItem(getFreeSlot(), id, quantity, refine, color, equipment);
}

void Inventory::setItem(int index, int id, int quantity,
                        int refine, unsigned char color, bool equipment)
{
    if (index < 0 || index >= static_cast<int>(mSize))
    {
        logger->log("Warning: invalid inventory index: %d", index);
        return;
    }

    if (!mItems[index] && id > 0)
    {
        Item *item = new Item(id, quantity, refine, color, equipment);
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

void Inventory::removeItem(int id)
{
    for (unsigned i = 0; i < mSize; i++)
    {
        if (mItems[i] && mItems[i]->getId() == id)
            removeItemAt(i);
    }
}

void Inventory::removeItemAt(int index)
{
    delete mItems[index];
    mItems[index] = 0;
    mUsed--;
    if (mUsed < 0) // Already at 0, no need to distribute event
        mUsed = 0;
    else
        distributeSlotsChangedEvent();
}

bool Inventory::contains(Item *item) const
{
    for (unsigned i = 0; i < mSize; i++)
    {
        if (mItems[i] && mItems[i]->getId() == item->getId())
            return true;
    }

    return false;
}

int Inventory::getFreeSlot() const
{
    Item **i = std::find_if(mItems, mItems + mSize,
        std::not1(SlotUsed()));
    return (i == mItems + static_cast<int>(mSize)) ? -1 : (i - mItems);
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

void Inventory::addInventoyListener(InventoryListener* listener)
{
    mInventoryListeners.push_back(listener);
}

void Inventory::removeInventoyListener(InventoryListener* listener)
{
    mInventoryListeners.remove(listener);
}

void Inventory::distributeSlotsChangedEvent()
{
    InventoryListenerList::const_iterator i = mInventoryListeners.begin();
    InventoryListenerList::const_iterator i_end = mInventoryListeners.end();
    for (; i != i_end; ++i)
        (*i)->slotsChanged(this);
}

Item *Inventory::findItemBySprite(std::string spritePath, Gender gender)
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
            std::string path = mItems[i]->getInfo().getSprite(gender);
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

    return 0;
}

std::string Inventory::getName()
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
