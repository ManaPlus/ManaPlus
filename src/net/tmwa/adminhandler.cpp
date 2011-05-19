/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "net/tmwa/adminhandler.h"

#include "net/ea/adminhandler.h"

#include "actorspritemanager.h"
#include "being.h"
#include "event.h"
#include "game.h"
#include "log.h"
#include "playerrelations.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "net/tmwa/protocol.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>

extern Net::AdminHandler *adminHandler;

namespace TmwAthena
{

AdminHandler::AdminHandler()
{
    static const Uint16 _messages[] =
    {
        SMSG_ADMIN_KICK_ACK,
        0
    };
    handledMessages = _messages;
    adminHandler = this;
}

void AdminHandler::handleMessage(Net::MessageIn &msg)
{
    int id;
    switch (msg.getId())
    {
        case SMSG_ADMIN_KICK_ACK:
            id = msg.readInt32();
            if (id == 0)
                SERVER_NOTICE(_("Kick failed!"))
            else
                SERVER_NOTICE(_("Kick succeeded!"))
        default:
            break;
    }
}

void AdminHandler::announce(const std::string &text)
{
    MessageOut outMsg(CMSG_ADMIN_ANNOUNCE);
    outMsg.writeInt16(static_cast<Sint16>(text.length() + 4));
    outMsg.writeString(text, static_cast<int>(text.length()));
}

void AdminHandler::localAnnounce(const std::string &text)
{
    MessageOut outMsg(CMSG_ADMIN_LOCAL_ANNOUNCE);
    outMsg.writeInt16(static_cast<Sint16>(text.length() + 4));
    outMsg.writeString(text, static_cast<int>(text.length()));
}

void AdminHandler::hide(bool hide _UNUSED_)
{
    MessageOut outMsg(CMSG_ADMIN_HIDE);
    outMsg.writeInt32(0); //unused
}

void AdminHandler::kick(int playerId)
{
    MessageOut outMsg(CMSG_ADMIN_KICK);
    outMsg.writeInt32(playerId);
}

} // namespace TmwAthena
