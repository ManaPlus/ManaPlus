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

#ifndef RESOURCES_ITEMINFO_H
#define RESOURCES_ITEMINFO_H

#include "being/being.h"

#include "resources/db/colordb.h"

#include <map>

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
    //   this item does not amount to one, it only
    //   indicates the chosen projectile.
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
    ITEM_EQUIPMENT_ARMS,  // 5
    ITEM_EQUIPMENT_HEAD,
    ITEM_EQUIPMENT_LEGS,
    ITEM_EQUIPMENT_SHIELD,
    ITEM_EQUIPMENT_RING,
    ITEM_EQUIPMENT_NECKLACE,  // 10
    ITEM_EQUIPMENT_FEET,
    ITEM_EQUIPMENT_AMMO,
    ITEM_EQUIPMENT_CHARM,
    ITEM_SPRITE_RACE,
    ITEM_SPRITE_HAIR  // 15
};

// sprite, <itemfrom, itemto>
typedef std::map<int, std::map<int, int> > SpriteToItemMap;
typedef SpriteToItemMap::const_iterator SpriteToItemMapCIter;

/**
 * Defines a class for storing item infos. This includes information used when
 * the item is equipped.
 */
class ItemInfo final
{
    public:
        /**
         * Constructor.
         */
        ItemInfo();

        A_DELETE_COPY(ItemInfo)

        ~ItemInfo();

        void setId(const int id)
        { mId = id; }

        int getId() const A_WARN_UNUSED
        { return mId; }

        void setName(const std::string &name)
        { mName = name; }

        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        const std::string getName(const unsigned char color)
                                  const A_WARN_UNUSED;

        void setDisplay(const SpriteDisplay &display)
        { mDisplay = display; }

        const SpriteDisplay &getDisplay() const A_WARN_UNUSED
        { return mDisplay; }

        void setDescription(const std::string &description)
        { mDescription = description; }

        const std::string &getDescription() const A_WARN_UNUSED
        { return mDescription; }

        const std::string getDescription(const unsigned char color)
                                         const A_WARN_UNUSED;

        void setEffect(const std::string &effect)
        { mEffect = effect; }

        const std::string &getEffect() const A_WARN_UNUSED
        { return mEffect; }

        void setType(const ItemType type)
        { mType = type; }

        ItemType getType() const A_WARN_UNUSED
        { return mType; }

        void setWeight(const int weight)
        { mWeight = weight; }

        int getWeight() const A_WARN_UNUSED
        { return mWeight; }

        int getView() const
        { return mView; }

        void setView(const int view)
        { mView = view; }

        void setSprite(const std::string &animationFile,
                       const Gender gender, const int race);

        const std::string &getSprite(const Gender gender,
                                     const int race) const A_WARN_UNUSED;

        void setAttackAction(const std::string &attackAction);

        void setSkyAttackAction(const std::string &attackAction);

        void setWaterAttackAction(const std::string &attackAction);

        // Handlers for seting and getting the string
        // used for particles when attacking
        void setMissileParticleFile(const std::string &s)
        { mMissileParticleFile = s; }

        const std::string &getMissileParticleFile() const A_WARN_UNUSED
        { return mMissileParticleFile; }

        void setHitEffectId(const int s)
        { mHitEffectId = s; }

        int getHitEffectId() const A_WARN_UNUSED
        { return mHitEffectId; }

        void setCriticalHitEffectId(const int s)
        { mCriticalHitEffectId = s; }

        int getCriticalHitEffectId() const A_WARN_UNUSED
        { return mCriticalHitEffectId; }

        void setMissEffectId(const int s)
        { mMissEffectId = s; }

        int getMissEffectId() const A_WARN_UNUSED
        { return mMissEffectId; }

        const std::string &getAttackAction() const
        { return mAttackAction; }

        const std::string &getSkyAttackAction() const
        { return mSkyAttackAction; }

        const std::string &getWaterAttackAction() const
        { return mWaterAttackAction; }

        int getAttackRange() const A_WARN_UNUSED
        { return mAttackRange; }

        void setAttackRange(const int r)
        { mAttackRange = r; }

        void addSound(const ItemSoundEvent event,
                      const std::string &filename,
                      const int delay);

        const SoundInfo &getSound(const ItemSoundEvent event)
                                  const A_WARN_UNUSED;

