/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/beinginfo.h"

#include "configuration.h"
#include "logger.h"

#include "const/resources/spriteaction.h"

#include "enums/resources/map/blockmask.h"

#include "resources/attack.h"

#include "resources/sprite/spritereference.h"

#include "resources/db/colordb.h"

#include "utils/cast.h"
#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

BeingInfo *BeingInfo::unknown = nullptr;
Attack *BeingInfo::empty = new Attack(SpriteAction::ATTACK,
    SpriteAction::ATTACKSKY,
    SpriteAction::ATTACKWATER,
    SpriteAction::ATTACKRIDE,
    -1,
    -1,
    -1,
    -1,
    std::string(),
    32.0F,
    7.0F,
    8.0F,
    500);

BeingInfo::BeingInfo() :
    mDisplay(),
    // TRANSLATORS: being info default name
    mName(_("unnamed")),
    mTargetCursorSize(TargetCursorSize::MEDIUM),
    mHoverCursor(Cursor::CURSOR_POINTER),
    mSounds(),
    mAttacks(),
    mMenu(),
    mStrings(),
    mCurrency(),
    mBlockWalkMask(BlockMask::WALL |
                   BlockMask::AIR |
                   BlockMask::WATER |
                   BlockMask::MONSTERWALL),
    mBlockType(BlockType::NONE),
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
    mAvatarId(BeingTypeId_zero),
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
    mQuickActionEffectId(-1),
    mStaticMaxHP(false),
    mTargetSelection(true),
    mAllowDelete(true),
    mAllowEquipment(false)
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
        setTargetCursorSize(TargetCursorSize::SMALL);
    }
    else if (size == "medium")
    {
        setTargetCursorSize(TargetCursorSize::MEDIUM);
    }
    else if (size == "large")
    {
        setTargetCursorSize(TargetCursorSize::LARGE);
    }
    else
    {
        logger->log("Unknown target cursor type \"%s\" for %s - using medium "
                    "sized one", size.c_str(), getName().c_str());
        setTargetCursorSize(TargetCursorSize::MEDIUM);
    }
}

void BeingInfo::addSound(const ItemSoundEvent::Type event,
                         const std::string &filename,
                         const int delay)
{
    if (mSounds.find(event) == mSounds.end())
        mSounds[event] = new SoundInfoVect;

    if (mSounds[event] != nullptr)
        mSounds[event]->push_back(SoundInfo(filename, delay));
}

const SoundInfo &BeingInfo::getSound(const ItemSoundEvent::Type event) const
{
    static SoundInfo emptySound("", 0);

    const ItemSoundEvents::const_iterator i = mSounds.find(event);

    if (i == mSounds.end())
        return emptySound;

    const SoundInfoVect *const vect = i->second;
    if (vect == nullptr || vect->empty())
        return emptySound;
    return vect->at(CAST_SIZE(rand()) % vect->size());
}

const Attack *BeingInfo::getAttack(const int id) const
{
    const Attacks::const_iterator i = mAttacks.find(id);
    return (i == mAttacks.end()) ? empty : (*i).second;
}

void BeingInfo::addAttack(const int id,
                          const std::string &action,
                          const std::string &skyAction,
                          const std::string &waterAction,
                          const std::string &rideAction,
                          const int effectId,
                          const int hitEffectId,
                          const int criticalHitEffectId,
                          const int missEffectId,
                          const std::string &missileParticle,
                          const float missileZ,
                          const float missileSpeed,
                          const float missileDieDistance,
                          const int missileLifeTime)
{
    delete mAttacks[id];
    mAttacks[id] = new Attack(action,
        skyAction,
        waterAction,
        rideAction,
        effectId,
        hitEffectId,
        criticalHitEffectId,
        missEffectId,
        missileParticle,
        missileZ,
        missileSpeed,
        missileDieDistance,
        missileLifeTime);
}

void BeingInfo::clear()
{
    delete2(unknown)
    delete2(empty)
}

void BeingInfo::init()
{
    if (empty != nullptr)
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

std::string BeingInfo::getColor(const ItemColor idx) const
{
    if (mColors == nullptr)
        return std::string();

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mColors->find(idx);
    if (it == mColors->end())
        return std::string();
    return it->second.color;
}

void BeingInfo::addMenu(const std::string &name, const std::string &command)
{
    mMenu.push_back(BeingMenuItem(name, command));
}

const STD_VECTOR<BeingMenuItem> &BeingInfo::getMenu() const
{
    return mMenu;
}

std::string BeingInfo::getString(const int idx) const
{
    const std::map<int, std::string>::const_iterator it = mStrings.find(idx);
    if (it == mStrings.end())
        return "";
    return (*it).second;
}
