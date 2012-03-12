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

#include "resources/iteminfo.h"

#include "resources/itemdb.h"
#include "configuration.h"

#include "utils/dtor.h"

#include <set>
#include <map>

#include "debug.h"

extern int serverVersion;

/*
ItemInfo::ItemInfo(ItemInfo &info)
{
    mMissileParticleFile = info.mMissileParticleFile;
    mDisplay = info.mDisplay;
    mName = info.mName;
    mDescription = info.mDescription;
    mEffect = info.mEffect;
    mType = info.mType;
    mParticle = info.mParticle;
    mWeight = info.mWeight;
    mView = info.mView;
    mId = info.mId;
    mIsRemoveSprites = info.mIsRemoveSprites;
    mAttackAction = info.mAttackAction;
    mAttackRange = info.mAttackRange;
    mMissileParticle = info.mMissileParticle;
    mColors = info.mColors;
    mColorList = info.mColorList;
    mHitEffectId = info.mHitEffectId;
    mCriticalHitEffectId = info.mCriticalHitEffectId;
    for (int f = 0; f < 9; f ++)
    {
        mSpriteToItemReplaceMap[f] = 0;
        mDrawBefore[f] = info.mDrawBefore[f];
        mDrawAfter[f] = info.mDrawAfter[f];
        mDrawPriority[f] = info.mDrawPriority[f];
    }
}
*/

ItemInfo::ItemInfo() :
    mMissileParticleFile(""),
    mName(""),
    mDescription(""),
    mEffect(""),
    mType(ITEM_UNUSABLE),
    mParticle(""),
    mWeight(0),
    mView(0),
    mId(0),
    mIsRemoveSprites(false),
    mAttackAction(SpriteAction::INVALID),
    mAttackRange(0),
    mMissileParticle(""),
    mColors(nullptr),
    mColorList(""),
    mHitEffectId(0),
    mCriticalHitEffectId(0)
{
    for (int f = 0; f < 9; f ++)
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
    for (int f = 0; f < 9; f ++)
        mSpriteToItemReplaceMap[f] = nullptr;
}

const std::string &ItemInfo::getSprite(Gender gender, int race) const
{
    if (mView)
    {
        // Forward the request to the item defining how to view this item
        return ItemDB::get(mView).getSprite(gender, race);
    }
    else
    {
        static const std::string empty("");
        std::map<int, std::string>::const_iterator i =
            mAnimationFiles.find(static_cast<int>(gender) * 2 + race);

        if (i != mAnimationFiles.end())
            return i->second;
        if (serverVersion > 0)
        {
            i = mAnimationFiles.find(static_cast<int>(gender) * 2);
            if (i != mAnimationFiles.end())
                return i->second;
        }
        return empty;
    }
}

void ItemInfo::setAttackAction(std::string attackAction)
{
    if (attackAction.empty())
        mAttackAction = SpriteAction::ATTACK; // (Equal to unarmed animation)
    else
        mAttackAction = attackAction;
}

void ItemInfo::addSound(EquipmentSoundEvent event, const std::string &filename)
{
    mSounds[event].push_back(paths.getStringValue("sfx") + filename);
}

const std::string &ItemInfo::getSound(EquipmentSoundEvent event) const
{
    static const std::string empty;
    std::map< EquipmentSoundEvent,
        std::vector<std::string> >::const_iterator i;

    i = mSounds.find(event);

    if (i == mSounds.end())
        return empty;
    return (!i->second.empty()) ? i->second[rand() % i->second.size()] : empty;
}

std::map<int, int> *ItemInfo::addReplaceSprite(int sprite, int direction)
{
    if (direction < 0 || direction >= 9)
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

void ItemInfo::setColorsList(std::string name)
{
    if (name.empty())
    {
        mColors = nullptr;
        mColorList = "";
    }
    else
    {
        mColors = ColorDB::getColorsList(name);
        mColorList = name;
    }
}

std::string ItemInfo::getDyeColorsString(int color) const
{
    if (!mColors || mColorList.empty())
        return "";

    std::map <int, ColorDB::ItemColor>::const_iterator
        it = mColors->find(color);
    if (it == mColors->end())
        return "";

    return it->second.color;
}

const std::string ItemInfo::getDescription(unsigned char color) const
{
    return replaceColors(mDescription, color);
}

const std::string ItemInfo::getName(unsigned char color) const
{
    return replaceColors(mName, color);
}

const std::string ItemInfo::replaceColors(std::string str,
                                          unsigned char color) const
{
    std::string name;
    if (mColors && !mColorList.empty())
    {
        std::map <int, ColorDB::ItemColor>::const_iterator
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
        name[0] = static_cast<char>(toupper(name[0]));

    return replaceAll(str, "%Color%", name);
}

SpriteToItemMap *ItemInfo::getSpriteToItemReplaceMap(int direction) const
{
    if (direction < 0 || direction >= 9)
        return nullptr;

    SpriteToItemMap *spMap = mSpriteToItemReplaceMap[direction];
    if (spMap)
        return spMap;
    if (direction == DIRECTION_UPLEFT || direction == DIRECTION_UPRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_UP];

    if (direction == DIRECTION_DOWNLEFT || direction == DIRECTION_DOWNRIGHT)
        return mSpriteToItemReplaceMap[DIRECTION_DOWN];

    return nullptr;
}

void ItemInfo::setSpriteOrder(int *ptr, int direction, int n, int def)
{
    if (direction == -1)
    {
        for (int f = 0; f < 9; f ++)
        {
            if (ptr[f] == def)
                ptr[f] = n;
        }
        return;
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

void ItemInfo::setDrawBefore(int direction, int n)
{
    setSpriteOrder(&mDrawBefore[0], direction, n);
}

void ItemInfo::setDrawAfter(int direction, int n)
{
    setSpriteOrder(&mDrawAfter[0], direction, n);
}

void ItemInfo::setDrawPriority(int direction, int n)
{
    setSpriteOrder(&mDrawPriority[0], direction, n, 0);
}

int ItemInfo::getDrawBefore(int direction) const
{
    if (direction < 0 || direction >= 9)
        return -1;
    return mDrawBefore[direction];
}

int ItemInfo::getDrawAfter(int direction) const
{
    if (direction < 0 || direction >= 9)
        return -1;
    return mDrawAfter[direction];
}

int ItemInfo::getDrawPriority(int direction) const
{
    if (direction < 0 || direction >= 9)
        return 0;
    return mDrawPriority[direction];
}

void ItemInfo::setSprite(const std::string &animationFile,
                         Gender gender, int race)
{
    mAnimationFiles[static_cast<int>(gender) * 2 + race] = animationFile;
}
