/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_ITEMINFO_H
#define RESOURCES_ITEMINFO_H

#include "enums/being/gender.h"

#include "enums/resources/item/itemdbtype.h"

#include "enums/simpletypes/beingtypeid.h"

#include "resources/cursors.h"
#include "resources/itemcolordata.h"
#include "resources/missileinfo.h"
#include "resources/soundinfo.h"

#include "resources/sprite/spritedisplay.h"

#include "utils/cast.h"
#include "utils/intmap.h"

struct ItemMenuItem;

// sprite, <itemfrom, itemto>
typedef std::map<int, IntMap> SpriteToItemMap;
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

        void setId(const int id) noexcept2
        { mId = id; }

        int getId() const noexcept2 A_WARN_UNUSED
        { return mId; }

        void setName(const std::string &name) noexcept2
        { mName = name; }

        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        const std::string getName(const ItemColor color)
                                  const A_WARN_UNUSED;

        void setNameEn(const std::string &name) noexcept2
        { mNameEn = name; }

        const std::string &getNameEn() const noexcept2 A_WARN_UNUSED
        { return mNameEn; }

        const std::string getNameEn(const ItemColor color)
                                    const A_WARN_UNUSED;

        const std::string getLink() const A_WARN_UNUSED;

        void setDisplay(const SpriteDisplay &display) noexcept2
        { mDisplay = display; }

        const SpriteDisplay &getDisplay() const noexcept2 A_WARN_UNUSED
        { return mDisplay; }

        void setDescription(const std::string &description) noexcept2
        { mDescription = description; }

        const std::string &getDescription() const noexcept2 A_WARN_UNUSED
        { return mDescription; }

        const std::string getDescription(const ItemColor color)
                                         const A_WARN_UNUSED;

        void setEffect(const std::string &effect) noexcept2
        { mEffect = effect; }

        const std::string &getEffect() const noexcept2 A_WARN_UNUSED
        { return mEffect; }

        void setType(const ItemDbTypeT type) noexcept2
        { mType = type; }

        void setUseButton(const std::string &str) noexcept2
        { mUseButton = str; }

        const std::string &getUseButton() const noexcept2 A_WARN_UNUSED
        { return mUseButton; }

        void setUseButton2(const std::string &str) noexcept2
        { mUseButton2 = str; }

        const std::string &getUseButton2() const noexcept2 A_WARN_UNUSED
        { return mUseButton2; }

        ItemDbTypeT getType() const noexcept2 A_WARN_UNUSED
        { return mType; }

        void setWeight(const int weight) noexcept2
        { mWeight = weight; }

        int getWeight() const noexcept2 A_WARN_UNUSED
        { return mWeight; }

        int getView() const noexcept2 A_WARN_UNUSED
        { return mView; }

        void setView(const int view) noexcept2
        { mView = view; }

        void setSprite(const std::string &animationFile,
                       const GenderT gender, const int race);

        const std::string &getSprite(const GenderT gender,
                                     const BeingTypeId race)
                                     const A_WARN_UNUSED;

        void setAttackAction(const std::string &attackAction);

        void setSkyAttackAction(const std::string &attackAction);

        void setWaterAttackAction(const std::string &attackAction);

        void setRideAttackAction(const std::string &attackAction);

        // Handlers for seting and getting the string
        // used for particles when attacking
        void setMissileParticleFile(const std::string &s) noexcept2
        { mMissile.particle = s; }

        MissileInfo &getMissile() noexcept2 A_WARN_UNUSED
        { return mMissile; }

        const MissileInfo &getMissileConst() const noexcept2 A_WARN_UNUSED
        { return mMissile; }

        void setHitEffectId(const int s) noexcept2
        { mHitEffectId = s; }

        int getHitEffectId() const noexcept2 A_WARN_UNUSED
        { return mHitEffectId; }

        void setCriticalHitEffectId(const int s) noexcept2
        { mCriticalHitEffectId = s; }

        int getCriticalHitEffectId() const noexcept2 A_WARN_UNUSED
        { return mCriticalHitEffectId; }

        void setMissEffectId(const int s) noexcept2
        { mMissEffectId = s; }

        int getMissEffectId() const noexcept2 A_WARN_UNUSED
        { return mMissEffectId; }

        const std::string &getAttackAction() const noexcept2 A_WARN_UNUSED
        { return mAttackAction; }

        const std::string &getSkyAttackAction() const noexcept2 A_WARN_UNUSED
        { return mSkyAttackAction; }

        const std::string &getWaterAttackAction() const noexcept2 A_WARN_UNUSED
        { return mWaterAttackAction; }

        const std::string &getRideAttackAction() const noexcept2 A_WARN_UNUSED
        { return mRideAttackAction; }

        int getAttackRange() const noexcept2 A_WARN_UNUSED
        { return mAttackRange; }

        void setAttackRange(const int r) noexcept2
        { mAttackRange = r; }

        void addSound(const ItemSoundEvent::Type event,
                      const std::string &filename,
                      const int delay);

        const SoundInfo &getSound(const ItemSoundEvent::Type event)
                                  const A_WARN_UNUSED;

        int getDrawBefore(const int direction) const A_WARN_UNUSED;

        void setDrawBefore(const int direction, const int n);

        int getDrawAfter(const int direction) const A_WARN_UNUSED;

        void setDrawAfter(const int direction, const int n);

        int getDrawPriority(const int direction) const A_WARN_UNUSED;

        void setDrawPriority(const int direction, const int n);

        const IntMap &getTags() const noexcept2 A_WARN_UNUSED
        { return mTags; }

        void addTag(const int tag)
        { mTags[tag] = 1; }

        void setRemoveSprites() noexcept2
        { mIsRemoveSprites = true; }

        bool isRemoveSprites() const noexcept2 A_WARN_UNUSED
        { return mIsRemoveSprites; }

        void setMaxFloorOffsetX(const int i) noexcept2
        { maxFloorOffsetX = i; }

        void setMaxFloorOffsetY(const int i) noexcept2
        { maxFloorOffsetY = i; }

        int getMaxFloorOffsetX() const noexcept2 A_WARN_UNUSED
        { return maxFloorOffsetX; }

        int getMaxFloorOffsetY() const noexcept2 A_WARN_UNUSED
        { return maxFloorOffsetY; }

        bool isRemoveItemId(int id) const A_WARN_UNUSED;

        void setCardColor(const ItemColor color) noexcept2
        { mCardColor = color; }

        ItemColor getCardColor() const noexcept2
        { return mCardColor; }

        int getReplaceToSpriteId(int id) const A_WARN_UNUSED;

        IntMap *addReplaceSprite(const int sprite,
                                 const int direction);

        const SpriteToItemMap *getSpriteToItemReplaceMap(const int directions)
                                                         const A_WARN_UNUSED;

        std::string getDyeColorsString(const ItemColor color)
                                       const A_WARN_UNUSED;

        std::string getDyeIconColorsString(const ItemColor color)
                                           const A_WARN_UNUSED;

        void setColorsList(const std::string &name);

        void setIconColorsList(const std::string &name);

        bool isHaveColors() const noexcept2 A_WARN_UNUSED
        { return !mColorsListName.empty(); }

        bool isHaveIconColors() const noexcept2 A_WARN_UNUSED
        { return !mIconColorsListName.empty(); }

        std::string getColorsListName() const noexcept2 A_WARN_UNUSED
        { return mColorsListName; }

        std::string getIconColorsListName() const noexcept2 A_WARN_UNUSED
        { return mIconColorsListName; }

        const std::string replaceColors(std::string str,
                                        const ItemColor color)
                                        const A_WARN_UNUSED;

        void setPickupCursor(const std::string &cursor)
        { return setPickupCursor(Cursors::stringToCursor(cursor)); }

        void setPickupCursor(const CursorT &cursor) noexcept2
        { mPickupCursor = cursor; }

        CursorT getPickupCursor() const noexcept2 A_WARN_UNUSED
        { return mPickupCursor; }

        void setProtected(const bool b) noexcept2
        { mProtected = b; }

        bool isProtected() const noexcept2 A_WARN_UNUSED
        { return mProtected; }

        int getColorsSize() const noexcept2 A_WARN_UNUSED
        { return mColorsList != nullptr ? CAST_S32(mColorsList->size()) : 0; }

        int getIconColorsSize() const noexcept2 A_WARN_UNUSED
        {
            return mIconColorsList != nullptr ?
                CAST_S32(mIconColorsList->size()) : 0;
        }

        std::string getColorName(const ItemColor idx) const;
        std::string getColor(const ItemColor idx) const;

        std::string getIconColorName(const ItemColor idx) const;
        std::string getIconColor(const ItemColor idx) const;

        STD_VECTOR<ItemMenuItem> &getInventoryMenu()
        { return mInventoryMenu; }
        STD_VECTOR<ItemMenuItem> &getStorageMenu()
        { return mStorageMenu; }
        STD_VECTOR<ItemMenuItem> &getCartMenu()
        { return mCartMenu; }

        const STD_VECTOR<ItemMenuItem> &getInventoryMenuConst() const A_CONST
        { return mInventoryMenu; }
        const STD_VECTOR<ItemMenuItem> &getStorageMenuConst() const A_CONST
        { return mStorageMenu; }
        const STD_VECTOR<ItemMenuItem> &getCartMenuConst() const A_CONST
        { return mCartMenu; }

        int mDrawBefore[10];
        int mDrawAfter[10];
        int mDrawPriority[10];

    private:
        static void setSpriteOrder(int *const ptr,
                                   const int direction,
                                   const int n,
                                   const int def = -1) A_NONNULL(1);

        SpriteDisplay mDisplay;     /**< Display info (like icon) */
        MissileInfo mMissile;
        std::string mName;
        std::string mNameEn;
        std::string mDescription;   /**< Short description. */
        std::string mEffect;        /**< Description of effects. */
        std::string mUseButton;
        std::string mUseButton2;
        ItemDbTypeT mType;          /**< Item type. */
        int mWeight;                /**< Weight in grams. */
        int mView;                  /**< Item ID of how this item looks. */
        int mId;                    /**< Item ID */
        bool mIsRemoveSprites;
        // sprite, <itemfrom, itemto> [direction]
        SpriteToItemMap *mSpriteToItemReplaceMap[10];

        STD_VECTOR<SpriteToItemMap*> mSpriteToItemReplaceList;

        // Equipment related members.
        /** Attack type, in case of weapon.
         * See SpriteAction in spritedef.h for more info.
         * Attack action sub-types (bow, sword, ...) are defined in items.xml.
         */
        std::string mAttackAction;
        std::string mSkyAttackAction;
        std::string mWaterAttackAction;
        std::string mRideAttackAction;
        int mAttackRange;     /**< Attack range, will be zero if non weapon. */

        // Particle to be shown when weapon attacks
        std::string mMissileParticle;

        /** Maps gender to sprite filenames. */
        std::map <int, std::string> mAnimationFiles;

        /** Stores the names of sounds to be played at certain event. */
        std::map <ItemSoundEvent::Type, SoundInfoVect> mSounds;
        std::map <int, int> mTags;
        const std::map <ItemColor, ItemColorData> *mColorsList;
        const std::map <ItemColor, ItemColorData> *mIconColorsList;
        STD_VECTOR<ItemMenuItem> mInventoryMenu;
        STD_VECTOR<ItemMenuItem> mStorageMenu;
        STD_VECTOR<ItemMenuItem> mCartMenu;
        std::string mColorsListName;
        std::string mIconColorsListName;
        ItemColor mCardColor;
        int mHitEffectId;
        int mCriticalHitEffectId;
        int mMissEffectId;
        int maxFloorOffsetX;
        int maxFloorOffsetY;
        CursorT mPickupCursor;
        bool mProtected;
};

#endif  // RESOURCES_ITEMINFO_H
