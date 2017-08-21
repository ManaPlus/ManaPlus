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

#include "resources/iteminfo.h"

#include "const/resources/spriteaction.h"

#include "const/resources/map/map.h"

#include "enums/resources/spritedirection.h"

#include "resources/itemmenuitem.h"

#include "resources/db/colordb.h"
#include "resources/db/itemdb.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/stringutils.h"

#include "debug.h"

ItemInfo::ItemInfo() :
    mDisplay(),
    mMissile(),
    mName(),
    mNameEn(),
    mDescription(),
    mEffect(),
    mUseButton(),
    mUseButton2(),
    mType(ItemDbType::UNUSABLE),
    mWeight(0),
    mView(0),
    mId(0),
    mIsRemoveSprites(false),
    mSpriteToItemReplaceList(),
    mAttackAction(SpriteAction::INVALID),
    mSkyAttackAction(SpriteAction::INVALID),
    mWaterAttackAction(SpriteAction::INVALID),
    mRideAttackAction(SpriteAction::INVALID),
    mAttackRange(0),
    mMissileParticle(),
    mAnimationFiles(),
    mSounds(),
    mTags(),
    mColorsList(nullptr),
    mIconColorsList(nullptr),
    mInventoryMenu(),
    mStorageMenu(),
    mCartMenu(),
    mColorsListName(),
    mIconColorsListName(),
    mCardColor(ItemColor_zero),
    mHitEffectId(-1),
    mCriticalHitEffectId(-1),
    mMissEffectId(-1),
    maxFloorOffsetX(mapTileSize),
    maxFloorOffsetY(mapTileSize),
    mPickupCursor(Cursor::CURSOR_POINTER),
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

