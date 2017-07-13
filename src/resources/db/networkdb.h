/*
 *  The ManaPlus Client
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

#ifndef RESOURCES_DB_NETWORKDB_H
#define RESOURCES_DB_NETWORKDB_H

#include "enums/simpletypes/skiperror.h"

#include "utils/vector.h"

#include <map>
#include <string>

typedef std::map<int32_t, int32_t> NetworkInPacketInfos;
typedef NetworkInPacketInfos::const_iterator NetworkInPacketInfosIter;
typedef STD_VECTOR<int> NetworkRemovePacketInfos;
typedef NetworkRemovePacketInfos::const_iterator NetworkRemovePacketInfosIter;

namespace NetworkDb
{
    /**
     * Loads the chars data.
     */
    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    /**
     * Clear the chars data
     */
    void unload();

    const NetworkInPacketInfos &getFakePackets();

    const NetworkRemovePacketInfos &getRemovePackets();
}  // namespace NetworkDb

#endif  // RESOURCES_DB_NETWORKDB_H
