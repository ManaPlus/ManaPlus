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

#ifndef ITEMINFO_H
#define ITEMINFO_H

#include "being.h"

#include "resources/colordb.h"
#include "resources/spritedef.h"

#include <map>
#include <string>
#include <vector>
#include <set>

enum EquipmentSoundEvent
{
    EQUIP_EVENT_STRIKE = 0,
    EQUIP_EVENT_HIT
};

enum EquipmentSlot
{
    // Equipment rules:
    // 1 Brest equipment
    EQUIP_TORSO_SLOT = 0,
    // 1 arms equipment
    EQUIP_ARMS_SLOT = 1,
    // 1 head equipment
    EQUIP_HEAD_SLOT = 2,
    // 1 legs equipment
    EQUIP_LEGS_SLOT = 3,
    // 1 feet equipment
    EQUIP_FEET_SLOT = 4,
    // 2 rings
    EQUIP_RING1_SLOT = 5,
    EQUIP_RING2_SLOT = 6,
    // 1 necklace
    EQUIP_NECKLACE_SLOT = 7,
    // Fight:
    //   2 one-handed weapons
    //   or 1 two-handed weapon
    //   or 1 one-handed weapon + 1 shield.
    EQUIP_FIGHT1_SLOT = 8,
    EQUIP_FIGHT2_SLOT = 9,
    // Projectile:
    //   this item does not amount to one, it only indicates the chosen projectile.
    EQUIP_PROJECTILE_SLOT = 10
};


/**
 * Enumeration of available Item types.
 */
enum ItemType
{
    ITEM_UNUSABLE = 0,
    ITEM_USABLE,
    ITEM_EQUIPMENT_ONE_HAND_WEAPON,
    ITEM_EQUIPMENT_TWO_HANDS_WEAPON,
    ITEM_EQUIPMENT_TORSO,
    ITEM_EQUIPMENT_ARMS, // 5
    ITEM_EQUIPMENT_HEAD,
    ITEM_EQUIPMENT_LEGS,
    ITEM_EQUIPMENT_SHIELD,
    ITEM_EQUIPMENT_RING,
    ITEM_EQUIPMENT_NECKLACE, // 10
    ITEM_EQUIPMENT_FEET,
    ITEM_EQUIPMENT_AMMO,
    ITEM_EQUIPMENT_CHARM,
    ITEM_SPRITE_RACE,
    ITEM_SPRITE_HAIR // 15
};

// sprite, <itemfrom, itemto>
typedef std::map<int, std::map<int, int> > SpriteToItemMap;

/**
 * Defines a class for storing item infos. This includes information used when
 * the item is equipped.
 */
class ItemInfo
{
    public:
        /**
         * Constructor.
         */
        ItemInfo();

        ItemInfo(ItemInfo &info);

        ~ItemInfo();

        void setId(int id)
        { mId = id; }

        int getId() const
        { return mId; }

        void setName(const std::string &name)
        { mName = name; }

        const std::string &getName() const
        { return mName; }

        const std::string getName(unsigned char color) const;

        void setParticleEffect(const std::string &particleEffect)
        { mParticle = particleEffect; }

        std::string getParticleEffect() const
        { return mParticle; }

        void setDisplay(SpriteDisplay display)
        { mDisplay = display; }

        const SpriteDisplay &getDisplay() const
        { return mDisplay; }

        void setDescription(const std::string &description)
        { mDescription = description; }

        const std::string &getDescription() const
        { return mDescription; }

        const std::string getDescription(unsigned char color) const;

        void setEffect(const std::string &effect)
        { mEffect = effect; }

        const std::string &getEffect() const { return mEffect; }

        void setType(ItemType type)
        { mType = type; }

        ItemType getType() const
        { return mType; }

        void setWeight(int weight)
        { mWeight = weight; }

        int getWeight() const
        { return mWeight; }

        int getView() const
        { return mView; }

        void setView(int view)
        { mView = view; }

