/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#ifndef RESOURCES_GROUPINFO_H
#define RESOURCES_GROUPINFO_H

#include "localconsts.h"

#include "enums/resources/servercommandenable.h"
#include "enums/resources/servercommandtype.h"
#include "enums/resources/serverpermissiontype.h"

#include "enums/simpletypes/enable.h"

#include "utils/cast.h"

#include <string>

struct GroupInfo final
{
    GroupInfo();

    A_DELETE_COPY(GroupInfo)

    ServerCommandEnable::Type mCommands[CAST_SIZE(ServerCommandType::Max)];
    Enable mPermissions[CAST_SIZE(ServerPermissionType::Max)];
    std::string name;
    std::string longName;
    std::string badge;
    bool showBadge;
};

#endif  // RESOURCES_GROUPINFO_H
