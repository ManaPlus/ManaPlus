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

#include "net/tmwa/partyrecv.h"

#include "actormanager.h"
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

namespace TmwAthena
{

void PartyRecv::processPartySettings(Net::MessageIn &msg)
{
    if (partyTab == nullptr)
    {
        if (chatWindow == nullptr)
            return;

        Ea::PartyRecv::createTab();
    }

    // These seem to indicate the sharing mode for exp and items
    const PartyShareT exp = static_cast<PartyShareT>(
        msg.readInt16("share exp"));
    const PartyShareT item = static_cast<PartyShareT>(
        msg.readInt16("share items"));
    Ea::PartyRecv::processPartyExpSettingsContinue(msg, exp);
    Ea::PartyRecv::processPartyItemSettingsContinue(msg, item);
}

void PartyRecv::processPartyInfo(Net::MessageIn &msg)
{
    bool isOldParty = false;
    std::set<std::string> names;
    std::set<std::string> onlineNames;
    if (Ea::taParty == nullptr)
    {
        logger->log1("error: party empty in SMSG_PARTY_INFO");
        Ea::taParty = Party::getParty(1);
    }
    if (Ea::taParty != nullptr)
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
            if (localPlayer != nullptr)
                onlineNames.insert(localPlayer->getName());
        }
    }

    if (localPlayer == nullptr)
        logger->log1("error: localPlayer==0 in SMSG_PARTY_INFO");

    if (Ea::taParty != nullptr)
        Ea::taParty->clearMembers();

    const int length = msg.readInt16("len");
    if (Ea::taParty != nullptr)
        Ea::taParty->setName(msg.readString(24, "party name"));

    const int count = (length - 28) / 46;
    if ((localPlayer != nullptr) && (Ea::taParty != nullptr))
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
    }

    for (int i = 0; i < count; i++)
    {
        const BeingId id = msg.readBeingId("id");
        std::string nick = msg.readString(24, "nick");
        std::string map = msg.readString(16, "map");
        const bool leader = msg.readUInt8("leader") == 0U;
        const bool online = msg.readUInt8("online") == 0U;

        if (Ea::taParty != nullptr)
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
            if (member != nullptr)
            {
                if (!joined && (partyTab != nullptr))
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

    if (Ea::taParty != nullptr)
        Ea::taParty->sort();

    if ((localPlayer != nullptr) && (Ea::taParty != nullptr))
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
        if (socialWindow != nullptr)
            socialWindow->updateParty();
    }
}

void PartyRecv::processPartyMessage(Net::MessageIn &msg)
{
    const int msgLength = msg.readInt16("len") - 8;
    if (msgLength <= 0)
        return;

    const BeingId id = msg.readBeingId("id");
    const std::string chatMsg = msg.readString(msgLength, "message");

    if ((Ea::taParty != nullptr) && (partyTab != nullptr))
    {
        const PartyMember *const member = Ea::taParty->getMember(id);
        if (member != nullptr)
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
    if (partyTab == nullptr)
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

void PartyRecv::processPartyInvited(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const std::string partyName = msg.readString(24, "party name");
    std::string nick;

    if (actorManager != nullptr)
    {
        const Being *const being = actorManager->findBeing(id);
        if (being != nullptr)
        {
            if (being->getType() == ActorType::Player)
                nick = being->getName();
        }
    }

    if (socialWindow != nullptr)
        socialWindow->showPartyInvite(partyName, nick, 0);
}

void PartyRecv::processPartyMove(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("id");
    PartyMember *m = nullptr;
    if (Ea::taParty != nullptr)
        m = Ea::taParty->getMember(id);
    if (m != nullptr)
    {
        msg.readInt32("unused");
        m->setX(msg.readInt16("x"));
        m->setY(msg.readInt16("y"));
        const bool online = msg.readUInt8("online") != 0;
        if (m->getOnline() != online)
            partyTab->showOnline(m->getName(), fromBool(online, Online));
        m->setOnline(online);
        msg.readString(24, "party");
        msg.readString(24, "nick");
        m->setMap(msg.readString(16, "map"));
    }
    else
    {
        msg.readInt32("unused");
        msg.readInt16("x");
        msg.readInt16("y");
        msg.readUInt8("online");
        msg.readString(24, "party");
        msg.readString(24, "nick");
        msg.readString(16, "map");
    }
}

void PartyRecv::processPartyUpdateHp(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("id");
    const int hp = msg.readInt16("hp");
    const int maxhp = msg.readInt16("max hp");
    PartyMember *m = nullptr;
    if (Ea::taParty != nullptr)
        m = Ea::taParty->getMember(id);
    if (m != nullptr)
    {
        m->setHp(hp);
        m->setMaxHp(maxhp);
    }

    // The server only sends this when the member is in range, so
    // lets make sure they get the party hilight.
    if ((actorManager != nullptr) && (Ea::taParty != nullptr))
    {
        if (Being *const b = actorManager->findBeing(id))
            b->setParty(Ea::taParty);
    }
}

}  // namespace TmwAthena
