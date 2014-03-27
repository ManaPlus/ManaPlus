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

#include "resources/beinginfo.h"

#include "configuration.h"
#include "logger.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

BeingInfo *BeingInfo::unknown = nullptr;
Attack *BeingInfo::empty = new Attack(SpriteAction::ATTACK,
    SpriteAction::ATTACKSKY, SpriteAction::ATTACKWATER,
    -1, -1, -1, -1, std::string());

BeingInfo::BeingInfo() :
    mDisplay(),
    // TRANSLATORS: being info default name
    mName(_("unnamed")),
    mTargetCursorSize(ActorSprite::TC_MEDIUM),
    mHoverCursor(Cursor::CURSOR_POINTER),
    mSounds(),
    mAttacks(),
    mWalkMask(Map::BLOCKMASK_WALL | Map::BLOCKMASK_CHARACTER
              | Map::BLOCKMASK_MONSTER | Map::BLOCKMASK_AIR
              | Map::BLOCKMASK_WATER),
    mBlockType(Map::BLOCKTYPE_CHARACTER),
    mColors(nullptr),
    mTargetOffsetX(0),
    mTargetOffsetY(0),
    mNameOffsetX(0),
    mNameOffsetY(0),
    mHpBarOffsetX(0),
    mHpBarOffsetY(0),
    mMaxHP(0),
    mSortOffsetY(0),
    mDeadSortOffsetY(31),
    mAvatarId(0),
    mWidth(0),
    mHeight(0),
    mStartFollowDist(3),
    mFollowDist(1),
    mWarpDist(11),
    mWalkSpeed(0),
    mSitOffsetX(0),
    mSitOffsetY(0),
    mMoveOffsetX(0),
    mMoveOffsetY(0),
    mDeadOffsetX(0),
    mDeadOffsetY(0),
    mAttackOffsetX(0),
    mAttackOffsetY(0),
    mThinkTime(50),
    mDirectionType(1),
    mSitDirectionType(1),
    mDeadDirectionType(1),
    mAttackDirectionType(1),
    mStaticMaxHP(false),
    mTargetSelection(true)
{
    SpriteDisplay display;
    display.sprites.push_back(SpriteReference::Empty);

    setDisplay(display);
}

BeingInfo::~BeingInfo()
{
    delete_all(mSounds);
    delete_all(mAttacks);
    mSounds.clear();
    delete_all(mDisplay.sprites);
}

void BeingInfo::setDisplay(const SpriteDisplay &display)
{
    mDisplay = display;
}

void BeingInfo::setTargetCursorSize(const std::string &size)
{
    if (size == "small")
    {
        setTargetCursorSize(ActorSprite::TC_SMALL);
    }
    else if (size == "medium")
    {
        setTargetCursorSize(ActorSprite::TC_MEDIUM);
    }
    else if (size == "large")
    {
        setTargetCursorSize(ActorSprite::TC_LARGE);
    }
    else
    {
        logger->log("Unknown target cursor type \"%s\" for %s - using medium "
                    "sized one", size.c_str(), getName().c_str());
        setTargetCursorSize(ActorSprite::TC_MEDIUM);
    }
}

void BeingInfo::addSound(const ItemSoundEvent event,
                         const std::string &filename,
                         const int delay)
{
    if (mSounds.find(event) == mSounds.end())
        mSounds[event] = new SoundInfoVect;

    if (mSounds[event])
        mSounds[event]->push_back(SoundInfo("sfx/" + filename, delay));
}

const SoundInfo &BeingInfo::getSound(const ItemSoundEvent event) const
{
    static SoundInfo emptySound("", 0);

    const ItemSoundEvents::const_iterator i = mSounds.find(event);

    if (i == mSounds.end())
        return emptySound;

    const SoundInfoVect *const vect = i->second;
    if (!vect || vect->empty())
        return emptySound;
    else
        return vect->at(rand() % vect->size());
}

const Attack *BeingInfo::getAttack(const int id) const
{
    const Attacks::const_iterator i = mAttacks.find(id);
    return (i == mAttacks.end()) ? empty : (*i).second;
}

void BeingInfo::addAttack(const int id, std::string action,
                          std::string skyAction, std::string waterAction,
                          const int effectId, const int hitEffectId,
                          const int criticalHitEffectId,
                          const int missEffectId,
                          const std::string &missileParticle)
{
    delete mAttacks[id];
    mAttacks[id] = new Attack(action, skyAction, waterAction, effectId,
        hitEffectId, criticalHitEffectId, missEffectId, missileParticle);
}

void BeingInfo::clear()
{
    delete2(unknown);
    delete2(empty);
}

void BeingInfo::init()
{
    if (empty)
    {
        empty->mEffectId = paths.getIntValue("effectId");
        empty->mHitEffectId = paths.getIntValue("hitEffectId");
        empty->mCriticalHitEffectId = paths.getIntValue("criticalHitEffectId");
        empty->mMissEffectId = paths.getIntValue("missEffectId");
    }
}

void BeingInfo::setColorsList(const std::string &name)
{
    if (name.empty())
        mColors = nullptr;
    else
        mColors = ColorDB::getColorsList(name);
}

std::string BeingInfo::getColor(const int idx) const
{
    if (!mColors)
        return std::string();

    const std::map <int, ColorDB::ItemColor>::const_iterator
        it = mColors->find(idx);
    if (it == mColors->end())
        return std::string();
    return it->second.color;
}
