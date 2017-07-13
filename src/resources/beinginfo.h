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

#ifndef RESOURCES_BEINGINFO_H
#define RESOURCES_BEINGINFO_H

#include "enums/being/targetcursorsize.h"

#include "enums/resources/map/blocktype.h"

#include "enums/simpletypes/beingtypeid.h"
#include "enums/simpletypes/itemcolor.h"

#include "resources/beingmenuitem.h"
#include "resources/cursors.h"
#include "resources/soundinfo.h"

#include "resources/sprite/spritedisplay.h"

struct Attack;

class ItemColorData;

typedef std::map<int, Attack*> Attacks;

/**
 * Holds information about a certain type of monster. This includes the name
 * of the monster, the sprite to display and the sounds the monster makes.
 *
 * @see MonsterDB
 * @see NPCDB
 */
class BeingInfo final
{
    public:
        static BeingInfo *unknown;
        static Attack *empty;

        BeingInfo();

        A_DELETE_COPY(BeingInfo)

        ~BeingInfo();

        void setName(const std::string &name)
        { mName = name; }

        const std::string &getName() const noexcept2 A_WARN_UNUSED
        { return mName; }

        void setDisplay(const SpriteDisplay &display);

        const SpriteDisplay &getDisplay() const noexcept2 A_WARN_UNUSED
        { return mDisplay; }

        void setTargetCursorSize(const std::string &size);

        void setTargetCursorSize(const TargetCursorSizeT &targetSize)
        { mTargetCursorSize = targetSize; }

        void setHoverCursor(const std::string &name)
        { return setHoverCursor(Cursors::stringToCursor(name)); }

        void setHoverCursor(const CursorT &cursor)
        { mHoverCursor = cursor; }

        CursorT getHoverCursor() const noexcept2 A_WARN_UNUSED
        { return mHoverCursor; }

        TargetCursorSizeT getTargetCursorSize() const noexcept2 A_WARN_UNUSED
        { return mTargetCursorSize; }

        void addSound(const ItemSoundEvent::Type event,
                      const std::string &filename,
                      const int delay);

        const SoundInfo &getSound(const ItemSoundEvent::Type event)
                                  const A_WARN_UNUSED;

        void addAttack(const int id,
                       const std::string &action,
                       const std::string &skyAttack,
                       const std::string &waterAttack,
                       const std::string &rideAttack,
                       const int effectId,
                       const int hitEffectId,
                       const int criticalHitEffectId,
                       const int missEffectId,
                       const std::string &missileParticle);

        const Attack *getAttack(const int id) const A_WARN_UNUSED;

        void setBlockWalkMask(const unsigned char mask)
        { mBlockWalkMask = mask; }

        /**
         * Gets the way the being is blocked by other objects
         */
        unsigned char getBlockWalkMask() const noexcept2 A_WARN_UNUSED
        { return mBlockWalkMask; }

        void setBlockType(const BlockTypeT &blockType)
        { mBlockType = blockType; }

        BlockTypeT getBlockType() const noexcept2 A_WARN_UNUSED
        { return mBlockType; }

        void setTargetOffsetX(const int n)
        { mTargetOffsetX = n; }

        int getTargetOffsetX() const noexcept2 A_WARN_UNUSED
        { return mTargetOffsetX; }

        void setTargetOffsetY(const int n)
        { mTargetOffsetY = n; }

        int getTargetOffsetY() const noexcept2 A_WARN_UNUSED
        { return mTargetOffsetY; }

        void setNameOffsetX(const int n)
        { mNameOffsetX = n; }

        int getNameOffsetX() const noexcept2 A_WARN_UNUSED
        { return mNameOffsetX; }

        void setNameOffsetY(const int n)
        { mNameOffsetY = n; }

        int getNameOffsetY() const noexcept2 A_WARN_UNUSED
        { return mNameOffsetY; }

        void setHpBarOffsetX(const int n)
        { mHpBarOffsetX = n; }

