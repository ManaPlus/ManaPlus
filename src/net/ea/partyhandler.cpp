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

#include "net/ea/partyhandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/localplayer.h"

#include "gui/windows/socialwindow.h"

#include "debug.h"

namespace Ea
{

PartyTab *partyTab = nullptr;
Party *taParty = nullptr;

PartyHandler::PartyHandler() :
    mShareExp(PARTY_SHARE_UNKNOWN),
    mShareItems(PARTY_SHARE_UNKNOWN)
{
    taParty = Party::getParty(1);
}

PartyHandler::~PartyHandler()
{
    delete partyTab;
    partyTab = nullptr;
}

void PartyHandler::join(const int partyId A_UNUSED) const
{
}

void PartyHandler::reload() const
{
    taParty = Party::getParty(1);
}

void PartyHandler::clear() const
{
    taParty = nullptr;
}

void PartyHandler::processPartyCreate(Net::MessageIn &msg) const
{
    if (msg.readInt8())
        NotifyManager::notify(NotifyManager::PARTY_CREATE_FAILED);
    else
        NotifyManager::notify(NotifyManager::PARTY_CREATED);
}

void PartyHandler::processPartyInfo(Net::MessageIn &msg) const
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
            onlineNames.insert(player_node->getName());
        }
    }

    if (!player_node)
        logger->log1("error: player_node==0 in SMSG_PARTY_INFO");

    if (Ea::taParty)
        Ea::taParty->clearMembers();

    const int length = msg.readInt16();
    if (Ea::taParty)
        Ea::taParty->setName(msg.readString(24));

    const int count = (length - 28) / 46;
    if (player_node && Ea::taParty)
    {
        player_node->setParty(Ea::taParty);
        player_node->setPartyName(Ea::taParty->getName());
    }

    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32();
        std::string nick = msg.readString(24);
        std::string map = msg.readString(16);
        const bool leader = msg.readInt8() == 0;
        const bool online = msg.readInt8() == 0;

        if (Ea::taParty)
        {
            bool joined(false);

            if (isOldParty)
            {
                if (names.find(nick) == names.end())
                {
                    NotifyManager::notify(NotifyManager::PARTY_USER_JOINED,
                        nick);
                    joined = true;
                }
            }
            PartyMember *const member = Ea::taParty->addMember(id, nick);
            if (member)
            {
                if (!joined && Ea::partyTab)
                {
                    if (!names.empty() && ((onlineNames.find(nick)
                        == onlineNames.end() && online)
                        || (onlineNames.find(nick) != onlineNames.end()
                        && !online)))
                    {
                        Ea::partyTab->showOnline(nick, online);
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

    if (player_node && Ea::taParty)
    {
        player_node->setParty(Ea::taParty);
        player_node->setPartyName(Ea::taParty->getName());
        if (socialWindow)
            socialWindow->updateParty();
    }
}

void PartyHandler::processPartyInviteResponse(Net::MessageIn &msg) const
{
    if (!Ea::partyTab)
        return;

    const std::string nick = msg.readString(24);

    switch (msg.readInt8())
    {
        case 0:
            NotifyManager::notify(NotifyManager::PARTY_INVITE_ALREADY_MEMBER,
                nick);
            break;
        case 1:
            NotifyManager::notify(NotifyManager::PARTY_INVITE_REFUSED, nick);
            break;
        case 2:
            NotifyManager::notify(NotifyManager::PARTY_INVITE_DONE, nick);
            break;
        case 3:
            NotifyManager::notify(NotifyManager::PARTY_INVITE_PARTY_FULL,
                nick);
            break;
        default:
            NotifyManager::notify(NotifyManager::PARTY_INVITE_ERROR, nick);
            break;
    }
}

void PartyHandler::processPartyInvited(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();
    const std::string partyName = msg.readString(24);
    std::string nick;

    if (actorManager)
    {
        const Being *const being = actorManager->findBeing(id);
        if (being)
        {
            if (being->getType() == Being::PLAYER)
                nick = being->getName();
        }
    }

    if (socialWindow)
        socialWindow->showPartyInvite(partyName, nick);
}

void PartyHandler::processPartySettings(Net::MessageIn &msg)
{
    if (!Ea::partyTab)
    {
        if (!chatWindow)
            return;

        Ea::partyTab = new PartyTab(chatWindow);
        if (config.getBoolValue("showChatHistory"))
            Ea::partyTab->loadFromLogFile("#Party");
    }

    // These seem to indicate the sharing mode for exp and items
    const int16_t exp = msg.readInt16();
    const int16_t item = msg.readInt16();

    if (!Ea::partyTab)
        return;

    switch (exp)
    {
        case PARTY_SHARE:
            if (mShareExp == PARTY_SHARE)
                break;
            mShareExp = PARTY_SHARE;
            NotifyManager::notify(NotifyManager::PARTY_EXP_SHARE_ON);
            break;
        case PARTY_SHARE_NO:
            if (mShareExp == PARTY_SHARE_NO)
                break;
            mShareExp = PARTY_SHARE_NO;
            NotifyManager::notify(NotifyManager::PARTY_EXP_SHARE_OFF);
            break;
        case PARTY_SHARE_NOT_POSSIBLE:
            if (mShareExp == PARTY_SHARE_NOT_POSSIBLE)
                break;
            mShareExp = PARTY_SHARE_NOT_POSSIBLE;
            NotifyManager::notify(NotifyManager::PARTY_EXP_SHARE_ERROR);
            break;
        default:
            logger->log("QQQ Unknown party exp option: %d\n", exp);
            break;
    }

    switch (item)
    {
        case PARTY_SHARE:
            if (mShareItems == PARTY_SHARE)
                break;
            mShareItems = PARTY_SHARE;
            NotifyManager::notify(NotifyManager::PARTY_ITEM_SHARE_ON);
            break;
        case PARTY_SHARE_NO:
            if (mShareItems == PARTY_SHARE_NO)
                break;
            mShareItems = PARTY_SHARE_NO;
            NotifyManager::notify(NotifyManager::PARTY_ITEM_SHARE_OFF);
            break;
        case PARTY_SHARE_NOT_POSSIBLE:
            if (mShareItems == PARTY_SHARE_NOT_POSSIBLE)
                break;
            mShareItems = PARTY_SHARE_NOT_POSSIBLE;
            NotifyManager::notify(NotifyManager::PARTY_ITEM_SHARE_ERROR);
            break;
        default:
            logger->log("QQQ Unknown party item option: %d\n", exp);
            break;
    }
}

void PartyHandler::processPartyMove(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();  // id
    PartyMember *m = nullptr;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        msg.skip(4);                    // 0
        m->setX(msg.readInt16());       // x
        m->setY(msg.readInt16());       // y
        const int online = msg.readInt8();
        if (m->getOnline() != online)
            Ea::partyTab->showOnline(m->getName(), online);
        m->setOnline(online);           // online (if 0)
        msg.readString(24);             // party
        msg.readString(24);             // nick
        m->setMap(msg.readString(16));  // map
    }
    else
    {
        msg.skip(4);         // 0
        msg.readInt16();     // x
        msg.readInt16();     // y
        msg.readInt8();      // online (if 0)
        msg.readString(24);  // party
        msg.readString(24);  // nick
        msg.readString(16);  // map
    }
}

void PartyHandler::processPartyLeave(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();
    const std::string nick = msg.readString(24);
    msg.readInt8();     // fail
    if (!player_node)
        return;

    if (id == player_node->getId())
    {
        if (Ea::taParty)
        {
            Ea::taParty->removeFromMembers();
            Ea::taParty->clearMembers();
        }
        NotifyManager::notify(NotifyManager::PARTY_LEFT);
        delete Ea::partyTab;
        Ea::partyTab = nullptr;

        if (socialWindow && Ea::taParty)
            socialWindow->removeTab(Ea::taParty);
        player_node->setPartyName("");
    }
    else
    {
        NotifyManager::notify(NotifyManager::PARTY_USER_LEFT, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeing(id);
            if (b && b->getType() == Being::PLAYER)
            {
                b->setParty(nullptr);
                b->setPartyName("");
            }
        }
        if (Ea::taParty)
            Ea::taParty->removeMember(id);
    }
}

void PartyHandler::processPartyUpdateHp(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();
    const int hp = msg.readInt16();
    const int maxhp = msg.readInt16();
    PartyMember *m = nullptr;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        m->setHp(hp);
        m->setMaxHp(maxhp);
    }

    // The server only sends this when the member is in range, so
    // lets make sure they get the party hilight.
    if (actorManager && Ea::taParty)
    {
        if (Being *const b = actorManager->findBeing(id))
            b->setParty(Ea::taParty);
    }
}

void PartyHandler::processPartyUpdateCoords(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();  // id
    PartyMember *m = nullptr;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        m->setX(msg.readInt16());    // x
        m->setY(msg.readInt16());    // y
    }
    else
    {
        msg.readInt16();    // x
        msg.readInt16();    // y
    }
}

void PartyHandler::processPartyMessage(Net::MessageIn &msg) const
{
    const int msgLength = msg.readInt16() - 8;
    if (msgLength <= 0)
        return;

    const int id = msg.readInt32();
    const std::string chatMsg = msg.readString(msgLength);

    if (Ea::taParty && Ea::partyTab)
    {
        const PartyMember *const member = Ea::taParty->getMember(id);
        if (member)
        {
            Ea::partyTab->chatLog(member->getName(), chatMsg);
        }
        else
        {
            NotifyManager::notify(NotifyManager::PARTY_UNKNOWN_USER_MSG,
                chatMsg);
        }
    }
}

ChatTab *PartyHandler::getTab() const
{
    return partyTab;
}

}  // namespace Ea
