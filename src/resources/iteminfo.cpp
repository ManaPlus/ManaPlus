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

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "configuration.h"

#include "utils/dtor.h"

#include <map>

#include "debug.h"

extern int serverVersion;

ItemInfo::ItemInfo() :
    mMissileParticleFile(),
    mDisplay(),
    mName(),
    mDescription(),
    mEffect(),
    mType(ITEM_UNUSABLE),
    mWeight(0),
    mView(0),
    mId(0),
    mIsRemoveSprites(false),
    mSpriteToItemReplaceList(),
    mAttackAction(SpriteAction::INVALID),
    mSkyAttackAction(SpriteAction::INVALID),
    mWaterAttackAction(SpriteAction::INVALID),
    mAttackRange(0),
    mMissileParticle(),
    mAnimationFiles(),
    mSounds(),
    mTags(),
    mColors(nullptr),
    mColorList(),
    mHitEffectId(-1),
    mCriticalHitEffectId(-1),
    mMissEffectId(-1),
    maxFloorOffset(mapTileSize),
    mPickupCursor(Cursor::CURSOR_POINTER),
    mPet(0),
    mProtected(false)
{
    for (int f = 0; f < 10; f ++)
    {
        mSpriteToItemReplaceMap[f] = nullptr;
        mDrawBefore[f] = -1;
        mDrawAfter[f] = -1;
        mDrawPriority[f] = 0;
    }
}

ItemInfo::~ItemInfo()
{
    delete_all(mSpriteToItemReplaceList);
    mSpriteToItemReplaceList.clear();
    for (int f = 0; f < 10; f ++)
        mSpriteToItemReplaceMap[f] = nullptr;
}

const std::string &ItemInfo::getSprite(const Gender gender,
                                       const int race) const
{
    if (mView)
    {
        // Forward the request to the item defining how to view this item
        return ItemDB::get(mView).getSprite(gender, race);
    }
    else
    {
        static const std::string empty;
        std::map<int, std::string>::const_iterator i =
            mAnimationFiles.find(static_cast<int>(gender) + race * 4);

        if (i != mAnimationFiles.end())
            return i->second;
        i = mAnimationFiles.find(static_cast<int>(gender));
        if (i != mAnimationFiles.end())
            return i->second;
        return empty;
    }
}

void ItemInfo::setAttackAction(const std::string &attackAction)
{
    if (attackAction.empty())
        mAttackAction = SpriteAction::ATTACK;  // (Equal to unarmed animation)
    else
        mAttackAction = attackAction;
}

void ItemInfo::setSkyAttackAction(const std::string &attackAction)
{
    if (attackAction.empty())
        mSkyAttackAction = SpriteAction::ATTACKSKY;
    else
        mSkyAttackAction = attackAction;
}

void ItemInfo::setWaterAttackAction(const std::string &attackAction)
{
    if (attackAction.empty())
        mWaterAttackAction = SpriteAction::ATTACKWATER;
    else
        mWaterAttackAction = attackAction;
}

void ItemInfo::addSound(const ItemSoundEvent event,
                        const std::string &filename, const int delay)
{
    mSounds[event].push_back(SoundInfo(
        paths.getStringValue("sfx").append(filename), delay));
}

const SoundInfo &ItemInfo::getSound(const ItemSoundEvent event) const
{
    static const SoundInfo empty("", 0);
    std::map<ItemSoundEvent, SoundInfoVect>::const_iterator i;

    i = mSounds.find(event);

    if (i == mSounds.end())
        return empty;
    return (!i->second.empty()) ? i->second[rand()
        % i->second.size()] : empty;
}

std::map<int, int> *ItemInfo::addReplaceSprite(const int sprite,
                                               const int direction)
{
    if (direction < 0 || direction >= 10)
        return nullptr;

    SpriteToItemMap *spMap = mSpriteToItemReplaceMap[direction];

    if (!spMap)
    {
        spMap = new SpriteToItemMap();
        mSpriteToItemReplaceMap[direction] = spMap;
        mSpriteToItemReplaceList.push_back(spMap);
    }

    SpriteToItemMap::iterator it = spMap->find(sprite);
    if (it == spMap->end())
    {
        std::map<int, int> tmp;
        (*mSpriteToItemReplaceMap[direction])[sprite] = tmp;
        it = mSpriteToItemReplaceMap[direction]->find(sprite);
    }
    return &it->second;
}

void ItemInfo::setColorsList(const std::string &name)
{
    if (name.empty())
    {
        mColors = nullptr;
        mColorList.clear();
    }
    else
    {
        mColors = ColorDB::getColorsList(name);
        mColorList = name;
    }
}

