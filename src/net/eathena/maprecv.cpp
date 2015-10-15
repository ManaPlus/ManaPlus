/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/eathena/maprecv.h"

#include "logger.h"

#include "enums/resources/map/blocktype.h"

#include "gui/viewport.h"

#include "net/messagein.h"

#include "resources/map/map.h"

#include "debug.h"

namespace EAthena
{

void MapRecv::processInstanceStart(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(61, "instance name");
    msg.readInt16("flag");
}

void MapRecv::processInstanceCreate(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("flag");
}

void MapRecv::processInstanceInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(61, "instance name");
    msg.readInt32("remaining time");
    msg.readInt32("no players close time");
}

void MapRecv::processInstanceDelete(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("flag");
    msg.readInt32("unused");
}

void MapRecv::processSetTilesType(Net::MessageIn &msg)
{
    const int x1 = msg.readInt16("x1");
    const int y1 = msg.readInt16("y1");
    const int x2 = msg.readInt16("x2");
    const int y2 = msg.readInt16("y2");
    const BlockType mask = fromInt(msg.readInt32("mask"), BlockType);
    const std::string name = msg.readString(16, "map name");

    Map *const map = viewport->getMap();
//    logger->log("map test name: %s, mask %d", map->getGatName().c_str(), (int)mask);
    if (map && map->getGatName() == name)
    {
        for (int y = y1; y <= y2; y ++)
        {
            for (int x = x1; x <= x2; x ++)
            {
                logger->log("set col %d,%d", x, y);
                map->setBlockMask(x, y, mask);
            }
        }
    }
}

}  // namespace EAthena
