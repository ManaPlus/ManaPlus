/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "being/playerinfo.h"

#include "configuration.h"
#include "itemsoundmanager.h"

#include "being/localplayer.h"

#include "being/homunculusinfo.h"
#include "being/mercenaryinfo.h"
#include "being/petinfo.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"

#include "net/homunculushandler.h"
#include "net/inventoryhandler.h"
#include "net/mercenaryhandler.h"
#include "net/playerhandler.h"

#include "resources/item/item.h"

#include "utils/delete2.h"

#include "debug.h"

namespace PlayerInfo
{

PlayerInfoBackend mData;
int mCharId = 0;

Inventory *mInventory = nullptr;
Inventory *mCartInventory = nullptr;
MercenaryInfo *mMercenary = nullptr;
HomunculusInfo *mHomunculus = nullptr;
PetInfo *mPet = nullptr;
std::string mRoomName;
Equipment *mEquipment = nullptr;
BeingId mPetBeingId = BeingId_zero;
GuildPositionFlags::Type mGuildPositionFlags = GuildPositionFlags::None;
BeingId mElementalId = BeingId_zero;

Trading mTrading = Trading_false;
bool mVending = false;
int mLevelProgress = 0;
std::set<int> mProtectedItems;

// --- Triggers ---------------------------------------------------------------

void triggerAttr(const AttributesT id,
                 const int old)
{
    AttributeListener::distributeEvent(id, old,
        mData.mAttributes.find(id)->second);
}

void triggerStat(const AttributesT id,
                 const int old1,
                 const int old2)
{
    StatListener::distributeEvent(id, old1, old2);
}

// --- Attributes -------------------------------------------------------------

int getAttribute(const AttributesT id)
{
    const AtrIntMap::const_iterator it = mData.mAttributes.find(id);
    if (it != mData.mAttributes.end())
        return it->second;
    else
        return 0;
}

void setAttribute(const AttributesT id,
                  const int value,
                  const Notify notify)
{
    const int old = mData.mAttributes[id];
    mData.mAttributes[id] = value;
    if (notify == Notify_true)
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

int getStatBase(const AttributesT id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.base;
    else
        return 0;
}

void setStatBase(const AttributesT id, const int value, const Notify notify)
{
    const int old = mData.mStats[id].base;
    mData.mStats[id].base = value;
    if (notify == Notify_true)
        triggerStat(id, old, 0);
}

int getStatMod(const AttributesT id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.mod;
    else
        return 0;
}

void setStatMod(const AttributesT id, const int value, const Notify notify)
{
    const int old = mData.mStats[id].mod;
    mData.mStats[id].mod = value;
    if (notify == Notify_true)
        triggerStat(id, old, 0);
}

int getStatEffective(const AttributesT id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    if (it != mData.mStats.end())
        return it->second.base + it->second.mod;
    else
        return 0;
}

const std::pair<int, int> getStatExperience(const AttributesT id)
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

void setStatExperience(const AttributesT id,
                       const int have,
                       const int need,
                       const Notify notify)
{
    Stat &stat = mData.mStats[id];

    const int oldExp = stat.exp;
    const int oldExpNeed = stat.expNeed;
    stat.exp = have;
    stat.expNeed = need;
    if (notify == Notify_true)
        triggerStat(id, oldExp, oldExpNeed);
}

// --- Inventory / Equipment --------------------------------------------------

Inventory *getInventory()
{
    return mInventory;
}

Inventory *getStorageInventory()
{
    if (inventoryHandler)
        return inventoryHandler->getStorage();
    return nullptr;
}

Inventory *getCartInventory()
{
    return mCartInventory;
}

void clearInventory()
{
    if (mEquipment)
        mEquipment->clear();
    if (mInventory)
        mInventory->clear();
}

Equipment *getEquipment()
{
    return mEquipment;
}

const Item *getEquipment(const unsigned int slot)
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

void equipItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
    if (inventoryHandler)
        inventoryHandler->equipItem(item);
}

void unequipItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
    if (inventoryHandler)
        inventoryHandler->unequipItem(item);
}

void useItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
    if (inventoryHandler)
        inventoryHandler->useItem(item);
}

void useEquipItem(const Item *const item, const Sfx sfx)
{
    if (item)
    {
        if (item->getType() == ItemType::Card)
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (inventoryHandler)
                    inventoryHandler->useCard(item);
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USECARD);
            }
        }
        else if (item->isEquipment() == Equipm_true)
        {
            if (item->isEquipped() == Equipped_true)
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
                if (inventoryHandler)
                    inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                if (inventoryHandler)
                    inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (inventoryHandler)
                    inventoryHandler->useItem(item);
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
            }
        }
    }
}