std::string ItemInfo::getDyeColorsString(const int color) const
{
    if (!mColors || mColorList.empty())
        return "";

    const std::map <int, ColorDB::ItemColor>::const_iterator
        it = mColors->find(color);
    if (it == mColors->end())
        return "";

    return it->second.color;
}

const std::string ItemInfo::getDescription(const unsigned char color) const
{
    return replaceColors(mDescription, color);
}

const std::string ItemInfo::getName(const unsigned char color) const
{
    return replaceColors(mName, color);
}

const std::string ItemInfo::replaceColors(std::string str,
                                          const unsigned char color) const
{
    std::string name;
    if (mColors && !mColorList.empty())
    {
        const std::map <int, ColorDB::ItemColor>::const_iterator
            it = mColors->find(color);
        if (it == mColors->end())
            name = "unknown";
        else
            name = it->second.name;
    }
    else
    {
        name = "unknown";
    }

    str = replaceAll(str, "%color%", name);
    if (!name.empty())
        name[0] = static_cast<signed char>(toupper(name[0]));

    return replaceAll(str, "%Color%", name);
}

const SpriteToItemMap *ItemInfo::getSpriteToItemReplaceMap(const int direction)
                                                           const
{
    if (direction < 0 || direction >= 10)
        return nullptr;

    SpriteToItemMap *const spMap = mSpriteToItemReplaceMap[direction];
    if (spMap)
        return spMap;
    if (direction == DIRECTION_UPLEFT || direction == DIRECTION_UPRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_UP];

    if (direction == DIRECTION_DOWNLEFT || direction == DIRECTION_DOWNRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_DOWN];

    return nullptr;
}

void ItemInfo::setSpriteOrder(int *const ptr,
                              const int direction,
                              const int n,
                              const int def)
{
    switch (direction)
    {
        case -1:
        {
            for (int f = 0; f < 10; f ++)
            {
                if (ptr[f] == def)
                    ptr[f] = n;
            }
            return;
        }
        case -2:
        {
            ptr[DIRECTION_DOWN] = n;
            ptr[DIRECTION_DOWNLEFT] = n;
            ptr[DIRECTION_DOWNRIGHT] = n;
            return;
        }
        case -3:
        {
            ptr[DIRECTION_UP] = n;
            ptr[DIRECTION_UPLEFT] = n;
            ptr[DIRECTION_UPRIGHT] = n;
            return;
        }
        default:
            break;
    }
    if (direction < 0 || direction >= 9)
        return;

    if (direction == DIRECTION_UP)
    {
        if (ptr[DIRECTION_UPLEFT] == def)
            ptr[DIRECTION_UPLEFT] = n;
        if (ptr[DIRECTION_UPRIGHT] == def)
            ptr[DIRECTION_UPRIGHT] = n;
    }
    else if (direction == DIRECTION_DOWN)
    {
        if (ptr[DIRECTION_DOWNLEFT] == def)
            ptr[DIRECTION_DOWNLEFT] = n;
        if (ptr[DIRECTION_DOWNRIGHT] == def)
            ptr[DIRECTION_DOWNRIGHT] = n;
    }
    ptr[direction] = n;
}

void ItemInfo::setDrawBefore(const int direction, const int n)
{
    setSpriteOrder(&mDrawBefore[0], direction, n);
}

void ItemInfo::setDrawAfter(const int direction, const int n)
{
    setSpriteOrder(&mDrawAfter[0], direction, n);
}

void ItemInfo::setDrawPriority(const int direction, const int n)
{
    setSpriteOrder(&mDrawPriority[0], direction, n, 0);
}

int ItemInfo::getDrawBefore(const int direction) const
{
    if (direction < 0 || direction >= 10)
        return -1;
    return mDrawBefore[direction];
}

int ItemInfo::getDrawAfter(const int direction) const
{
    if (direction < 0 || direction >= 10)
        return -1;
    return mDrawAfter[direction];
}

int ItemInfo::getDrawPriority(const int direction) const
{
    if (direction < 0 || direction >= 10)
        return 0;
    return mDrawPriority[direction];
}

void ItemInfo::setSprite(const std::string &animationFile,
                         const Gender gender, const int race)
{
    mAnimationFiles[static_cast<int>(gender) + race * 4] = animationFile;
}

std::string ItemInfo::getColorName(const int idx) const
{
    if (!mColors)
        return std::string();

    const std::map <int, ColorDB::ItemColor>::const_iterator
        it = mColors->find(idx);
    if (it == mColors->end())
        return std::string();
    return it->second.name;
}

std::string ItemInfo::getColor(const int idx) const
{
    if (!mColors)
        return std::string();

    const std::map <int, ColorDB::ItemColor>::const_iterator
        it = mColors->find(idx);
    if (it == mColors->end())
        return std::string();
    return it->second.color;
}
