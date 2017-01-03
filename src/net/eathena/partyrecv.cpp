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

#include "net/eathena/partyrecv.h"

#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "net/messagein.h"

#include "net/ea/partyrecv.h"

#include "debug.h"

namespace EAthena
{

namespace PartyRecv
{
    PartyShareT mShareAutoItems = PartyShare::UNKNOWN;
}  // namespace PartyRecv

void PartyRecv::processPartyInvitationStats(Net::MessageIn &msg)
{
    // +++ for now server allow only switch this option but not using it.
    msg.readUInt8("allow party");
}

void PartyRecv::processPartyMemberInfo(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const bool leader = msg.readInt32("leader") == 0U;
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const bool online = msg.readInt8("online") == 0U;
    msg.readString(24, "party name");
    const std::string nick = msg.readString(24, "player name");
    const std::string map = msg.readString(16, "map name");
    msg.readInt8("pickup item share (&1)");
    msg.readInt8("get item share (&2)");

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

void PartyRecv::processPartySettings(Net::MessageIn &msg)
{
    if (!partyTab)
    {
        if (!chatWindow)
            return;

        Ea::PartyRecv::createTab();
    }

    const PartyShareT exp = static_cast<PartyShareT>(
        msg.readInt32("party exp"));
    Ea::PartyRecv::processPartyExpSettingsContinue(msg, exp);
    if (msg.getVersion() >= 20090603)
    {
        const PartyShareT item = static_cast<PartyShareT>(
            msg.readInt8("pickup item share (&1)"));
        Ea::PartyRecv::processPartyItemSettingsContinue(msg, item);
        const PartyShareT autoItem = static_cast<PartyShareT>(
            msg.readInt8("get auto item share (&2)"));
        processPartyAutoItemSettingsContinue(msg, autoItem);
    }
}

void PartyRecv::processPartyInfo(Net::MessageIn &msg)
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
        Ea::PartyRecv::createTab();

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
    const std::string name = msg.readString(24, "party name");
    if (Ea::taParty)
        Ea::taParty->setName(name);

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

void PartyRecv::processPartyMessage(Net::MessageIn &msg)
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

void PartyRecv::processPartyInviteResponse(Net::MessageIn &msg)
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

void PartyRecv::processPartyItemPickup(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    // +++ probably need add option to show pickup notifications
    // in party tab
    msg.readBeingId("account id");
    msg.readInt16("item id");
    msg.readUInt8("identify");
    msg.readUInt8("attribute");
    msg.readUInt8("refine");
    for (int f = 0; f < maxCards; f++)
        msg.readUInt16("card");
    msg.readInt16("equip location");
    msg.readUInt8("item type");
    // for color can be used ItemColorManager
}

void PartyRecv::processPartyLeader(Net::MessageIn &msg)
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

void PartyRecv::processPartyInvited(Net::MessageIn &msg)
{
    const int id = msg.readInt32("party id");
    const std::string partyName = msg.readString(24, "party name");

    if (socialWindow)
        socialWindow->showPartyInvite(partyName, std::string(), id);
}

void PartyRecv::processPartyAutoItemSettingsContinue(Net::MessageIn &msg,
                                                     const PartyShareT item)
{
    switch (item)
    {
        case PartyShare::YES:
            if (mShareAutoItems == PartyShare::YES)
                break;
            mShareAutoItems = PartyShare::YES;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ON);
            break;
        case PartyShare::NO:
            if (mShareAutoItems == PartyShare::NO)
                break;
            mShareAutoItems = PartyShare::NO;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_OFF);
            break;
        case PartyShare::NOT_POSSIBLE:
            if (mShareAutoItems == PartyShare::NOT_POSSIBLE)
                break;
            mShareAutoItems = PartyShare::NOT_POSSIBLE;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ERROR);
            break;
        default:
        case PartyShare::UNKNOWN:
            UNIMPLEMENTEDPACKETFIELD(CAST_S32(item));
            break;
    }
}

}  // namespace EAthena
