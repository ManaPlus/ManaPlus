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

#ifndef BEING_PLAYERINFO_H
#define BEING_PLAYERINFO_H

#include "equipment.h"

#include <map>
#include <string>


/**
 * Stat information storage structure.
 */
struct Stat
{
    int base;
    int mod;
    int exp;
    int expNeed;
};

typedef std::map<int, int> IntMap;
typedef std::map<int, Stat> StatMap;

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

    IntMap mAttributes;
    StatMap mStats;
    IntMap mSkills;
};

class FloorItem;
class Inventory;
class Item;

/**
 * Special information storage structure.
 */
struct Special final
{
    int currentMana;
    int neededMana;
    int recharge;
};

typedef std::map<int, Special> SpecialsMap;

/**
 * A database like namespace which holds global info about the localplayer
 *
 * NOTE: 'bool notify' is used to determine if a event is to be triggered.
 */
namespace PlayerInfo
{
    /**
     * Standard attributes for players.
     */
    enum Attribute
    {
        LEVEL = 0,
        HP,
        MAX_HP,
        MP,
        MAX_MP,
        EXP,
        EXP_NEEDED,
        MONEY,
        TOTAL_WEIGHT,
        MAX_WEIGHT,
        SKILL_POINTS,
        CHAR_POINTS,
        CORR_POINTS,
        ATTACK_DELAY = 100,
        ATTACK_RANGE = 101,
        WALK_SPEED = 102,
        ATTACK_SPEED = 103
    };

// --- Attributes -------------------------------------------------------------

    /**
     * Returns the value of the given attribute.
     */
    int getAttribute(const int id) A_WARN_UNUSED;

    /**
     * Changes the value of the given attribute.
     */
    void setAttribute(const int id, const int value,
                      const bool notify = true);

    int getSkillLevel(const int id) A_WARN_UNUSED;

    void setSkillLevel(const int id, const int value);

// --- Stats ------------------------------------------------------------------

    /**
     * Returns the base value of the given stat.
     */
    int getStatBase(const int id) A_WARN_UNUSED;

    /**
     * Changes the base value of the given stat.
     */
    void setStatBase(const int id, const int value,
                     const bool notify = true);

    /**
     * Returns the modifier for the given stat.
     */
    int getStatMod(const int id) A_WARN_UNUSED;

    /**
     * Changes the modifier for the given stat.
     */
    void setStatMod(const int id, const int value,
                    const bool notify = true);

    /**
     * Returns the current effective value of the given stat. Effective is base
     * + mod
     */
    int getStatEffective(const int id) A_WARN_UNUSED;

    /**
     * Changes the level of the given stat.
     */
    void setStatLevel(int id, int value, bool notify = true);

    /**
     * Returns the experience of the given stat.
     */
    const std::pair<int, int> getStatExperience(const int id) A_WARN_UNUSED;

    /**
     * Changes the experience of the given stat.
     */
    void setStatExperience(const int id, const int have,
                           const int need, const bool notify = true);

// --- Inventory / Equipment --------------------------------------------------

    /**
     * Returns the player's inventory.
     */
    Inventory *getInventory() A_WARN_UNUSED;

    Inventory *getStorageInventory() A_WARN_UNUSED;

    /**
     * Clears the player's inventory and equipment.
     */
    void clearInventory();

    void clear();

    /**
     * Changes the inventory item at the given slot.
     */
    void setInventoryItem(const int index, const int id,
                          const int amount, const int refine);

    /**
     * Returns the player's equipment.
     */
    Equipment *getEquipment() A_WARN_UNUSED;

    /**
     * Returns the player's equipment at the given slot.
     */
    Item *getEquipment(const unsigned int slot) A_WARN_UNUSED;

// --- Misc -------------------------------------------------------------------

    /**
     * Changes the internal PlayerInfoBackend reference;
     */
    void setBackend(const PlayerInfoBackend &backend);

    void setCharId(const int charId);

    int getCharId();

    /**
     * Does necessary updates every tick.
     */
    void logic();

    /**
     * Returns true if the player is involved in a trade at the moment, false
     * otherwise.
     */
    bool isTrading();

    /**
     * Sets whether the player is currently involved in trade or not.
     */
    void setTrading(const bool trading);

    void updateAttrs();

    /**
     * Initializes some internals.
     */
    void init();

    void deinit();

    void loadData();

    bool isTalking();

    void gameDestroyed();

    void stateChange(const int state);

    void triggerAttr(int id);

    void triggerAttr(const int id, const int old);

    void triggerStat(int id);

    void triggerStat(const int id, const std::string &changed,
                     const int old1, const int old2 = 0);

    void setEquipmentBackend(Equipment::Backend *const backend);

    void equipItem(const Item *const item, const bool sfx);

    void unequipItem(const Item *const item, const bool sfx);

    void useItem(const Item *const item, const bool sfx);

    void useEquipItem(const Item *const item, const bool sfx);

    void useEquipItem2(const Item *const item, const bool sfx);

    void dropItem(const Item *const item, const int amount, const bool sfx);

    void pickUpItem(const FloorItem *const item, const bool sfx);

    void protectItem(const int id);

    void unprotectItem(const int id);

    bool isItemProtected(const int id);

}  // namespace PlayerInfo

#endif  // BEING_PLAYERINFO_H
