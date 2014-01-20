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

#ifndef RESOURCES_BEINGINFO_H
#define RESOURCES_BEINGINFO_H

#include "being/actorsprite.h"

#include "resources/cursor.h"
#include "resources/soundinfo.h"

#include "resources/db/colordb.h"

#include <map>

struct Attack final
{
    std::string mAction;
    std::string mSkyAction;
    std::string mWaterAction;
    int mEffectId;
    int mHitEffectId;
    int mCriticalHitEffectId;
    int mMissEffectId;
    std::string mMissileParticle;

    Attack(const std::string &action, const std::string &skyAction,
           const std::string &waterAction, const int effectId,
           const int hitEffectId, const int criticalHitEffectId,
           const int missEffectId, const std::string &missileParticle) :
        mAction(action),
        mSkyAction(skyAction),
        mWaterAction(waterAction),
        mEffectId(effectId),
        mHitEffectId(hitEffectId),
        mCriticalHitEffectId(criticalHitEffectId),
        mMissEffectId(missEffectId),
        mMissileParticle(missileParticle)
    {
    }

    A_DELETE_COPY(Attack)
};

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

        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        void setDisplay(const SpriteDisplay &display);

        const SpriteDisplay &getDisplay() const A_WARN_UNUSED
        { return mDisplay; }

        void setTargetCursorSize(const std::string &size);

        void setTargetCursorSize(const ActorSprite::TargetCursorSize
                                 &targetSize)
        { mTargetCursorSize = targetSize; }

        void setHoverCursor(const std::string &name)
        { return setHoverCursor(Cursor::stringToCursor(name)); }

        void setHoverCursor(const Cursor::Cursor &cursor)
        { mHoverCursor = cursor; }

        Cursor::Cursor getHoverCursor() const A_WARN_UNUSED
        { return mHoverCursor; }

        ActorSprite::TargetCursorSize getTargetCursorSize() const A_WARN_UNUSED
        { return mTargetCursorSize; }

        void addSound(const ItemSoundEvent event, const std::string &filename,
                      const int delay);

        const SoundInfo &getSound(const ItemSoundEvent event)
                                  const A_WARN_UNUSED;

        void addAttack(const int id, std::string action, std::string skyAttack,
                       std::string waterAttack, const int effectId,
                       const int hitEffectId, const int criticalHitEffectId,
                       const int missEffectId,
                       const std::string &missileParticle);

        const Attack *getAttack(const int id) const A_WARN_UNUSED;

        void setWalkMask(const unsigned char mask)
        { mWalkMask = mask; }

        /**
         * Gets the way the being is blocked by other objects
         */
        unsigned char getWalkMask() const A_WARN_UNUSED
        { return mWalkMask; }

        void setBlockType(const Map::BlockType &blockType)
        { mBlockType = blockType; }

        Map::BlockType getBlockType() const A_WARN_UNUSED
        { return mBlockType; }

        void setTargetOffsetX(const int n)
        { mTargetOffsetX = n; }

        int getTargetOffsetX() const A_WARN_UNUSED
        { return mTargetOffsetX; }

        void setTargetOffsetY(const int n)
        { mTargetOffsetY = n; }

        int getTargetOffsetY() const A_WARN_UNUSED
        { return mTargetOffsetY; }

        void setNameOffsetX(const int n)
        { mNameOffsetX = n; }

        int getNameOffsetX() const A_WARN_UNUSED
        { return mNameOffsetX; }

        void setNameOffsetY(const int n)
        { mNameOffsetY = n; }

        int getNameOffsetY() const A_WARN_UNUSED
        { return mNameOffsetY; }

        void setHpBarOffsetX(const int n)
        { mHpBarOffsetX = n; }

        int getHpBarOffsetX() const A_WARN_UNUSED
        { return mHpBarOffsetX; }

        void setHpBarOffsetY(const int n)
        { mHpBarOffsetY = n; }

        int getHpBarOffsetY() const A_WARN_UNUSED
        { return mHpBarOffsetY; }

        void setMaxHP(const int n)
        { mMaxHP = n; }

        int getMaxHP() const A_WARN_UNUSED
        { return mMaxHP; }

        bool isStaticMaxHP() const A_WARN_UNUSED
        { return mStaticMaxHP; }

        void setStaticMaxHP(const bool n)
        { mStaticMaxHP = n; }

        void setTargetSelection(const bool n)
        { mTargetSelection = n; }

        bool isTargetSelection() const A_WARN_UNUSED
        { return mTargetSelection; }

        int getSortOffsetY() const A_WARN_UNUSED
        { return mSortOffsetY; }

        void setSortOffsetY(const int n)
        { mSortOffsetY = n; }

        int getDeadSortOffsetY() const A_WARN_UNUSED
        { return mDeadSortOffsetY; }

        void setDeadSortOffsetY(const int n)
        { mDeadSortOffsetY = n; }

        uint16_t getAvatarId() const A_WARN_UNUSED
        { return mAvatarId; }

        void setAvatarId(const uint16_t id)
        { mAvatarId = id; }

        int getWidth() const A_WARN_UNUSED
        { return mWidth; }

        int getHeight() const A_WARN_UNUSED
        { return mHeight; }

        void setWidth(const int n)
        { mWidth = n; }

        void setHeight(const int n)
        { mHeight = n; }

        void setStartFollowDist(const int n)
        { mStartFollowDist = n; }

        int getStartFollowDist() const A_WARN_UNUSED
        { return mStartFollowDist; }

        void setFollowDist(const int n)
        { mFollowDist = n; }

        int getFollowDist() const A_WARN_UNUSED
        { return mFollowDist; }

        void setWalkSpeed(const int n)
        { mWalkSpeed = n; }

        int getWalkSpeed() const A_WARN_UNUSED
        { return mWalkSpeed; }

        void setWarpDist(const int n)
        { mWarpDist = n; }

        int getWarpDist() const A_WARN_UNUSED
        { return mWarpDist; }

        void setSitOffsetX(const int n)
        { mSitOffsetX = n; }

        int getSitOffsetX() const A_WARN_UNUSED
        { return mSitOffsetX; }

        void setSitOffsetY(const int n)
        { mSitOffsetY = n; }

        int getSitOffsetY() const A_WARN_UNUSED
        { return mSitOffsetY; }

        void setMoveOffsetX(const int n)
        { mMoveOffsetX = n; }

        int getMoveOffsetX() const A_WARN_UNUSED
        { return mMoveOffsetX; }

        void setMoveOffsetY(const int n)
        { mMoveOffsetY = n; }

        int getMoveOffsetY() const A_WARN_UNUSED
        { return mMoveOffsetY; }

        void setDeadOffsetX(const int n)
        { mDeadOffsetX = n; }

        int getDeadOffsetX() const A_WARN_UNUSED
        { return mDeadOffsetX; }

        void setDeadOffsetY(const int n)
        { mDeadOffsetY = n; }

        int getDeadOffsetY() const A_WARN_UNUSED
        { return mDeadOffsetY; }

        void setAttackOffsetX(const int n)
        { mAttackOffsetX = n; }

        int getAttackOffsetX() const A_WARN_UNUSED
        { return mAttackOffsetX; }

        void setAttackOffsetY(const int n)
        { mAttackOffsetY = n; }

        int getAttackOffsetY() const A_WARN_UNUSED
        { return mAttackOffsetY; }

        void setThinkTime(const int n)
        { mThinkTime = n; }

        int getThinkTime() const A_WARN_UNUSED
        { return mThinkTime; }

        void setDirectionType(const int n)
        { mDirectionType = n; }

        int getDirectionType() const A_WARN_UNUSED
        { return mDirectionType; }

        void setSitDirectionType(const int n)
        { mSitDirectionType = n; }

        int getSitDirectionType() const A_WARN_UNUSED
        { return mSitDirectionType; }

        void setDeadDirectionType(const int n)
        { mDeadDirectionType = n; }

        int getDeadDirectionType() const A_WARN_UNUSED
        { return mDeadDirectionType; }

        void setAttackDirectionType(const int n)
        { mAttackDirectionType = n; }

        int getAttackDirectionType() const A_WARN_UNUSED
        { return mAttackDirectionType; }

        void setColorsList(const std::string &name);

        std::string getColor(const int idx) const A_WARN_UNUSED;

        static void init();

        static void clear();

    private:
        SpriteDisplay mDisplay;
        std::string mName;
        ActorSprite::TargetCursorSize mTargetCursorSize;
        Cursor::Cursor mHoverCursor;
        ItemSoundEvents mSounds;
        Attacks mAttacks;
        unsigned char mWalkMask;
        Map::BlockType mBlockType;
        const std::map <int, ColorDB::ItemColor> *mColors;
        int mTargetOffsetX;
        int mTargetOffsetY;
        int mNameOffsetX;
        int mNameOffsetY;
        int mHpBarOffsetX;
        int mHpBarOffsetY;
        int mMaxHP;
        int mSortOffsetY;
        int mDeadSortOffsetY;
        uint16_t mAvatarId;
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
        bool mStaticMaxHP;
        bool mTargetSelection;
};

typedef std::map<int, BeingInfo*> BeingInfos;
typedef BeingInfos::iterator BeingInfoIterator;

#endif  // RESOURCES_BEINGINFO_H