        void setSprite(const std::string &animationFile, Gender gender)
        { mAnimationFiles[gender] = animationFile; }

        const std::string &getSprite(Gender gender) const;

        void setAttackAction(std::string attackAction);

        // Handlers for seting and getting the string used for particles when attacking
        void setMissileParticleFile(const std::string &s)
        { mMissileParticleFile = s; }

        const std::string &getMissileParticleFile() const
        { return mMissileParticleFile; }

        void setHitEffectId(int s)
        { mHitEffectId = s; }

        int getHitEffectId() const
        { return mHitEffectId; }

        void setCriticalHitEffectId(int s)
        { mCriticalHitEffectId = s; }

        int getCriticalHitEffectId() const
        { return mCriticalHitEffectId; }

        const std::string &getAttackAction() const
        { return mAttackAction; }

        int getAttackRange() const
        { return mAttackRange; }

        void setAttackRange(int r)
        { mAttackRange = r; }

        void addSound(EquipmentSoundEvent event, const std::string &filename);

        const std::string &getSound(EquipmentSoundEvent event) const;

        int getDrawBefore(int direction) const;

        void setDrawBefore(int direction, int n);

        int getDrawAfter(int direction) const;

        void setDrawAfter(int direction, int n);

        int getDrawPriority(int direction) const;

        void setDrawPriority(int direction, int n);

        void setSpriteOrder(int *ptr, int direction, int n, int def = -1);

        std::map<int, int> getTags() const
        { return mTags; }

        void addTag(int tag)
        { mTags[tag] = 1; }

        void setRemoveSprites()
        { mIsRemoveSprites = true; }

        bool isRemoveSprites() const
        { return mIsRemoveSprites; }

        bool isRemoveItemId(int id) const;

        /** Effects to be shown when weapon attacks - see also effects.xml */
        std::string mMissileParticleFile;

        int getReplaceToSpriteId(int id) const;

        std::map<int, int> *addReplaceSprite(int sprite, int direction);

        SpriteToItemMap *getSpriteToItemReplaceMap(int directions) const;

        std::string getDyeString(int color) const;

        std::string getDyeColorsString(int color) const;

        void setColorsList(std::string name);

        bool isHaveColors()
        { return !mColorList.empty(); }

        const std::string replaceColors(std::string str,
                                        unsigned char color) const;

        int mDrawBefore[9];
        int mDrawAfter[9];
        int mDrawPriority[9];

    protected:
        SpriteDisplay mDisplay;     /**< Display info (like icon) */
        std::string mName;
        std::string mDescription;   /**< Short description. */
        std::string mEffect;        /**< Description of effects. */
        ItemType mType;             /**< Item type. */
        std::string mParticle;      /**< Particle effect used with this item */
        int mWeight;                /**< Weight in grams. */
        int mView;                  /**< Item ID of how this item looks. */
        int mId;                    /**< Item ID */
        bool mIsRemoveSprites;
        // sprite, <itemfrom, itemto> [direction]
        SpriteToItemMap *mSpriteToItemReplaceMap[9];

        std::vector<SpriteToItemMap*> mSpriteToItemReplaceList;

        // Equipment related members.
        /** Attack type, in case of weapon.
         * See SpriteAction in spritedef.h for more info.
         * Attack action sub-types (bow, sword, ...) are defined in items.xml.
         */
        std::string mAttackAction;
        int mAttackRange;     /**< Attack range, will be zero if non weapon. */

        // Particle to be shown when weapon attacks
        std::string mMissileParticle;

        /** Maps gender to sprite filenames. */
        std::map <int, std::string> mAnimationFiles;

        /** Stores the names of sounds to be played at certain event. */
        std::map < EquipmentSoundEvent, std::vector<std::string> > mSounds;
        std::map <int, int> mTags;
        std::map <int, ColorDB::ItemColor> *mColors;
        std::string mColorList;
        int mHitEffectId;
        int mCriticalHitEffectId;
};

#endif
