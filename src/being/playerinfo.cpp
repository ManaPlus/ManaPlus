/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "being/playerinfo.h"

#include "client.h"
#include "configuration.h"
#include "inventory.h"

#include "being/attributes.h"
#include "being/homunculusinfo.h"
#include "being/localplayer.h"
#include "being/mercenaryinfo.h"
#include "being/petinfo.h"

#include "itemsoundmanager.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/npcpostdialog.h"

#include "resources/iteminfo.h"

#include "net/homunculushandler.h"
#include "net/inventoryhandler.h"
#include "net/mercenaryhandler.h"
#include "net/playerhandler.h"

#include "utils/delete2.h"

#include "listeners/statlistener.h"

#include "debug.h"

namespace PlayerInfo
{

PlayerInfoBackend mData;
int mCharId = 0;

Inventory *mInventory = nullptr;
Equipment *mEquipment = nullptr;
MercenaryInfo *mMercenary = nullptr;
HomunculusInfo *mHomunculus = nullptr;
PetInfo *mPet = nullptr;
GuildPositionFlags::Type mGuildPositionFlags = GuildPositionFlags::None;

bool mTrading = false;
int mLevelProgress = 0;
std::set<int> mProtectedItems;

// --- Triggers ---------------------------------------------------------------

void triggerAttr(const int id, const int old)
{
    AttributeListener::distributeEvent(id, old,
        mData.mAttributes.find(id)->second);
}

void triggerStat(const int id, const int old1, const int old2)
{
    StatListener::distributeEvent(id, old1, old2);
}

// --- Attributes -------------------------------------------------------------

int getAttribute(const int id)
{
    const IntMap::const_iterator it = mData.mAttributes.find(id);
    if (it != mData.mAttributes.end())
        return it->second;
    else
        return 0;
}

void setAttribute(const int id, const int value, const bool notify)
{
    const int old = mData.mAttributes[id];
    mData.mAttributes[id] = value;
    if (notify)
        triggerAttr(id, old);
}

int getSkillLevel(const int id)
{
    const IntMap::const_iterator it = mData.mSkills.find(id);
    if (it != mData.mSkills.end())
        return it->second;
    else
        return 0;
}

void setSkillLevel(const int id, const int value)
{
    mData.mSkills[id] = value;
}

// --- Stats ------------------------------------------------------------------

int getStatBase(const int id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.base;
    else
        return 0;
}

void setStatBase(const int id, const int value, const bool notify)
{
    const int old = mData.mStats[id].base;
    mData.mStats[id].base = value;
    if (notify)
        triggerStat(id, old, 0);
}

int getStatMod(const int id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.mod;
    else
        return 0;
}

void setStatMod(const int id, const int value, const bool notify)
{
    const int old = mData.mStats[id].mod;
    mData.mStats[id].mod = value;
    if (notify)
        triggerStat(id, old, 0);
}

int getStatEffective(const int id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.base + it->second.mod;
    else
        return 0;
}

const std::pair<int, int> getStatExperience(const int id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    int a, b;
    if (it != mData.mStats.end())
    {
        a = it->second.exp;
        b = it->second.expNeed;
    }
    else
    {
        a = 0;
        b = 0;
    }
    return std::pair<int, int>(a, b);
}

void setStatExperience(const int id, const int have,
                       const int need, const bool notify)
{
    Stat &stat = mData.mStats[id];

    const int oldExp = stat.exp;
    const int oldExpNeed = stat.expNeed;
    stat.exp = have;
    stat.expNeed = need;
    if (notify)
        triggerStat(id, oldExp, oldExpNeed);
}

// --- Inventory / Equipment --------------------------------------------------

Inventory *getInventory()
{
    return mInventory;
}

Inventory *getStorageInventory()
{
    return inventoryHandler->getStorage();
}

void clearInventory()
{
    if (mEquipment)
        mEquipment->clear();
    if (mInventory)
        mInventory->clear();
}

void setInventoryItem(const int index, const int id,
                      const int amount, const uint8_t refine)
{
    bool equipment = false;
    const ItemType::Type itemType = ItemDB::get(id).getType();
    if (itemType != ItemType::UNUSABLE && itemType != ItemType::USABLE)
        equipment = true;
    if (mInventory)
        mInventory->setItem(index, id, amount, refine, equipment);
}

Equipment *getEquipment()
{
    return mEquipment;
}

Item *getEquipment(const unsigned int slot)
{
    if (mEquipment)
        return mEquipment->getEquipment(slot);
    else
        return nullptr;
}

void setEquipmentBackend(Equipment::Backend *const backend)
{
    if (mEquipment)
        mEquipment->setBackend(backend);
}

void equipItem(const Item *const item, const bool sfx)
{
    if (sfx)
        ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
    inventoryHandler->equipItem(item);
}

void unequipItem(const Item *const item, const bool sfx)
{
    if (sfx)
        ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
    inventoryHandler->unequipItem(item);
}

void useItem(const Item *const item, const bool sfx)
{
    if (sfx)
        ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
    inventoryHandler->useItem(item);
}

void useEquipItem(const Item *const item, const bool sfx)
{
    if (item)
    {
        if (item->isEquipment())
        {
            if (item->isEquipped())
            {
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
                inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                inventoryHandler->useItem(item);
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
            }
        }
    }
}

void useEquipItem2(const Item *const item, const bool sfx)
{
    if (item)
    {
        if (!item->isEquipment())
        {
            if (item->isEquipped())
            {
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
                inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (sfx)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
                inventoryHandler->useItem(item);
            }
        }
    }
}

void dropItem(const Item *const item, const int amount, const bool sfx)
{
    if (item && mProtectedItems.find(item->getId()) == mProtectedItems.end())
    {
        if (sfx)
            ItemSoundManager::playSfx(item, ItemSoundEvent::DROP);
        inventoryHandler->dropItem(item, amount);
    }
}

void pickUpItem(const FloorItem *const item, const bool sfx)
{
    if (sfx)
        ItemSoundManager::playSfx(item, ItemSoundEvent::PICKUP);
    playerHandler->pickUp(item);
}

// --- Misc -------------------------------------------------------------------

void setBackend(const PlayerInfoBackend &backend)
{
    mData = backend;
}

void setCharId(const int charId)
{
    mCharId = charId;
}

int getCharId()
{
    return mCharId;
}

void logic()
{
}

bool isTrading()
{
    return mTrading;
}

void setTrading(const bool trading)
{
    mTrading = trading;
}

void updateAttrs()
{
    const int attackDelay = getStatBase(Attributes::ATTACK_DELAY);
    if (attackDelay)
    {
        setStatBase(Attributes::ATTACK_SPEED,
            getStatBase(Attributes::ATK) * 1000 / attackDelay, false);
        setStatMod(Attributes::ATTACK_SPEED,
            getStatMod(Attributes::ATK) * 1000 / attackDelay, true);
    }
    else
    {
        setStatBase(Attributes::ATTACK_SPEED, 0, false);
        setStatMod(Attributes::ATTACK_SPEED, 0, true);
    }
}

void init()
{
}

void deinit()
{
    clearInventory();
    delete2(mMercenary);
}

void loadData()
{
    mProtectedItems.clear();
    splitToIntSet(mProtectedItems,
        serverConfig.getStringValue("protectedItems"), ',');
}

void clear()
{
    mData.mSkills.clear();
}

bool isTalking()
{
    return NpcDialog::isActive() || NpcPostDialog::isActive()
        || InventoryWindow::isStorageActive();
}

void gameDestroyed()
{
    delete2(mInventory);
    delete2(mEquipment);
}

void stateChange(const State state)
{
    if (state == STATE_GAME)
    {
        if (!mInventory)
        {
            mInventory = new Inventory(Inventory::INVENTORY);
            mEquipment = new Equipment();
        }
    }
}

static void saveProtectedItems()
{
    std::string str;
    std::set<int>::const_iterator it = mProtectedItems.begin();
    std::set<int>::const_iterator it_end = mProtectedItems.end();
    if (it != it_end)
    {
        str.append(toString(*it));
        ++ it;
    }
    while (it != it_end)
    {
        str.append(",").append(toString(*it));
        ++ it;
    }
    serverConfig.setValue("protectedItems", str);
    serverConfig.write();
}

void protectItem(const int id)
{
    mProtectedItems.insert(id);
    saveProtectedItems();
}

void unprotectItem(const int id)
{
    mProtectedItems.erase(id);
    saveProtectedItems();
}

bool isItemProtected(const int id)
{
    return mProtectedItems.find(id) != mProtectedItems.end();
}

void setMercenary(MercenaryInfo *const info)
{
    if (mMercenary)
        delete mMercenary;
    mMercenary = info;
}

void setMercenaryBeing(Being *const being)
{
    if (!being || !mMercenary)
        return;
    being->setName(mMercenary->name);
    being->setOwner(localPlayer);
    being->setLevel(mMercenary->level);
    being->setAttackRange(mMercenary->range);
}

MercenaryInfo *getMercenary()
{
    return mMercenary;
}

void setPet(PetInfo *const info)
{
    if (mPet)
        delete mPet;
    mPet = info;
}

void setPetBeing(Being *const being)
{
    if (!being || !mPet)
        return;
    being->setName(mPet->name);
    being->setOwner(localPlayer);
    being->setLevel(mPet->level);
}

PetInfo *getPet()
{
    return mPet;
}

void setHomunculus(HomunculusInfo *const info)
{
    if (mHomunculus)
        delete mHomunculus;
    mHomunculus = info;
}

void setHomunculusBeing(Being *const being)
{
    if (!being || !mHomunculus)
        return;
    being->setName(mHomunculus->name);
    being->setOwner(localPlayer);
}

HomunculusInfo *getHomunculus()
{
    return mHomunculus;
}

int getHomunculusId()
{
    return mHomunculus ? mHomunculus->id : 0;
}

int getMercenaryId()
{
    return mMercenary ? mMercenary->id : 0;
}

void updateMoveAI()
{
    if (mMercenary)
        mercenaryHandler->moveToMaster();
    if (mHomunculus)
        homunculusHandler->moveToMaster();
}

void updateAttackAi(const int targetId, const bool keep)
{
    if (mMercenary)
        mercenaryHandler->attack(targetId, keep);
    if (mHomunculus)
        homunculusHandler->attack(targetId, keep);
}

void setGuildPositionFlags(const GuildPositionFlags::Type pos)
{
    mGuildPositionFlags = pos;
}

GuildPositionFlags::Type getGuildPositionFlags()
{
    return mGuildPositionFlags;
}

}  // namespace PlayerInfo
