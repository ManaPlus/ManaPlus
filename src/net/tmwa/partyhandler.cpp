/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#include "net/tmwa/partyhandler.h"

#include "actormanager.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "net/ea/partyrecv.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/partyrecv.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

extern Net::PartyHandler *partyHandler;

namespace TmwAthena
{

PartyHandler::PartyHandler() :
    MessageHandler(),
    Ea::PartyHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PARTY_CREATE,
        SMSG_PARTY_INFO,
        SMSG_PARTY_INVITE_RESPONSE,
        SMSG_PARTY_INVITED,
        SMSG_PARTY_SETTINGS,
        SMSG_PARTY_MOVE,
        SMSG_PARTY_LEAVE,
        SMSG_PARTY_UPDATE_HP,
        SMSG_PARTY_UPDATE_COORDS,
        SMSG_PARTY_MESSAGE,
        0
    };
    handledMessages = _messages;
    partyHandler = this;
}

PartyHandler::~PartyHandler()
{
}

void PartyHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("PartyHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_PARTY_CREATE:
            Ea::PartyRecv::processPartyCreate(msg);
            break;
        case SMSG_PARTY_INFO:
            PartyRecv::processPartyInfo(msg);
            break;
        case SMSG_PARTY_INVITE_RESPONSE:
            PartyRecv::processPartyInviteResponse(msg);
            break;
        case SMSG_PARTY_INVITED:
            PartyRecv::processPartyInvited(msg);
            break;
        case SMSG_PARTY_SETTINGS:
            PartyRecv::processPartySettings(msg);
            break;
        case SMSG_PARTY_MOVE:
            PartyRecv::processPartyMove(msg);
            break;
        case SMSG_PARTY_LEAVE:
            Ea::PartyRecv::processPartyLeave(msg);
            break;
        case SMSG_PARTY_UPDATE_HP:
            PartyRecv::processPartyUpdateHp(msg);
            break;
        case SMSG_PARTY_UPDATE_COORDS:
            Ea::PartyRecv::processPartyUpdateCoords(msg);
            break;
        case SMSG_PARTY_MESSAGE:
            PartyRecv::processPartyMessage(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PartyHandler::handleMessage")
}

void PartyHandler::create(const std::string &name) const
{
    createOutPacket(CMSG_PARTY_CREATE);
    outMsg.writeString(name.substr(0, 23), 24, "party name");
}

void PartyHandler::invite(const std::string &name) const
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being)
    {
        createOutPacket(CMSG_PARTY_INVITE);
        outMsg.writeBeingId(being->getId(), "account id");
    }
}

void PartyHandler::inviteResponse(const int partyId A_UNUSED,
                                  const bool accept) const
{
    if (localPlayer)
    {
        createOutPacket(CMSG_PARTY_INVITED);
        outMsg.writeBeingId(localPlayer->getId(), "account id");
        outMsg.writeInt32(accept ? 1 : 0, "accept");
    }
}

void PartyHandler::leave() const
{
    createOutPacket(CMSG_PARTY_LEAVE);
}

void PartyHandler::kick(const Being *const being) const
{
    if (being)
    {
        createOutPacket(CMSG_PARTY_KICK);
        outMsg.writeBeingId(being->getId(), "account id");
        outMsg.writeString("", 24, "unused");
    }
}

void PartyHandler::kick(const std::string &name) const
{
    if (!Ea::taParty)
        return;

    const PartyMember *const m = Ea::taParty->getMember(name);
    if (!m)
    {
        NotifyManager::notify(NotifyTypes::PARTY_USER_NOT_IN_PARTY, name);
        return;
    }

    createOutPacket(CMSG_PARTY_KICK);
    outMsg.writeBeingId(m->getID(), "member id");
    outMsg.writeString(name, 24, "unused");
}

void PartyHandler::chat(const std::string &text) const
{
    createOutPacket(CMSG_PARTY_MESSAGE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "text");
}

void PartyHandler::setShareExperience(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(share), "share exp");
    outMsg.writeInt16(static_cast<int16_t>(Ea::PartyRecv::mShareItems),
        "share items");
}

void PartyHandler::setShareItems(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(Ea::PartyRecv::mShareExp),
        "share exp");
    outMsg.writeInt16(static_cast<int16_t>(share), "share items");
}

void PartyHandler::changeLeader(const std::string &name A_UNUSED) const
{
}

void PartyHandler::allowInvite(const bool allow A_UNUSED) const
{
}

}  // namespace TmwAthena
