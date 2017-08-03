/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#include "net/eathena/partyhandler.h"

#include "actormanager.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "net/ea/partyrecv.h"

#include "net/eathena/messageout.h"
#include "net/eathena/partyrecv.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

PartyHandler::PartyHandler() :
    Ea::PartyHandler()
{
    partyHandler = this;
}

PartyHandler::~PartyHandler()
{
    partyHandler = nullptr;
}

void PartyHandler::create(const std::string &name) const
{
    createOutPacket(CMSG_PARTY_CREATE);
    outMsg.writeString(name.substr(0, 23), 24, "party name");
}

void PartyHandler::invite(const std::string &name) const
{
    if (actorManager == nullptr)
        return;
    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being != nullptr)
    {
        createOutPacket(CMSG_PARTY_INVITE);
        outMsg.writeBeingId(being->getId(), "account id");
    }
    else
    {
        if (packetVersion < 20070227)
            return;
        createOutPacket(CMSG_PARTY_INVITE2);
        outMsg.writeString(name, 24, "nick");
    }
}

void PartyHandler::inviteResponse(const int partyId,
                                  const bool accept) const
{
    if (localPlayer != nullptr)
    {
        createOutPacket(CMSG_PARTY_INVITED2);
        outMsg.writeInt32(partyId, "party id");
        outMsg.writeInt8(CAST_S8(accept ? 1 : 0), "accept");
    }
}

void PartyHandler::leave() const
{
    createOutPacket(CMSG_PARTY_LEAVE);
}

void PartyHandler::kick(const Being *const being) const
{
    if (being != nullptr)
    {
        createOutPacket(CMSG_PARTY_KICK);
        outMsg.writeBeingId(being->getId(), "account id");
        outMsg.writeString(being->getName(), 24, "player name");
    }
}

void PartyHandler::kick(const std::string &name) const
{
    if (Ea::taParty == nullptr)
        return;

    const PartyMember *const m = Ea::taParty->getMember(name);
    if (m == nullptr)
    {
        NotifyManager::notify(NotifyTypes::PARTY_USER_NOT_IN_PARTY, name);
        return;
    }

    createOutPacket(CMSG_PARTY_KICK);
    outMsg.writeBeingId(m->getID(), "account id");
    outMsg.writeString(name, 24, "player name");
}

void PartyHandler::chat(const std::string &text) const
{
    createOutPacket(CMSG_PARTY_MESSAGE);
    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_S16(mes.length() + 4), "len");
        outMsg.writeString(mes, CAST_S32(mes.length()), "nick : message");
    }
    else
    {
        outMsg.writeInt16(CAST_S16(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, CAST_S32(mes.length()), "nick : message");
        outMsg.writeInt8(0, "null char");
    }
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareExperience(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    if (packetVersion >= 20090603)
    {
        outMsg.writeInt32(CAST_S32(share), "share exp");
        outMsg.writeInt16(CAST_S16(Ea::PartyRecv::mShareItems),
            "share items");
    }
    else
    {
        outMsg.writeInt32(CAST_S32(share), "share exp");
    }
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareItems(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    if (packetVersion >= 20090603)
    {
        outMsg.writeInt32(CAST_S32(Ea::PartyRecv::mShareExp),
            "share exp");
        outMsg.writeInt16(CAST_S16(share), "share items");
    }
    else
    {
//        outMsg.writeInt32(CAST_S16(Ea::PartyRecv::mShareExp),
//            "share exp");
    }
}

void PartyHandler::changeLeader(const std::string &name) const
{
    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being == nullptr)
        return;
    createOutPacket(CMSG_PARTY_CHANGE_LEADER);
    outMsg.writeBeingId(being->getId(), "account id");
}

void PartyHandler::allowInvite(const bool allow) const
{
    createOutPacket(CMSG_PARTY_ALLOW_INVITES);
    outMsg.writeInt8(CAST_S8(allow ? 1 : 0), "allow");
}

PartyShareT PartyHandler::getShareAutoItems() const
{
    return PartyRecv::mShareAutoItems;
}

void PartyHandler::setShareAutoItems(const PartyShareT share) const
{
    PartyRecv::mShareAutoItems = share;
}

}  // namespace EAthena
