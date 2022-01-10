/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_DB_ITEMDB_H
#define RESOURCES_DB_ITEMDB_H

#include "enums/simpletypes/skiperror.h"

#include "utils/stringvector.h"

#include <map>

#include "localconsts.h"

class ItemInfo;

/**
 * Item information database.
 */
namespace ItemDB
{
    void load();

    void unload();

    void loadXmlFile(const std::string &fileName,
                     int &tagNum,
                     const SkipError skipError);

    const StringVect &getTags();

    bool exists(const int id) A_WARN_UNUSED;
    bool exists(const std::string &name) A_WARN_UNUSED;

    const ItemInfo &get(const int id) A_WARN_UNUSED;
    const ItemInfo &get(const std::string &name) A_WARN_UNUSED;

    const ItemInfo &getEmpty() A_WARN_UNUSED;

    int getNumOfHairstyles() A_WARN_UNUSED;

    // Items database
    typedef std::map<int, ItemInfo*> ItemInfos;
    typedef std::map<std::string, ItemInfo*> NamedItemInfos;

    const ItemDB::ItemInfos &getItemInfos();

    std::string getNamesStr(const STD_VECTOR<int> &parts);

#ifdef UNITTESTS
    ItemDB::NamedItemInfos &getNamedItemInfosTest();

    ItemDB::ItemInfos &getItemInfosTest();
#endif  // UNITTESTS

    int getTagId(const std::string &tagName) A_WARN_UNUSED;
}  // namespace ItemDB

#endif  // RESOURCES_DB_ITEMDB_H
