/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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
#include "settings.h"

#include "being/localplayer.h"

#include "being/homunculusinfo.h"
#include "being/mercenaryinfo.h"
#include "being/petinfo.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"

#include "listeners/statlistener.h"

#include "net/homunculushandler.h"
#include "net/inventoryhandler.h"
#include "net/mercenaryhandler.h"
#include "net/playerhandler.h"

#include "resources/item/item.h"

#include "utils/delete2.h"

#include "utils/translation/translationmanager.h"

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
int mServerLanguage = -1;
std::set<int> mProtectedItems;

// --- Triggers ---------------------------------------------------------------

void triggerAttr(const AttributesT id,
                 const int64_t old)
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

int64_t getAttribute64(const AttributesT id)
{
    const AtrIntMap::const_iterator it = mData.mAttributes.find(id);
    if (it != mData.mAttributes.end())
        return it->second;
    return 0;
}

int32_t getAttribute(const AttributesT id)
{
    const AtrIntMap::const_iterator it = mData.mAttributes.find(id);
    if (it != mData.mAttributes.end())
        return CAST_S32(it->second);
    return 0;
}

void setAttribute(const AttributesT id,
                  const int64_t value,
                  const Notify notify)
{
    const int64_t old = mData.mAttributes[id];
    mData.mAttributes[id] = value;
    if (notify == Notify_true)
        triggerAttr(id, old);
}

int getSkillLevel(const int id)
{
    const IntMap::const_iterator it = mData.mSkills.find(id);
    if (it != mData.mSkills.end())
        return it->second;
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
    return 0;
}

const std::pair<int, int> getStatExperience(const AttributesT id)
{
    const StatMap::const_iterator it = mData.mStats.find(id);
    int a;
    int b;
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

// --- Inventory / Equipment --------------------------------------------------

Inventory *getInventory()
{
    return mInventory;
}

Inventory *getStorageInventory()
{
    if (inventoryHandler != nullptr)
        return inventoryHandler->getStorage();
    return nullptr;
}

Inventory *getCartInventory()
{
    return mCartInventory;
}

void clearInventory()
{
    if (mEquipment != nullptr)
        mEquipment->clear();
    if (mInventory != nullptr)
        mInventory->clear();
}

Equipment *getEquipment()
{
    return mEquipment;
}

const Item *getEquipment(const unsigned int slot)
{
    if (mEquipment != nullptr)
        return mEquipment->getEquipment(slot);
    return nullptr;
}

void setEquipmentBackend(Equipment::Backend *const backend)
{
    if (mEquipment != nullptr)
        mEquipment->setBackend(backend);
}

void equipItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
    if (inventoryHandler != nullptr)
        inventoryHandler->equipItem(item);
}

void unequipItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
    if (inventoryHandler != nullptr)
        inventoryHandler->unequipItem(item);
}

void useItem(const Item *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
    if (inventoryHandler != nullptr)
        inventoryHandler->useItem(item);
}

void useEquipItem(const Item *const item,
                  const int16_t useType,
                  const Sfx sfx)
{
    if (item != nullptr)
    {
        if (item->getType() == ItemType::Card)
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (inventoryHandler != nullptr)
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
                if (inventoryHandler != nullptr)
                    inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                if (inventoryHandler != nullptr)
                    inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (inventoryHandler != nullptr)
                {
                    if (useType == 0)
                        inventoryHandler->useItem(item);
                    else
                        inventoryHandler->useItem(item, useType);
                }
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
            }
        }
    }
}

void useEquipItem2(const Item *const item,
                   const int16_t useType,
                   const Sfx sfx)
{
    if (item != nullptr)
    {
        if (item->isEquipment() == Equipm_false)
        {
            if (item->isEquipped() == Equipped_true)
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::UNEQUIP);
                if (inventoryHandler != nullptr)
                    inventoryHandler->unequipItem(item);
            }
            else
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::EQUIP);
                if (inventoryHandler != nullptr)
                    inventoryHandler->equipItem(item);
            }
        }
        else
        {
            if (mProtectedItems.find(item->getId()) == mProtectedItems.end())
            {
                if (sfx == Sfx_true)
                    ItemSoundManager::playSfx(item, ItemSoundEvent::USE);
                if (inventoryHandler != nullptr)
                {
                    if (useType == 0)
                        inventoryHandler->useItem(item);
                    else
                        inventoryHandler->useItem(item, useType);
                }
            }
        }
    }
}

void dropItem(const Item *const item, const int amount, const Sfx sfx)
{
    if (item != nullptr &&
        mProtectedItems.find(item->getId()) == mProtectedItems.end())
    {
        if (sfx == Sfx_true)
            ItemSoundManager::playSfx(item, ItemSoundEvent::DROP);
        if (inventoryHandler != nullptr)
            inventoryHandler->dropItem(item, amount);
    }
}

void pickUpItem(const FloorItem *const item, const Sfx sfx)
{
    if (sfx == Sfx_true)
        ItemSoundManager::playSfx(item, ItemSoundEvent::PICKUP);
    if (playerHandler != nullptr)
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
    if (attackDelay != 0) \
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
    delete2(mMercenary)
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
    delete2(mInventory)
    delete2(mEquipment)
    delete2(mCartInventory)
}

void stateChange(const StateT state)
{
    if (state == State::GAME)
    {
        if (mInventory == nullptr)
        {
            mInventory = new Inventory(InventoryType::Inventory,
                settings.fixedInventorySize);
            mEquipment = new Equipment;
            mCartInventory = new Inventory(InventoryType::Cart, -1);
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
    if (being == nullptr ||
        mMercenary == nullptr)
    {
        return;
    }
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
    if (being != nullptr)
        mPetBeingId = being->getId();
    else
        mPetBeingId = BeingId_zero;
    if (being == nullptr ||
        mPet == nullptr)
    {
        return;
    }
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
    if (being == nullptr ||
        mHomunculus == nullptr)
    {
        return;
    }
    being->setName(mHomunculus->name);
    being->setOwner(localPlayer);
}

HomunculusInfo *getHomunculus()
{
    return mHomunculus;
}

BeingId getHomunculusId()
{
    return mHomunculus != nullptr ? mHomunculus->id : BeingId_zero;
}

BeingId getMercenaryId()
{
    return mMercenary != nullptr ? mMercenary->id : BeingId_zero;
}

void updateAttackAi(const BeingId targetId,
                    const Keep keep)
{
    if (mMercenary != nullptr &&
        mercenaryHandler != nullptr)
    {
        mercenaryHandler->attack(targetId, keep);
    }
    if (mHomunculus != nullptr &&
        homunculusHandler != nullptr)
    {
        homunculusHandler->attack(targetId, keep);
    }
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

void setServerLanguage(const int lang)
{
    if (lang != mServerLanguage)
    {
        mServerLanguage = lang;
        TranslationManager::loadDictionaryLang();
    }
}

int getServerLanguage()
{
    return mServerLanguage;
}

}  // namespace PlayerInfo
