/*
 *  The ManaPlus Client
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

#include "utils/browserboxtools.h"

#include "itemcolormanager.h"
#include "main.h"
#include "settings.h"

#include "input/inputmanager.h"

#include "utils/stringutils.h"

#ifndef DYECMD
#include "const/resources/item/cards.h"

#include "resources/beinginfo.h"
#include "resources/iteminfo.h"

#include "resources/db/homunculusdb.h"
#include "resources/db/itemdb.h"
#include "resources/db/mercenarydb.h"
#include "resources/db/monsterdb.h"
#include "resources/db/questdb.h"
#include "resources/db/petdb.h"
#endif  // DYECMD

#include "debug.h"

void BrowserBoxTools::replaceVars(std::string &data)
{
    data = replaceAll(data, "%VER%", SMALL_VERSION);
    data = replaceAll(data, "%SUPPORTURL%", settings.supportUrl);
}

void BrowserBoxTools::replaceKeys(std::string &data)
{
    size_t idx1 = data.find("###");
    while (idx1 != std::string::npos)
    {
        const size_t idx2 = data.find(';', idx1);
        if (idx2 == std::string::npos)
            break;

        const std::string str = inputManager.getKeyValueByNameLong(
            data.substr(idx1 + 3, idx2 - idx1 - 3));
        data.replace(idx1, idx2 - idx1 + 1, str);

        idx1 = data.find("###");
    }
}

std::string BrowserBoxTools::replaceLinkCommands(const std::string &link)
{
#ifdef DYECMD
    return link;
#else  // DYECMD

    std::string data = link;

    if (strStartWith(data, "http://") ||
        strStartWith(data, "https://"))
    {
        return data;
    }

    if (!link.empty() && link[0] == 'm')
    {  // monster link
        const BeingTypeId id = static_cast<BeingTypeId>(
            atoi(link.substr(1).c_str()));
        BeingInfo *const info = MonsterDB::get(id);
        if (info != nullptr)
            data = info->getName();
    }
    else if (!link.empty() && link[0] == 'p')
    {  // pet link
        const BeingTypeId id = static_cast<BeingTypeId>(
            atoi(link.substr(1).c_str()));
        BeingInfo *const info = PETDB::get(id);
        if (info != nullptr)
            data = info->getName();
    }
    else if (!link.empty() && link[0] == 'h')
    {  // homunculus link
        const BeingTypeId id = static_cast<BeingTypeId>(
            atoi(link.substr(1).c_str()));
        BeingInfo *const info = HomunculusDB::get(id);
        if (info != nullptr)
            data = info->getName();
    }
    else if (!link.empty() && link[0] == 'M')
    {  // mercenary link
        const BeingTypeId id = static_cast<BeingTypeId>(
            atoi(link.substr(1).c_str()));
        BeingInfo *const info = MercenaryDB::get(id);
        if (info != nullptr)
            data = info->getName();
    }
    else if (!link.empty() && link[0] == 'q')
    {  // quest link
        data = QuestDb::getName(
            atoi(link.substr(1).c_str()));
    }
    else
    {  // item link
        size_t idx = link.find(',');
        if (idx != std::string::npos)
        {
            const int id = atoi(link.substr(0, idx).c_str());
            if (id != 0)
            {
                STD_VECTOR<int> parts;
                splitToIntVector(parts,
                    link.substr(idx), ',');
                while (parts.size() < maxCards)
                    parts.push_back(0);
                const ItemColor itemColor =
                    ItemColorManager::getColorFromCards(&parts[0]);
                data = ItemDB::get(id).getName(itemColor);
            }
        }
        else
        {
            const int id = atoi(link.c_str());
            if (id != 0)
                data = ItemDB::get(id).getName();
        }
    }
    return data;
#endif  // DYECMD
}

void BrowserBoxTools::replaceTabs(std::string &data)
{
    size_t idx1 = data.find("\\t");
    while (idx1 != std::string::npos)
    {
        const size_t idx2 = data.find(';', idx1);
        if (idx2 == std::string::npos)
            break;

        const unsigned int newSize = CAST_U32(
            atoi(data.substr(
            idx1 + 2, idx2 - idx1 - 2).c_str()));
        std::string str = data.substr(0, idx1);
        while (str.size() < newSize)
            str.append(" ");
        str.append(data.substr(idx2 + 1));
        data = str;
        idx1 = data.find("\\t");
    }
}
