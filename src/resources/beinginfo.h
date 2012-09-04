/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef BEINGINFO_H
#define BEINGINFO_H

#include "actorsprite.h"

#include <list>
#include <map>

struct Attack
{
    std::string action;
    std::string particleEffect;
    std::string missileParticle;

    Attack(std::string action0, std::string particleEffect0,
           std::string missileParticle0) :
        action(action0),
        particleEffect(particleEffect0),
        missileParticle(missileParticle0)
    {
    }
};

typedef std::map<int, Attack*> Attacks;

enum SoundEvent
{
    SOUND_EVENT_HIT = 0,
    SOUND_EVENT_MISS,
    SOUND_EVENT_HURT,
    SOUND_EVENT_DIE,
    SOUND_EVENT_MOVE,
    SOUND_EVENT_SIT,
    SOUND_EVENT_SITTOP,
    SOUND_EVENT_SPAWN
};

typedef std::map<SoundEvent, StringVect*> SoundEvents;

/**
 * Holds information about a certain type of monster. This includes the name
 * of the monster, the sprite to display and the sounds the monster makes.
 *
 * @see MonsterDB
 * @see NPCDB
 */
class BeingInfo
{
    public:
        static BeingInfo *unknown;
        static Attack *empty;

        BeingInfo();

        ~BeingInfo();

        void setName(const std::string &name)
        { mName = name; }

        const std::string &getName() const
        { return mName; }

        void setDisplay(SpriteDisplay display);

        const SpriteDisplay &getDisplay() const
        { return mDisplay; }

        void setTargetCursorSize(const std::string &size);

        void setTargetCursorSize(const ActorSprite::TargetCursorSize
                                 &targetSize)
        { mTargetCursorSize = targetSize; }

        ActorSprite::TargetCursorSize getTargetCursorSize() const
        { return mTargetCursorSize; }

        void addSound(const SoundEvent event, const std::string &filename);

        const std::string &getSound(const SoundEvent event) const;

        void addAttack(const int id, std::string action,
                       const std::string &particleEffect,
                       const std::string &missileParticle);

        const Attack *getAttack(const int type) const;

        void setWalkMask(const unsigned char mask)
        { mWalkMask = mask; }

        /**
         * Gets the way the being is blocked by other objects
         */
        unsigned char getWalkMask() const
        { return mWalkMask; }

        void setBlockType(const Map::BlockType &blockType)
        { mBlockType = blockType; }

        Map::BlockType getBlockType() const
        { return mBlockType; }

        void setTargetOffsetX(const int n)
        { mTargetOffsetX = n; }

        int getTargetOffsetX() const
        { return mTargetOffsetX; }

        void setTargetOffsetY(const int n)
        { mTargetOffsetY = n; }

        int getTargetOffsetY() const
        { return mTargetOffsetY; }

        void setMaxHP(const int n)
        { mMaxHP = n; }

        int getMaxHP() const
        { return mMaxHP; }

        bool isStaticMaxHP() const
        { return mStaticMaxHP; }

        void setStaticMaxHP(const bool n)
        { mStaticMaxHP = n; }

        void setTargetSelection(const bool n)
        { mTargetSelection = n; }

        bool isTargetSelection() const
        { return mTargetSelection; }

        static void clear();

    private:
        SpriteDisplay mDisplay;
        std::string mName;
        ActorSprite::TargetCursorSize mTargetCursorSize;
        SoundEvents mSounds;
        Attacks mAttacks;
        unsigned char mWalkMask;
        Map::BlockType mBlockType;
        int mTargetOffsetX;
        int mTargetOffsetY;
        int mMaxHP;
        bool mStaticMaxHP;
        bool mTargetSelection;
};

typedef std::map<int, BeingInfo*> BeingInfos;
typedef BeingInfos::iterator BeingInfoIterator;

#endif // BEINGINFO_H
