/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#include "net/tmwa/partyhandler.h"

#include "actormanager.h"
#include "notifications.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "net/tmwa/messageout.h"
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
            processPartyCreate(msg);
            break;
        case SMSG_PARTY_INFO:
            processPartyInfo(msg);
            break;
        case SMSG_PARTY_INVITE_RESPONSE:
            processPartyInviteResponse(msg);
            break;
        case SMSG_PARTY_INVITED:
            processPartyInvited(msg);
            break;
        case SMSG_PARTY_SETTINGS:
            processPartySettings(msg);
            break;
        case SMSG_PARTY_MOVE:
            processPartyMove(msg);
            break;
        case SMSG_PARTY_LEAVE:
            processPartyLeave(msg);
            break;
        case SMSG_PARTY_UPDATE_HP:
            processPartyUpdateHp(msg);
            break;
        case SMSG_PARTY_UPDATE_COORDS:
            processPartyUpdateCoords(msg);
            break;
        case SMSG_PARTY_MESSAGE:
            processPartyMessage(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PartyHandler::handleMessage")
}

void PartyHandler::create(const std::string &name) const
{
    MessageOut outMsg(CMSG_PARTY_CREATE);
    outMsg.writeString(name.substr(0, 23), 24);
}

void PartyHandler::invite(Being *const being) const
{
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_INVITE);
        outMsg.writeInt32(being->getId());
    }
}

void PartyHandler::invite(const std::string &name) const
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, Being::PLAYER);
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_INVITE);
        outMsg.writeInt32(being->getId());
    }
}

void PartyHandler::inviteResponse(const std::string &inviter A_UNUSED,
                                  const bool accept) const
{
    if (player_node)
    {
        MessageOut outMsg(CMSG_PARTY_INVITED);
        outMsg.writeInt32(player_node->getId());
        outMsg.writeInt32(accept ? 1 : 0);
    }
}

void PartyHandler::leave() const
{
    MessageOut outMsg(CMSG_PARTY_LEAVE);
}

void PartyHandler::kick(Being *const being) const
{
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_KICK);
        outMsg.writeInt32(being->getId());
        outMsg.writeString("", 24);  // unused
    }
}

void PartyHandler::kick(const std::string &name) const
{
    if (!Ea::taParty)
        return;

    const PartyMember *const m = Ea::taParty->getMember(name);
    if (!m)
    {
        NotifyManager::notify(NotifyManager::PARTY_USER_NOT_IN_PARTY, name);
        return;
    }

    MessageOut outMsg(CMSG_PARTY_KICK);
    outMsg.writeInt32(m->getID());
    outMsg.writeString(name, 24);  // unused
}

void PartyHandler::chat(const std::string &text) const
{
    MessageOut outMsg(CMSG_PARTY_MESSAGE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4));
    outMsg.writeString(text, static_cast<int>(text.length()));
}

void PartyHandler::setShareExperience(const PartyShare share) const
{
    if (share == PARTY_SHARE_NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(share));
    outMsg.writeInt16(static_cast<int16_t>(mShareItems));
}

void PartyHandler::setShareItems(const PartyShare share) const
{
    if (share == PARTY_SHARE_NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(mShareExp));
    outMsg.writeInt16(static_cast<int16_t>(share));
}

}  // namespace TmwAthena
