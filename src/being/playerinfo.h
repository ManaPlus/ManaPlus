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

#ifndef BEING_PLAYERINFO_H
#define BEING_PLAYERINFO_H

#include "equipment.h"

#include "enums/guildpositionflags.h"
#include "enums/state.h"

#include "enums/being/attributes.h"

#include "enums/simpletypes/beingid.h"
#include "enums/simpletypes/keep.h"
#include "enums/simpletypes/notify.h"
#include "enums/simpletypes/sfx.h"
#include "enums/simpletypes/trading.h"

#include "utils/intmap.h"

#include <string>

/**
 * Stat information storage structure.
 */
struct Stat final
{
    int base;
    int mod;
    int exp;
    int expNeed;
};

typedef std::map<AttributesT, int> AtrIntMap;
typedef std::map<AttributesT, Stat> StatMap;

/**
 * Backend for core player information.
 */
struct PlayerInfoBackend final
{
    PlayerInfoBackend() :
        mAttributes(),
        mStats(),
        mSkills()
    {
    }

    AtrIntMap mAttributes;
    StatMap mStats;
    IntMap mSkills;
};

class Being;
class FloorItem;
class Inventory;
class Item;

struct HomunculusInfo;
struct MercenaryInfo;
struct PetInfo;

/**
 * A database like namespace which holds global info about the localplayer
 *
 */
namespace PlayerInfo
{
// --- Attributes -------------------------------------------------------------

    /**
     * Returns the value of the given attribute.
     */
    int getAttribute(const AttributesT id) A_WARN_UNUSED;

    /**
     * Changes the value of the given attribute.
     */
    void setAttribute(const AttributesT id, const int value,
                      const Notify notify = Notify_true);

    int getSkillLevel(const int id) A_WARN_UNUSED;

    void setSkillLevel(const int id, const int value);

// --- Stats ------------------------------------------------------------------

    /**
     * Returns the base value of the given stat.
     */
    int getStatBase(const AttributesT id) A_WARN_UNUSED;

    /**
     * Changes the base value of the given stat.
     */
    void setStatBase(const AttributesT id,
                     const int value,
                     const Notify notify = Notify_true);

    /**
     * Returns the modifier for the given stat.
     */
    int getStatMod(const AttributesT id) A_WARN_UNUSED;

    /**
     * Changes the modifier for the given stat.
     */
    void setStatMod(const AttributesT id,
                    const int value,
                    const Notify notify = Notify_true);

    /**
     * Returns the current effective value of the given stat. Effective is base
     * + mod
     */
    int getStatEffective(const AttributesT id) A_WARN_UNUSED;

    /**
     * Changes the level of the given stat.
     */
    void setStatLevel(AttributesT id, int value, Notify notify = Notify_true);

    /**
     * Returns the experience of the given stat.
     */
    const std::pair<int, int> getStatExperience(const AttributesT id)
                                                A_WARN_UNUSED;

    /**
     * Changes the experience of the given stat.
     */
    void setStatExperience(const AttributesT id,
                           const int have,
                           const int need,
                           const Notify notify = Notify_true);

// --- Inventory / Equipment --------------------------------------------------

    /**
     * Returns the player's inventory.
     */
    Inventory *getInventory() A_WARN_UNUSED;

    Inventory *getStorageInventory() A_WARN_UNUSED;

    Inventory *getCartInventory() A_WARN_UNUSED;

    /**
     * Clears the player's inventory and equipment.
     */
    void clearInventory();

    void clear();

    /**
     * Returns the player's equipment.
     */
    Equipment *getEquipment() A_WARN_UNUSED;

    /**
     * Returns the player's equipment at the given slot.
     */
    const Item *getEquipment(const unsigned int slot) A_WARN_UNUSED;

// --- Misc -------------------------------------------------------------------

    /**
     * Changes the internal PlayerInfoBackend reference;
     */
    void setBackend(const PlayerInfoBackend &backend);

    void setCharId(const int charId);

    int getCharId();

    /**
     * Returns true if the player is involved in a trade at the moment, false
     * otherwise.
     */
    Trading isTrading();

    /**
     * Sets whether the player is currently involved in trade or not.
     */
    void setTrading(const Trading trading);

    void updateAttrs();

    /**
     * Initializes some internals.
     */
    void init() A_CONST;

    void deinit();

    void loadData();

    bool isTalking();

    void gameDestroyed();

    void stateChange(const StateT state);

    void triggerAttr(const AttributesT id,
                     const int old);

    void triggerStat(const AttributesT id,
                     const int old1,
                     const int old2);

    void setEquipmentBackend(Equipment::Backend *const backend);

    void equipItem(const Item *const item, const Sfx sfx);

    void unequipItem(const Item *const item, const Sfx sfx);

    void useItem(const Item *const item, const Sfx sfx);

    void useEquipItem(const Item *const item, const Sfx sfx);

    void useEquipItem2(const Item *const item, const Sfx sfx);

    void dropItem(const Item *const item, const int amount, const Sfx sfx);

    void pickUpItem(const FloorItem *const item, const Sfx sfx);

    void protectItem(const int id);

    void unprotectItem(const int id);

    bool isItemProtected(const int id);

    MercenaryInfo *getMercenary();

    void setMercenary(MercenaryInfo *const info);

    void setMercenaryBeing(Being *const being);

    PetInfo *getPet();

    void setPet(PetInfo *const info);

    void setPetBeing(Being *const being);

    BeingId getPetBeingId();

    HomunculusInfo *getHomunculus();

    void setHomunculus(HomunculusInfo *const info);

    void setHomunculusBeing(Being *const being);

    void setElemental(const BeingId id);

    BeingId getHomunculusId();

    BeingId getMercenaryId();

    BeingId getElementalId();

    void updateAttackAi(const BeingId targetId,
                        const Keep keep);

    std::string getRoomName();

    void setRoomName(const std::string &name);

    bool isInRoom();

    void setGuildPositionFlags(const GuildPositionFlags::Type pos);

    GuildPositionFlags::Type getGuildPositionFlags();

    void enableVending(const bool b);

    bool isVending() A_WARN_UNUSED;
}  // namespace PlayerInfo

#endif  // BEING_PLAYERINFO_H
