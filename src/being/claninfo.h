/*
 *  The ManaPlus Client
 *  Copyright (C) 2018  The ManaPlus Developers
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

#ifndef BEING_CLANINFO_H
#define BEING_CLANINFO_H

#include "enums/simpletypes/beingid.h"

#include "utils/vector.h"

#include <string>

#include "localconsts.h"

struct ClanInfo final
{
    ClanInfo() :
        allyClans(),
        antagonistClans(),
        name(),
        masterName(),
        mapName(),
        id(0),
        onlineMembers(0),
        totalMembers(0)
    {
    }

    A_DELETE_COPY(ClanInfo)

    void clear()
    {
        allyClans.clear();
        antagonistClans.clear();
        name.clear();
        masterName.clear();
        mapName.clear();
        id = 0;
        onlineMembers = 0;
        totalMembers = 0;
    }

    STD_VECTOR<std::string> allyClans;
    STD_VECTOR<std::string> antagonistClans;
    std::string name;
    std::string masterName;
    std::string mapName;
    int id;
    int onlineMembers;
    int totalMembers;
};

extern ClanInfo clanInfo;

#endif  // BEING_CLANINFO_H