const std::string &ItemInfo::getSprite(const GenderT gender,
                                       const BeingTypeId race) const
{
    if (mView != 0)
    {
        // Forward the request to the item defining how to view this item
        return ItemDB::get(mView).getSprite(gender, race);
    }
    static const std::string empty;
    std::map<int, std::string>::const_iterator i =
        mAnimationFiles.find(CAST_S32(gender) +
        toInt(race, int) * 4);

    if (i != mAnimationFiles.end())
        return i->second;
    i = mAnimationFiles.find(CAST_S32(gender));
    if (i != mAnimationFiles.end())
        return i->second;
    return empty;
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

void ItemInfo::setRideAttackAction(const std::string &attackAction)
{
    if (attackAction.empty())
        mRideAttackAction = SpriteAction::ATTACKRIDE;
    else
        mRideAttackAction = attackAction;
}

void ItemInfo::addSound(const ItemSoundEvent::Type event,
                        const std::string &filename, const int delay)
{
    mSounds[event].push_back(SoundInfo(
        filename,
        delay));
}

const SoundInfo &ItemInfo::getSound(const ItemSoundEvent::Type event) const
{
    static const SoundInfo empty("", 0);
    std::map<ItemSoundEvent::Type, SoundInfoVect>::const_iterator i;

    i = mSounds.find(event);

    if (i == mSounds.end())
        return empty;
    return (!i->second.empty()) ? i->second[CAST_U32(rand())
        % i->second.size()] : empty;
}

IntMap *ItemInfo::addReplaceSprite(const int sprite,
                                   const int direction)
{
    if (direction < 0 || direction >= 10)
        return nullptr;

    SpriteToItemMap *spMap = mSpriteToItemReplaceMap[direction];

    if (spMap == nullptr)
    {
        spMap = new SpriteToItemMap;
        mSpriteToItemReplaceMap[direction] = spMap;
        mSpriteToItemReplaceList.push_back(spMap);
    }

    SpriteToItemMap::iterator it = spMap->find(sprite);
    if (it == spMap->end())
    {
        IntMap tmp;
        (*mSpriteToItemReplaceMap[direction])[sprite] = tmp;
        it = mSpriteToItemReplaceMap[direction]->find(sprite);
    }
    return &it->second;
}

void ItemInfo::setColorsList(const std::string &name)
{
    if (name.empty())
    {
        mColorsList = nullptr;
        mColorsListName.clear();
    }
    else
    {
        mColorsList = ColorDB::getColorsList(name);
        mColorsListName = name;
    }
}

void ItemInfo::setIconColorsList(const std::string &name)
{
    if (name.empty())
    {
        mIconColorsList = nullptr;
        mIconColorsListName.clear();
    }
    else
    {
        mIconColorsList = ColorDB::getColorsList(name);
        mIconColorsListName = name;
    }
}

std::string ItemInfo::getDyeColorsString(const ItemColor color) const
{
    if ((mColorsList == nullptr) || mColorsListName.empty())
        return "";

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mColorsList->find(color);
    if (it == mColorsList->end())
        return "";

    return it->second.color;
}

std::string ItemInfo::getDyeIconColorsString(const ItemColor color) const
{
    if ((mIconColorsList == nullptr) || mIconColorsListName.empty())
        return "";

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mIconColorsList->find(color);
    if (it == mIconColorsList->end())
        return "";

    return it->second.color;
}

const std::string ItemInfo::getDescription(const ItemColor color) const
{
    return replaceColors(mDescription, color);
}

const std::string ItemInfo::getName(const ItemColor color) const
{
    return replaceColors(mName, color);
}

const std::string ItemInfo::getNameEn(const ItemColor color) const
{
    return replaceColors(mNameEn, color);
}

const std::string ItemInfo::replaceColors(std::string str,
                                          const ItemColor color) const
{
    std::string name;
    if ((mColorsList != nullptr) && !mColorsListName.empty())
    {
        const std::map <ItemColor, ItemColorData>::const_iterator
            it = mColorsList->find(color);
        if (it == mColorsList->end())
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
        name[0] = CAST_S8(toupper(name[0]));

    return replaceAll(str, "%Color%", name);
}

const SpriteToItemMap *ItemInfo::getSpriteToItemReplaceMap(const int direction)
                                                           const
{
    if (direction < 0 || direction >= 10)
        return nullptr;

    const SpriteToItemMap *const spMap = mSpriteToItemReplaceMap[direction];
    if (spMap != nullptr)
        return spMap;
    if (direction == SpriteDirection::UPLEFT
        || direction == SpriteDirection::UPRIGHT)
    {
        return mSpriteToItemReplaceMap[SpriteDirection::UP];
    }

    if (direction == SpriteDirection::DOWNLEFT
        || direction == SpriteDirection::DOWNRIGHT)
    {
        return mSpriteToItemReplaceMap[SpriteDirection::DOWN];
    }

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
            ptr[SpriteDirection::DOWN] = n;
            ptr[SpriteDirection::DOWNLEFT] = n;
            ptr[SpriteDirection::DOWNRIGHT] = n;
            return;
        }
        case -3:
        {
            ptr[SpriteDirection::UP] = n;
            ptr[SpriteDirection::UPLEFT] = n;
            ptr[SpriteDirection::UPRIGHT] = n;
            return;
        }
        default:
            break;
    }
    if (direction < 0 || direction >= 9)
        return;

    if (direction == SpriteDirection::UP)
    {
        if (ptr[SpriteDirection::UPLEFT] == def)
            ptr[SpriteDirection::UPLEFT] = n;
        if (ptr[SpriteDirection::UPRIGHT] == def)
            ptr[SpriteDirection::UPRIGHT] = n;
    }
    else if (direction == SpriteDirection::DOWN)
    {
        if (ptr[SpriteDirection::DOWNLEFT] == def)
            ptr[SpriteDirection::DOWNLEFT] = n;
        if (ptr[SpriteDirection::DOWNRIGHT] == def)
            ptr[SpriteDirection::DOWNRIGHT] = n;
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
                         const GenderT gender,
                         const int race)
{
    mAnimationFiles[CAST_S32(gender) + race * 4] = animationFile;
}

std::string ItemInfo::getColorName(const ItemColor idx) const
{
    if (mColorsList == nullptr)
        return std::string();

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mColorsList->find(idx);
    if (it == mColorsList->end())
    {
        reportAlways("Color %d for palette %s not found",
            CAST_S32(idx),
            mColorsListName.c_str());
        return std::string();
    }
    return it->second.name;
}

std::string ItemInfo::getColor(const ItemColor idx) const
{
    if (mColorsList == nullptr)
        return std::string();

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mColorsList->find(idx);
    if (it == mColorsList->end())
    {
        reportAlways("Color %d for palette %s not found",
            CAST_S32(idx),
            mColorsListName.c_str());
        return std::string();
    }
    return it->second.color;
}

std::string ItemInfo::getIconColorName(const ItemColor idx) const
{
    if (mIconColorsList == nullptr)
        return std::string();

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mIconColorsList->find(idx);
    if (it == mIconColorsList->end())
    {
        reportAlways("Color %d for palette %s not found",
            CAST_S32(idx),
            mColorsListName.c_str());
        return std::string();
    }
    return it->second.name;
}

std::string ItemInfo::getIconColor(const ItemColor idx) const
{
    if (mIconColorsList == nullptr)
        return std::string();

    const std::map <ItemColor, ItemColorData>::const_iterator
        it = mIconColorsList->find(idx);
    if (it == mIconColorsList->end())
    {
        reportAlways("Color %d for palette %s not found",
            CAST_S32(idx),
            mColorsListName.c_str());
        return std::string();
    }
    return it->second.color;
}

const std::string ItemInfo::getLink() const
{
    return strprintf("[@@%d|%s@@]", mId, mName.c_str());
}
