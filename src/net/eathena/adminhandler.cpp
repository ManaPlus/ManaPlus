/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/adminhandler.h"

#include "notifymanager.h"

#include "gui/chatconsts.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include <string>

#include "debug.h"

extern Net::AdminHandler *adminHandler;

namespace EAthena
{

AdminHandler::AdminHandler() :
    MessageHandler(),
    Ea::AdminHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ADMIN_KICK_ACK,
        0
    };
    handledMessages = _messages;
    adminHandler = this;
}

void AdminHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ADMIN_KICK_ACK:
            if (msg.readInt32() == 0)
                NotifyManager::notify(NotifyTypes::KICK_FAIL);
            else
                NotifyManager::notify(NotifyTypes::KICK_SUCCEED);
            break;
        default:
            break;
    }
}

void AdminHandler::announce(const std::string &text) const
{
    MessageOut outMsg(CMSG_ADMIN_ANNOUNCE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
}

void AdminHandler::localAnnounce(const std::string &text) const
{
    MessageOut outMsg(CMSG_ADMIN_LOCAL_ANNOUNCE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
}

void AdminHandler::hide(const bool h A_UNUSED) const
{
    MessageOut outMsg(CMSG_ADMIN_HIDE);
    outMsg.writeInt32(0, "unused");
}

void AdminHandler::kick(const int playerId) const
{
    MessageOut outMsg(CMSG_ADMIN_KICK);
    outMsg.writeInt32(playerId, "account id");
}

void AdminHandler::kickAll() const
{
    MessageOut outMsg(CMSG_ADMIN_KICK_ALL);
}

void AdminHandler::warp(const std::string &map, const int x, const int y) const
{
    MessageOut outMsg(CMSG_PLAYER_MAPMOVE);
    outMsg.writeString(map, 16, "map");
    outMsg.writeInt16(x, "x");
    outMsg.writeInt16(y, "y");
}

void AdminHandler::resetStats() const
{
    MessageOut outMsg(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(0, "flag");
}

void AdminHandler::resetSkills() const
{
    MessageOut outMsg(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(1, "flag");
}

void AdminHandler::gotoName(const std::string &name) const
{
    MessageOut outMsg(CMSG_ADMIN_GOTO);
    outMsg.writeString(name, 24, "name");
}

}  // namespace EAthena