        int getHpBarOffsetX() const noexcept2 A_WARN_UNUSED
        { return mHpBarOffsetX; }

        void setHpBarOffsetY(const int n)
        { mHpBarOffsetY = n; }

        int getHpBarOffsetY() const noexcept2 A_WARN_UNUSED
        { return mHpBarOffsetY; }

        void setMaxHP(const int n)
        { mMaxHP = n; }

        int getMaxHP() const noexcept2 A_WARN_UNUSED
        { return mMaxHP; }

        bool isStaticMaxHP() const noexcept2 A_WARN_UNUSED
        { return mStaticMaxHP; }

        void setStaticMaxHP(const bool n)
        { mStaticMaxHP = n; }

        void setTargetSelection(const bool n)
        { mTargetSelection = n; }

        bool isTargetSelection() const noexcept2 A_WARN_UNUSED
        { return mTargetSelection; }

        int getSortOffsetY() const noexcept2 A_WARN_UNUSED
        { return mSortOffsetY; }

        void setSortOffsetY(const int n)
        { mSortOffsetY = n; }

        int getDeadSortOffsetY() const noexcept2 A_WARN_UNUSED
        { return mDeadSortOffsetY; }

        void setDeadSortOffsetY(const int n)
        { mDeadSortOffsetY = n; }

        BeingTypeId getAvatarId() const noexcept2 A_WARN_UNUSED
        { return mAvatarId; }

        void setAvatarId(const BeingTypeId id)
        { mAvatarId = id; }

        int getWidth() const noexcept2 A_WARN_UNUSED
        { return mWidth; }

        int getHeight() const noexcept2 A_WARN_UNUSED
        { return mHeight; }

        void setWidth(const int n)
        { mWidth = n; }

        void setHeight(const int n)
        { mHeight = n; }

        void setStartFollowDist(const int n)
        { mStartFollowDist = n; }

        int getStartFollowDist() const noexcept2 A_WARN_UNUSED
        { return mStartFollowDist; }

        void setFollowDist(const int n)
        { mFollowDist = n; }

        int getFollowDist() const noexcept2 A_WARN_UNUSED
        { return mFollowDist; }

        void setWalkSpeed(const int n)
        { mWalkSpeed = n; }

        int getWalkSpeed() const noexcept2 A_WARN_UNUSED
        { return mWalkSpeed; }

        void setWarpDist(const int n)
        { mWarpDist = n; }

        int getWarpDist() const noexcept2 A_WARN_UNUSED
        { return mWarpDist; }

        void setSitOffsetX(const int n)
        { mSitOffsetX = n; }

        int getSitOffsetX() const noexcept2 A_WARN_UNUSED
        { return mSitOffsetX; }

        void setSitOffsetY(const int n)
        { mSitOffsetY = n; }

        int getSitOffsetY() const noexcept2 A_WARN_UNUSED
        { return mSitOffsetY; }

        void setMoveOffsetX(const int n)
        { mMoveOffsetX = n; }

        int getMoveOffsetX() const noexcept2 A_WARN_UNUSED
        { return mMoveOffsetX; }

        void setMoveOffsetY(const int n)
        { mMoveOffsetY = n; }

        int getMoveOffsetY() const noexcept2 A_WARN_UNUSED
        { return mMoveOffsetY; }

        void setDeadOffsetX(const int n)
        { mDeadOffsetX = n; }

        int getDeadOffsetX() const noexcept2 A_WARN_UNUSED
        { return mDeadOffsetX; }

        void setDeadOffsetY(const int n)
        { mDeadOffsetY = n; }

        int getDeadOffsetY() const noexcept2 A_WARN_UNUSED
        { return mDeadOffsetY; }

        void setAttackOffsetX(const int n)
        { mAttackOffsetX = n; }

        int getAttackOffsetX() const noexcept2 A_WARN_UNUSED
        { return mAttackOffsetX; }

