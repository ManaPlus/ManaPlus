/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/eathena/elementalhandler.h"

#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::ElementalHandler *elementalHandler;

namespace EAthena
{

ElementalHandler::ElementalHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ELEMENTAL_UPDATE_STATUS,
        0
    };
    handledMessages = _messages;
    elementalHandler = this;
}

void ElementalHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ELEMENTAL_UPDATE_STATUS:
            processElementalUpdateStatus(msg);
            break;

        default:
            break;
    }
}

void ElementalHandler::processElementalUpdateStatus(Net::MessageIn &msg)
{
    msg.readInt16("type");
    msg.readInt32("value");
}

}  // namespace EAthena
