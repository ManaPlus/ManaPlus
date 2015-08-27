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

#include "net/eathena/maphandler.h"

#include "logger.h"

#include "net/eathena/maprecv.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::MapHandler *mapHandler;

namespace EAthena
{

MapHandler::MapHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_INSTANCE_START,
        SMSG_INSTANCE_CREATE,
        SMSG_INSTANCE_INFO,
        SMSG_INSTANCE_DELETE,
        0
    };
    handledMessages = _messages;
    mapHandler = this;
}

void MapHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_INSTANCE_START:
            MapRecv::processInstanceStart(msg);
            break;

        case SMSG_INSTANCE_CREATE:
            MapRecv::processInstanceCreate(msg);
            break;

        case SMSG_INSTANCE_INFO:
            MapRecv::processInstanceInfo(msg);
            break;

        case SMSG_INSTANCE_DELETE:
            MapRecv::processInstanceDelete(msg);
            break;

        default:
            break;
    }
}

}  // namespace EAthena