        void setAttackOffsetY(const int n)
        { mAttackOffsetY = n; }

        int getAttackOffsetY() const noexcept2 A_WARN_UNUSED
        { return mAttackOffsetY; }

        void setThinkTime(const int n)
        { mThinkTime = n; }

        int getThinkTime() const noexcept2 A_WARN_UNUSED
        { return mThinkTime; }

        void setDirectionType(const int n)
        { mDirectionType = n; }

        int getDirectionType() const noexcept2 A_WARN_UNUSED
        { return mDirectionType; }

        void setSitDirectionType(const int n)
        { mSitDirectionType = n; }

        int getSitDirectionType() const noexcept2 A_WARN_UNUSED
        { return mSitDirectionType; }

        void setDeadDirectionType(const int n)
        { mDeadDirectionType = n; }

        int getDeadDirectionType() const noexcept2 A_WARN_UNUSED
        { return mDeadDirectionType; }

        void setAttackDirectionType(const int n)
        { mAttackDirectionType = n; }

        int getAttackDirectionType() const noexcept2 A_WARN_UNUSED
        { return mAttackDirectionType; }

        void setAllowDelete(const bool b)
        { mAllowDelete = b; }

        int getAllowDelete() const noexcept2 A_WARN_UNUSED
        { return static_cast<int>(mAllowDelete); }

        void setQuickActionEffectId(const int n)
        { mQuickActionEffectId = n; }

        int getQuickActionEffectId() const noexcept2 A_WARN_UNUSED
        { return mQuickActionEffectId; }

        void setColorsList(const std::string &name);

        std::string getColor(const ItemColor idx) const A_WARN_UNUSED;

        void addMenu(const std::string &name, const std::string &command);

        const STD_VECTOR<BeingMenuItem> &getMenu() const A_CONST;

        void setString(const int idx,
                       const std::string &value)
        { mStrings[idx] = value; }

        std::string getString(const int idx) const A_WARN_UNUSED;

        std::string getCurrency() const A_WARN_UNUSED
        { return mCurrency; }

        void setCurrency(const std::string &name)
        { mCurrency = name; }

        static void init();

        static void clear();

    private:
        SpriteDisplay mDisplay;
        std::string mName;
        TargetCursorSizeT mTargetCursorSize;
        CursorT mHoverCursor;
        ItemSoundEvents mSounds;
        Attacks mAttacks;
        STD_VECTOR<BeingMenuItem> mMenu;
        std::map<int, std::string> mStrings;
        std::string mCurrency;
        unsigned char mBlockWalkMask;
        BlockTypeT mBlockType;
        const std::map <ItemColor, ItemColorData> *mColors;
        int mTargetOffsetX;
        int mTargetOffsetY;
        int mNameOffsetX;
        int mNameOffsetY;
        int mHpBarOffsetX;
        int mHpBarOffsetY;
        int mMaxHP;
        int mSortOffsetY;
        int mDeadSortOffsetY;
        BeingTypeId mAvatarId;
        int mWidth;
        int mHeight;
        int mStartFollowDist;
        int mFollowDist;
        int mWarpDist;
        int mWalkSpeed;
        int mSitOffsetX;
        int mSitOffsetY;
        int mMoveOffsetX;
        int mMoveOffsetY;
        int mDeadOffsetX;
        int mDeadOffsetY;
        int mAttackOffsetX;
        int mAttackOffsetY;
        int mThinkTime;
        int mDirectionType;
        int mSitDirectionType;
        int mDeadDirectionType;
        int mAttackDirectionType;
        int mQuickActionEffectId;
        bool mStaticMaxHP;
        bool mTargetSelection;
        bool mAllowDelete;
};

typedef std::map<BeingTypeId, BeingInfo*> BeingInfos;
typedef BeingInfos::iterator BeingInfoIterator;

#endif  // RESOURCES_BEINGINFO_H
