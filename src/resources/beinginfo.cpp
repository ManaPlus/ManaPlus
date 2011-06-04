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

#include "resources/beinginfo.h"

#include "log.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

BeingInfo *BeingInfo::unknown = new BeingInfo;
Attack *BeingInfo::empty = new Attack(SpriteAction::ATTACK, "", "");

BeingInfo::BeingInfo():
        mName(_("unnamed")),
        mTargetCursorSize(ActorSprite::TC_MEDIUM),
        mWalkMask(Map::BLOCKMASK_WALL | Map::BLOCKMASK_CHARACTER
            | Map::BLOCKMASK_MONSTER | Map::BLOCKMASK_AIR
            | Map::BLOCKMASK_WATER),
        mBlockType(Map::BLOCKTYPE_CHARACTER),
        mTargetOffsetX(0), mTargetOffsetY(0),
        mMaxHP(0), mStaticMaxHP(false)
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

void BeingInfo::setDisplay(SpriteDisplay display)
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

void BeingInfo::addSound(SoundEvent event, const std::string &filename)
{
    if (mSounds.find(event) == mSounds.end())
        mSounds[event] = new std::vector<std::string>;

    if (mSounds[event])
        mSounds[event]->push_back("sfx/" + filename);
}

const std::string &BeingInfo::getSound(SoundEvent event) const
{
    static std::string empty("");

    SoundEvents::const_iterator i = mSounds.find(event);
    return (i == mSounds.end() || !i->second) ? empty :
            i->second->at(rand() % i->second->size());
}

const Attack *BeingInfo::getAttack(int type) const
{

    Attacks::const_iterator i = mAttacks.find(type);
    return (i == mAttacks.end()) ? empty : (*i).second;
}

void BeingInfo::addAttack(int id, std::string action,
                          const std::string &particleEffect,
                          const std::string &missileParticle)
{
    if (mAttacks[id])
        delete mAttacks[id];

    mAttacks[id] = new Attack(action, particleEffect, missileParticle);
}

void BeingInfo::clear()
{
    delete unknown;
    unknown = 0;
    delete empty;
    empty = 0;
}
