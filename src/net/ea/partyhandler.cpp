/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "actorspritemanager.h"
#include "configuration.h"
#include "localplayer.h"
#include "notifymanager.h"

#include "gui/socialwindow.h"

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

void PartyHandler::join(int partyId A_UNUSED)
{
    // TODO?
}

void PartyHandler::requestPartyMembers() const
{
    // Our eAthena doesn't have this message
    // Not needed anyways
}

void PartyHandler::reload()
{
    taParty = Party::getParty(1);
}

void PartyHandler::clear()
{
    taParty = nullptr;
}

void PartyHandler::processPartyCreate(Net::MessageIn &msg)
{
    if (msg.readInt8())
        NotifyManager::notify(NotifyManager::PARTY_CREATE_FAILED);
    else
        NotifyManager::notify(NotifyManager::PARTY_CREATED);
}

void PartyHandler::processPartyInfo(Net::MessageIn &msg)
{
    bool oldParty = false;
    std::set<std::string> names;
    if (!Ea::taParty)
    {
        logger->log1("error: party empty in SMSG_PARTY_INFO");
        Ea::taParty = Party::getParty(1);
    }
    if (Ea::taParty)
    {
        if (Ea::taParty->getNumberOfElements() > 1)
        {
            oldParty = true;

            Ea::taParty->getNamesSet(names);
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
            if (oldParty)
            {
                //member = Ea::taParty->getMember(id);
                if (names.find(nick) == names.end())
                {
                    NotifyManager::notify(NotifyManager::PARTY_USER_JOINED,
                        nick);
                }
            }
            PartyMember *member = Ea::taParty->addMember(id, nick);

            if (member)
            {
                member->setLeader(leader);
                member->setOnline(online);
                member->setMap(map);
            }
        }
    }

    if (Ea::taParty)
        Ea::taParty->sort();

    if (player_node && Ea::taParty)
    {
        player_node->setParty(Ea::taParty);
        player_node->setPartyName(Ea::taParty->getName());
    }
}

void PartyHandler::processPartyInviteResponse(Net::MessageIn &msg)
{
    if (!Ea::partyTab)
        return;

    std::string nick = msg.readString(24);

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

void PartyHandler::processPartyInvited(Net::MessageIn &msg)
{
    const int id = msg.readInt32();
    std::string partyName = msg.readString(24);
    std::string nick("");
    const Being *being;

    if (actorSpriteManager)
    {
        if ((being = actorSpriteManager->findBeing(id)))
        {
            if (being && being->getType() == Being::PLAYER)
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
    const short exp = msg.readInt16();
    const short item = msg.readInt16();

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

void PartyHandler::processPartyMove(Net::MessageIn &msg)
{
    const int id = msg.readInt32();    // id
    PartyMember *m = nullptr;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        msg.skip(4);        // 0
        m->setX(msg.readInt16());    // x
        m->setY(msg.readInt16());    // y
        m->setOnline(msg.readInt8());     // online (if 0)
        msg.readString(24); // party
        msg.readString(24); // nick
        m->setMap(msg.readString(16)); // map
    }
    else
    {
        msg.skip(4);        // 0
        msg.readInt16();    // x
        msg.readInt16();    // y
        msg.readInt8();     // online (if 0)
        msg.readString(24); // party
        msg.readString(24); // nick
        msg.readString(16); // map
    }
}

void PartyHandler::processPartyLeave(Net::MessageIn &msg)
{
    const int id = msg.readInt32();
    std::string nick = msg.readString(24);
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
        if (actorSpriteManager)
        {
            Being *const b = actorSpriteManager->findBeing(id);
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

void PartyHandler::processPartyUpdateHp(Net::MessageIn &msg)
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
    if (actorSpriteManager && Ea::taParty)
    {
        if (Being *const b = actorSpriteManager->findBeing(id))
            b->setParty(Ea::taParty);
    }
}  

void PartyHandler::processPartyUpdateCoords(Net::MessageIn &msg)
{
    const int id = msg.readInt32(); // id
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

void PartyHandler::processPartyMessage(Net::MessageIn &msg)
{
    const int msgLength = msg.readInt16() - 8;
    if (msgLength <= 0)
        return;

    const int id = msg.readInt32();
    std::string chatMsg = msg.readString(msgLength);

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

ChatTab *PartyHandler::getTab()
{
    return partyTab;
}

} // namespace Ea
