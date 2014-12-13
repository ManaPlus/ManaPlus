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
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "resources/notifytypes.h"

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
        outMsg.writeInt32(being->getId(), "account id");
    }
}

void PartyHandler::inviteResponse(const std::string &inviter A_UNUSED,
                                  const bool accept) const
{
    if (localPlayer)
    {
        createOutPacket(CMSG_PARTY_INVITED);
        outMsg.writeInt32(localPlayer->getId(), "account id");
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
        outMsg.writeInt32(being->getId(), "account id");
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
    outMsg.writeInt32(m->getID(), "member id");
    outMsg.writeString(name, 24, "unused");
}

void PartyHandler::chat(const std::string &text) const
{
    createOutPacket(CMSG_PARTY_MESSAGE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "text");
}

void PartyHandler::setShareExperience(const Net::PartyShare::Type share) const
{
    if (share == Net::PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(share), "share exp");
    outMsg.writeInt16(static_cast<int16_t>(mShareItems), "share items");
}

void PartyHandler::setShareItems(const Net::PartyShare::Type share) const
{
    if (share == Net::PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(mShareExp), "share exp");
    outMsg.writeInt16(static_cast<int16_t>(share), "share items");
}

void PartyHandler::processPartySettings(Net::MessageIn &msg)
{
    if (!partyTab)
    {
        if (!chatWindow)
            return;

        createTab();
    }

    // These seem to indicate the sharing mode for exp and items
    const int16_t exp = msg.readInt16("share exp");
    const int16_t item = msg.readInt16("share items");
    processPartySettingsContinue(exp, item);
}

void PartyHandler::processPartyInfo(Net::MessageIn &msg)
{
    bool isOldParty = false;
    std::set<std::string> names;
    std::set<std::string> onlineNames;
    if (!Ea::taParty)
    {
        logger->log1("error: party empty in SMSG_PARTY_INFO");
        Ea::taParty = Party::getParty(1);
    }
    if (Ea::taParty)
    {
        if (Ea::taParty->getNumberOfElements() > 1)
        {
            isOldParty = true;
            Ea::taParty->getNamesSet(names);
            const Party::MemberList *const members = Ea::taParty->getMembers();
            FOR_EACHP (Party::MemberList::const_iterator, it, members)
            {
                if ((*it)->getOnline())
                    onlineNames.insert((*it)->getName());
            }
            if (localPlayer)
                onlineNames.insert(localPlayer->getName());
        }
    }

    if (!localPlayer)
        logger->log1("error: localPlayer==0 in SMSG_PARTY_INFO");

    if (Ea::taParty)
        Ea::taParty->clearMembers();

    const int length = msg.readInt16("len");
    if (Ea::taParty)
        Ea::taParty->setName(msg.readString(24, "party name"));

    const int count = (length - 28) / 46;
    if (localPlayer && Ea::taParty)
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
    }

    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32("id");
        std::string nick = msg.readString(24, "nick");
        std::string map = msg.readString(16, "map");
        const bool leader = msg.readUInt8("leader") == 0U;
        const bool online = msg.readUInt8("online") == 0U;

        if (Ea::taParty)
        {
            bool joined(false);

            if (isOldParty)
            {
                if (names.find(nick) == names.end())
                {
                    NotifyManager::notify(NotifyTypes::PARTY_USER_JOINED,
                        nick);
                    joined = true;
                }
            }
            PartyMember *const member = Ea::taParty->addMember(id, nick);
            if (member)
            {
                if (!joined && partyTab)
                {
                    if (!names.empty() && ((onlineNames.find(nick)
                        == onlineNames.end() && online)
                        || (onlineNames.find(nick) != onlineNames.end()
                        && !online)))
                    {
                        partyTab->showOnline(nick, online);
                    }

                    member->setLeader(leader);
                    member->setOnline(online);
                    member->setMap(map);
                }
                else
                {
                    member->setLeader(leader);
                    member->setOnline(online);
                    member->setMap(map);
                }
            }
        }
    }

    if (Ea::taParty)
        Ea::taParty->sort();

    if (localPlayer && Ea::taParty)
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
        if (socialWindow)
            socialWindow->updateParty();
    }
}

void PartyHandler::processPartyMessage(Net::MessageIn &msg)
{
    const int msgLength = msg.readInt16("len") - 8;
    if (msgLength <= 0)
        return;

    const int id = msg.readInt32("id");
    const std::string chatMsg = msg.readString(msgLength, "message");

    if (Ea::taParty && partyTab)
    {
        const PartyMember *const member = Ea::taParty->getMember(id);
        if (member)
        {
            partyTab->chatLog(member->getName(), chatMsg);
        }
        else
        {
            NotifyManager::notify(NotifyTypes::PARTY_UNKNOWN_USER_MSG,
                chatMsg);
        }
    }
}

void PartyHandler::processPartyInviteResponse(Net::MessageIn &msg)
{
    if (!partyTab)
        return;

    const std::string nick = msg.readString(24, "nick");

    switch (msg.readUInt8("status"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_ALREADY_MEMBER,
                nick);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_REFUSED, nick);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_DONE, nick);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_FULL,
                nick);
            break;
        default:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_ERROR, nick);
            break;
    }
}

void PartyHandler::changeLeader(const std::string &name A_UNUSED) const
{
}

void PartyHandler::allowInvite(const bool allow A_UNUSED) const
{
}

void PartyHandler::processPartyInvited(Net::MessageIn &msg)
{
    const int id = msg.readInt32("account id");
    const std::string partyName = msg.readString(24, "party name");
    std::string nick;

    if (actorManager)
    {
        const Being *const being = actorManager->findBeing(id);
        if (being)
        {
            if (being->getType() == ActorType::Player)
                nick = being->getName();
        }
    }

    if (socialWindow)
        socialWindow->showPartyInvite(partyName, nick);
}

}  // namespace TmwAthena
