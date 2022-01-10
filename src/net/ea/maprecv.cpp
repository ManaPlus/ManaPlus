/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#include "net/ea/maprecv.h"

#include "gui/viewport.h"

#include "resources/map/map.h"

#include "net/messagein.h"

#include "debug.h"

namespace Ea
{

void MapRecv::processSetTilesType(Net::MessageIn &msg)
{
    const int x1 = msg.readInt16("x1");
    const int y1 = msg.readInt16("y1");
    const int x2 = msg.readInt16("x2");
    const int y2 = msg.readInt16("y2");
    const BlockTypeT mask = static_cast<BlockTypeT>(msg.readInt32("mask"));
    const int layer = msg.readInt32("layer");
    const std::string name = msg.readString(16, "map name");
    if (layer != 0)
        return;
    Map *const map = viewport->getMap();
    if ((map != nullptr) && map->getGatName() == name)
    {
        for (int y = y1; y <= y2; y ++)
        {
            for (int x = x1; x <= x2; x ++)
                map->setBlockMask(x, y, mask);
        }
        map->updateConditionLayers();
        map->preCacheLayers();
    }
}

}  // namespace Ea