        int getDrawBefore(const int direction) const A_WARN_UNUSED;

        void setDrawBefore(const int direction, const int n);

        int getDrawAfter(const int direction) const A_WARN_UNUSED;

        void setDrawAfter(const int direction, const int n);

        int getDrawPriority(const int direction) const A_WARN_UNUSED;

        void setDrawPriority(const int direction, const int n);

        static void setSpriteOrder(int *const ptr,
                                   const int direction,
                                   const int n,
                                   const int def = -1);

        const std::map<int, int> &getTags() const A_WARN_UNUSED
        { return mTags; }

        void addTag(const int tag)
        { mTags[tag] = 1; }

        void setRemoveSprites()
        { mIsRemoveSprites = true; }

        bool isRemoveSprites() const A_WARN_UNUSED
        { return mIsRemoveSprites; }

        void setMaxFloorOffset(const int i)
        { maxFloorOffset = i; }

        int getMaxFloorOffset() const A_WARN_UNUSED
        { return maxFloorOffset; }

        bool isRemoveItemId(int id) const A_WARN_UNUSED;

        void setPet(const int pet)
        { mPet = pet; }

        int getPet() const
        { return mPet; }

        /** Effects to be shown when weapon attacks - see also effects.xml */
        std::string mMissileParticleFile;

        int getReplaceToSpriteId(int id) const A_WARN_UNUSED;

        std::map<int, int> *addReplaceSprite(const int sprite,
                                             const int direction);

        const SpriteToItemMap *getSpriteToItemReplaceMap(const int directions)
                                                         const A_WARN_UNUSED;

        std::string getDyeColorsString(const int color) const A_WARN_UNUSED;

        void setColorsList(const std::string &name);

        bool isHaveColors() const A_WARN_UNUSED
        { return !mColorList.empty(); }

        const std::string replaceColors(std::string str,
                                        const unsigned char color)
                                        const A_WARN_UNUSED;

        void setPickupCursor(const std::string &cursor)
        { return setPickupCursor(Cursor::stringToCursor(cursor)); }

        void setPickupCursor(const Cursor::Cursor &cursor)
        { mPickupCursor = cursor; }

        Cursor::Cursor getPickupCursor() const A_WARN_UNUSED
        { return mPickupCursor; }

        void setProtected(const bool b)
        { mProtected = b; }

        bool isProtected() const
        { return mProtected; }

        int getColorsSize() const
        { return mColors ? mColors->size() : 0; }

        std::string getColorName(const int idx) const;

        std::string getColor(const int idx) const;

        int mDrawBefore[10];
        int mDrawAfter[10];
        int mDrawPriority[10];

    protected:
        SpriteDisplay mDisplay;     /**< Display info (like icon) */
        std::string mName;
        std::string mDescription;   /**< Short description. */
        std::string mEffect;        /**< Description of effects. */
        ItemType mType;             /**< Item type. */
        int mWeight;                /**< Weight in grams. */
        int mView;                  /**< Item ID of how this item looks. */
        int mId;                    /**< Item ID */
        bool mIsRemoveSprites;
        // sprite, <itemfrom, itemto> [direction]
        SpriteToItemMap *mSpriteToItemReplaceMap[10];

        std::vector<SpriteToItemMap*> mSpriteToItemReplaceList;

        // Equipment related members.
        /** Attack type, in case of weapon.
         * See SpriteAction in spritedef.h for more info.
         * Attack action sub-types (bow, sword, ...) are defined in items.xml.
         */
        std::string mAttackAction;
        std::string mSkyAttackAction;
        std::string mWaterAttackAction;
        int mAttackRange;     /**< Attack range, will be zero if non weapon. */

        // Particle to be shown when weapon attacks
        std::string mMissileParticle;

        /** Maps gender to sprite filenames. */
        std::map <int, std::string> mAnimationFiles;

        /** Stores the names of sounds to be played at certain event. */
        std::map <ItemSoundEvent, SoundInfoVect> mSounds;
        std::map <int, int> mTags;
        const std::map <int, ColorDB::ItemColor> *mColors;
        std::string mColorList;
        int mHitEffectId;
        int mCriticalHitEffectId;
        int mMissEffectId;
        int maxFloorOffset;
        Cursor::Cursor mPickupCursor;
        int mPet;
        bool mProtected;
};

#endif  // RESOURCES_ITEMINFO_H