void useEquipItem2(const Item *const item, const Sfx sfx)
{
    if (item)
    {
        if (item->isEquipment() == Equipm_false)
        {
            if (item->isEquipped() == Equipped_true)
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
                if (inventoryHandler)
                    inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                if (inventoryHandler)
                    inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
                if (inventoryHandler)
                    inventoryHandler->useItem(item);
            }
        }
    }
}

void dropItem(const Item *const item, const int amount, const Sfx sfx)
{
    if (item && mProtectedItems.find(item->getId()) == mProtectedItems.end())
    {
        if (sfx == Sfx_true)
            ItemSoundManager::playSfx(item, ItemSoundEvent::DROP);
        if (inventoryHandler)
            inventoryHandler->dropItem(item, amount);
    }
}

void pickUpItem(const FloorItem *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::PICKUP);
    if (playerHandler)
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

Trading isTrading()
{
    return mTrading;
}

void setTrading(const Trading trading)
{
    mTrading = trading;
}

#define updateAttackStat(atk, delay, speed) \
    attackDelay = getStatBase(delay); \
    if (attackDelay) \
    { \
        setStatBase(speed, \
            getStatBase(atk) * 1000 / attackDelay, \
            Notify_false); \
        setStatMod(speed, \
            getStatMod(atk) * 1000 / attackDelay, \
            Notify_true); \
    } \
    else \
    { \
        setStatBase(speed, 0, \
            Notify_false); \
        setStatMod(speed, 0, \
            Notify_true); \
    }

void updateAttrs()
{
    int attackDelay;
    updateAttackStat(Attributes::PLAYER_ATK,
        Attributes::PLAYER_ATTACK_DELAY,
        Attributes::PLAYER_ATTACK_SPEED)
    updateAttackStat(Attributes::HOMUN_ATK,
        Attributes::HOMUN_ATTACK_DELAY,
        Attributes::HOMUN_ATTACK_SPEED)
    updateAttackStat(Attributes::MERC_ATK,
        Attributes::MERC_ATTACK_DELAY,
        Attributes::MERC_ATTACK_SPEED)
}

void init()
{
}

void deinit()
{
    clearInventory();
    delete2(mMercenary);
    mPetBeingId = BeingId_zero;
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
    mPetBeingId = BeingId_zero;
}

bool isTalking()
{
    return NpcDialog::isActive() || InventoryWindow::isStorageActive();
}

void gameDestroyed()
{
    delete2(mInventory);
    delete2(mEquipment);
    delete2(mCartInventory);
}

void stateChange(const StateT state)
{
    if (state == State::GAME)
    {
        if (!mInventory)
        {
            mInventory = new Inventory(InventoryType::Inventory);
            mEquipment = new Equipment;
            mCartInventory = new Inventory(InventoryType::Cart);
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

void setElemental(const BeingId id)
{
    mElementalId = id;
}

BeingId getElementalId()
{
    return mElementalId;
}

MercenaryInfo *getMercenary()
{
    return mMercenary;
}

void setPet(PetInfo *const info)
{
    delete mPet;
    mPet = info;
}

void setPetBeing(Being *const being)
{
    if (being)
        mPetBeingId = being->getId();
    else
        mPetBeingId = BeingId_zero;
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

BeingId getPetBeingId()
{
    return mPetBeingId;
}

void setHomunculus(HomunculusInfo *const info)
{
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

BeingId getHomunculusId()
{
    return mHomunculus ? mHomunculus->id : BeingId_zero;
}

BeingId getMercenaryId()
{
    return mMercenary ? mMercenary->id : BeingId_zero;
}

void updateAttackAi(const BeingId targetId,
                    const Keep keep)
{
    if (mMercenary && mercenaryHandler)
        mercenaryHandler->attack(targetId, keep);
    if (mHomunculus && homunculusHandler)
        homunculusHandler->attack(targetId, keep);
}

std::string getRoomName()
{
    return mRoomName;
}

void setRoomName(const std::string &name)
{
    mRoomName = name;
}

bool isInRoom()
{
    return !mRoomName.empty();
}

void setGuildPositionFlags(const GuildPositionFlags::Type pos)
{
    mGuildPositionFlags = pos;
}

GuildPositionFlags::Type getGuildPositionFlags()
{
    return mGuildPositionFlags;
}

void enableVending(const bool b)
{
    mVending = b;
}

bool isVending()
{
    return mVending;
}

}  // namespace PlayerInfo
