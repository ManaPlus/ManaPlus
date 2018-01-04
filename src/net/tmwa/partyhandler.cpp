/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/ea/partyrecv.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "debug.h"

namespace TmwAthena
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
}

void PartyHandler::inviteResponse(const int partyId A_UNUSED,
                                  const bool accept) const
{
    if (localPlayer != nullptr)
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
    if (being != nullptr)
    {
        createOutPacket(CMSG_PARTY_KICK);
        outMsg.writeBeingId(being->getId(), "account id");
        outMsg.writeString("", 24, "unused");
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
    outMsg.writeBeingId(m->getID(), "member id");
    outMsg.writeString(name, 24, "unused");
}

void PartyHandler::chat(const std::string &text) const
{
    createOutPacket(CMSG_PARTY_MESSAGE);
    outMsg.writeInt16(CAST_S16(text.length() + 4), "len");
    outMsg.writeString(text, CAST_S32(text.length()), "text");
}

void PartyHandler::setShareExperience(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(CAST_S16(share), "share exp");
    outMsg.writeInt16(CAST_S16(Ea::PartyRecv::mShareItems),
        "share items");
}

void PartyHandler::setShareItems(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(CAST_S16(Ea::PartyRecv::mShareExp),
        "share exp");
    outMsg.writeInt16(CAST_S16(share), "share items");
}

void PartyHandler::changeLeader(const std::string &name A_UNUSED) const
{
}

void PartyHandler::allowInvite(const bool allow A_UNUSED) const
{
}

PartyShareT PartyHandler::getShareAutoItems() const
{
    return PartyShare::NOT_POSSIBLE;
}

void PartyHandler::setShareAutoItems(const PartyShareT share A_UNUSED) const
{
}

}  // namespace TmwAthena
