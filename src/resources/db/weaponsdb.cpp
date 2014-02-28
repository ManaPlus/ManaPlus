/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#include "resources/db/weaponsdb.h"

#include "utils/xmlutils.h"

#include "debug.h"

namespace
{
    WeaponsInfos mBows;
    WeaponsInfos mSwords;
    WeaponsInfos mShields;
    bool mLoaded = false;
}

static void loadDB(const std::string &name, WeaponsInfos &arr)
{
    readXmlIntVector("weapons.xml",
        "weapons",
        name,
        "item",
        "id",
        arr);
}

void WeaponsDB::load()
{
    if (mLoaded)
        unload();

    loadDB("swords", mSwords);
    loadDB("bows", mBows);
    loadDB("shields", mShields);
}

void WeaponsDB::unload()
{
    mBows.clear();
    mSwords.clear();
    mShields.clear();
    mLoaded = false;
}

const WeaponsInfos &WeaponsDB::getBows()
{
    return mBows;
}

const WeaponsInfos &WeaponsDB::getSwords()
{
    return mSwords;
}

const WeaponsInfos &WeaponsDB::getShields()
{
    return mShields;
}
