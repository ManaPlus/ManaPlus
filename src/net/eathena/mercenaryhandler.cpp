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

#include "net/eathena/mercenaryhandler.h"

#include "logger.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::MercenaryHandler *mercenaryHandler;

namespace EAthena
{

MercenaryHandler::MercenaryHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_MERCENARY_UPDATE,
        0
    };
    handledMessages = _messages;
    mercenaryHandler = this;
}

void MercenaryHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_MERCENARY_UPDATE:
            processMercenaryUpdate(msg);
            break;

        default:
            break;
    }
}

void MercenaryHandler::processMercenaryUpdate(Net::MessageIn &msg)
{
    // +++ need create if need mercenary being and update stat
    msg.readInt16("type");
    msg.readInt32("value");
}

}  // namespace EAthena
