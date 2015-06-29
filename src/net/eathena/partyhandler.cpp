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

#include "net/eathena/partyhandler.h"

#include "actormanager.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::PartyHandler *partyHandler;

namespace EAthena
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
        SMSG_PARTY_LEAVE,
        SMSG_PARTY_UPDATE_COORDS,
        SMSG_PARTY_MESSAGE,
        SMSG_PARTY_INVITATION_STATS,
        SMSG_PARTY_MEMBER_INFO,
        SMSG_PARTY_ITEM_PICKUP,
        SMSG_PARTY_LEADER,
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
        case SMSG_PARTY_LEAVE:
            processPartyLeave(msg);
            break;
        case SMSG_PARTY_UPDATE_COORDS:
            processPartyUpdateCoords(msg);
            break;
        case SMSG_PARTY_MESSAGE:
            processPartyMessage(msg);
            break;
        case SMSG_PARTY_INVITATION_STATS:
            processPartyInvitationStats(msg);
            break;
        case SMSG_PARTY_MEMBER_INFO:
            processPartyMemberInfo(msg);
            break;
        case SMSG_PARTY_ITEM_PICKUP:
            processPartyItemPickup(msg);
            break;
        case SMSG_PARTY_LEADER:
            processPartyLeader(msg);
            break;

        default:
            break;
    }
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
    else
    {
        createOutPacket(CMSG_PARTY_INVITE2);
        outMsg.writeString(name, 24, "nick");
    }
}

void PartyHandler::inviteResponse(const int partyId,
                                  const bool accept) const
{
    if (localPlayer)
    {
        createOutPacket(CMSG_PARTY_INVITED2);
        outMsg.writeInt32(partyId, "party id");
        outMsg.writeInt8(static_cast<int8_t>(accept ? 1 : 0), "accept");
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
        outMsg.writeString(being->getName(), 24, "player name");
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
    outMsg.writeBeingId(m->getID(), "account id");
    outMsg.writeString(name, 24, "player name");
}

void PartyHandler::chat(const std::string &text) const
{
    createOutPacket(CMSG_PARTY_MESSAGE);
    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length()), "nick : message");
    outMsg.writeInt8(0, "null char");
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareExperience(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(share), "share exp");
    outMsg.writeInt16(static_cast<int16_t>(mShareItems), "share items");
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareItems(const PartyShareT share) const
{
    if (share == PartyShare::NOT_POSSIBLE)
        return;

    createOutPacket(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(mShareExp), "share exp");
    outMsg.writeInt16(static_cast<int16_t>(share), "share items");
}

void PartyHandler::processPartyInvitationStats(Net::MessageIn &msg)
{
    // +++ for now server allow only switch this option but not using it.
    msg.readUInt8("allow party");
}

void PartyHandler::processPartyMemberInfo(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const bool leader = msg.readInt32("leader") == 0U;
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const bool online = msg.readInt8("online") == 0U;
    msg.readString(24, "party name");
    const std::string nick = msg.readString(24, "player name");
    const std::string map = msg.readString(16, "map name");
    msg.readInt8("party.item&1");
    msg.readInt8("party.item&2");

    if (!Ea::taParty)
        return;

    PartyMember *const member = Ea::taParty->addMember(id, nick);
    if (member)
    {
        if (partyTab && member->getOnline() != online)
            partyTab->showOnline(nick, fromBool(online, Online));
        member->setLeader(leader);
        member->setOnline(online);
        member->setMap(map);
        member->setX(x);
        member->setY(y);
    }
}

void PartyHandler::processPartySettings(Net::MessageIn &msg)
{
    if (!partyTab)
    {
        if (!chatWindow)
            return;

        createTab();
    }

    msg.readInt32("party exp");
    const PartyShareT exp = static_cast<PartyShareT>(
        msg.readInt8("share exp"));
    const PartyShareT item = static_cast<PartyShareT>(
        msg.readInt8("share item"));
    processPartySettingsContinue(msg, exp, item);
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
    if (!partyTab)
        createTab();

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
        const BeingId id = msg.readBeingId("account id");
        std::string nick = msg.readString(24, "nick");
        std::string map = msg.readString(16, "map name");
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
                        partyTab->showOnline(nick, fromBool(online, Online));
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

    const BeingId id = msg.readBeingId("id");
    std::string chatMsg = msg.readString(msgLength, "message");

    const size_t pos = chatMsg.find(" : ", 0);
    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

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

    switch (msg.readInt32("result"))
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
        case 4:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_SAME_ACCOUNT,
                nick);
            break;
        case 5:
            NotifyManager::notify(
                NotifyTypes::PARTY_INVITE_PARTY_BLOCKED_INVITE,
                nick);
            break;
        case 7:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_NOT_ONLINE,
                nick);
            break;
        default:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_ERROR, nick);
            break;
    }
}

void PartyHandler::changeLeader(const std::string &name) const
{
    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (!being)
        return;
    createOutPacket(CMSG_PARTY_CHANGE_LEADER);
    outMsg.writeBeingId(being->getId(), "account id");
}

void PartyHandler::allowInvite(const bool allow) const
{
    createOutPacket(CMSG_PARTY_ALLOW_INVITES);
    outMsg.writeInt8(static_cast<int8_t>(allow ? 1 : 0), "allow");
}

void PartyHandler::processPartyItemPickup(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ probably need add option to show pickup notifications
    // in party tab
    msg.readBeingId("account id");
    msg.readInt16("item id");
    msg.readUInt8("identify");
    msg.readUInt8("attribute");
    msg.readUInt8("refine");
    for (int f = 0; f < 4; f++)
        msg.readInt16("card");
    msg.readInt16("equip location");
    msg.readUInt8("item type");
}

void PartyHandler::processPartyLeader(Net::MessageIn &msg)
{
    PartyMember *const oldMember = Ea::taParty->getMember(
        msg.readBeingId("old leder id"));
    PartyMember *const newMember = Ea::taParty->getMember(
        msg.readBeingId("new leder id"));
    if (oldMember)
        oldMember->setLeader(false);
    if (newMember)
        newMember->setLeader(true);
}

void PartyHandler::processPartyInvited(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
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
        socialWindow->showPartyInvite(partyName, nick, 0);
}

}  // namespace EAthena
