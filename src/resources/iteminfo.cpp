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

#include "resources/iteminfo.h"

#include "resources/itemdb.h"
#include "configuration.h"

#include <set>
#include <map>

const std::string &ItemInfo::getSprite(Gender gender) const
{
    if (mView)
    {
        // Forward the request to the item defining how to view this item
        return ItemDB::get(mView).getSprite(gender);
    }
    else
    {
        static const std::string empty = "";
        std::map<int, std::string>::const_iterator i =
            mAnimationFiles.find(gender);

        return (i != mAnimationFiles.end()) ? i->second : empty;
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

    return i == mSounds.end() ? empty : i->second[rand() % i->second.size()];
}

std::map<int, int> &ItemInfo::addReplaceSprite(int sprite)
{
    std::map<int, std::map<int, int> >::iterator it
        = mSpriteToItemReplaceMap.find(sprite);
    if (it == mSpriteToItemReplaceMap.end())
    {
        std::map<int, int> tmp;
        mSpriteToItemReplaceMap[sprite] = tmp;
        it = mSpriteToItemReplaceMap.find(sprite);
    }
    return it->second;
}

std::string ItemInfo::getDyeString(int color) const
{
    if (mId == 1172)
    {
        if (color == 1)
            return "W:#115511,22aa22,99dd99;R:#547000,a5dc00,d1ff46";
        if (color == 0)
            return "W:#547000,a5dc00,d1ff46;R:#115511,22aa22,99dd99";
        if (color == 3)
            return "W:#ffffff,000000,000000;R:#333333,333333,333333";
    }
    return "";
}

std::string ItemInfo::getDyeColorsString(int color) const
{
    if (mId == 1172)
    {
        if (color == 1)
            return "#115511,22aa22,99dd99;#547000,a5dc00,d1ff46";
        if (color == 0)
            return "#547000,a5dc00,d1ff46;#115511,22aa22,99dd99";
        if (color == 3)
            return "#ffffff,000000,000000;#333333,333333,333333";
    }
    return "";
}