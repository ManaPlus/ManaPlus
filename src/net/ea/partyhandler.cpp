/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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
#include "localplayer.h"
#include "logger.h"

#include "gui/socialwindow.h"

#include "net/messagein.h"

#include "net/ea/gui/partytab.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

PartyTab *partyTab = 0;
Party *taParty = 0;

PartyHandler::PartyHandler() :
    mShareExp(PARTY_SHARE_UNKNOWN),
    mShareItems(PARTY_SHARE_UNKNOWN)
{
    taParty = Party::getParty(1);
}

PartyHandler::~PartyHandler()
{
    delete partyTab;
    partyTab = 0;
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
    taParty = 0;
}

void PartyHandler::processPartyCreate(Net::MessageIn &msg)
{
    if (msg.readInt8())
        SERVER_NOTICE(_("Could not create party."))
    else
        SERVER_NOTICE(_("Party successfully created."))
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

    int length = msg.readInt16();
    if (Ea::taParty)
        Ea::taParty->setName(msg.readString(24));

    int count = (length - 28) / 46;
    if (player_node && Ea::taParty)
    {
        player_node->setParty(Ea::taParty);
        player_node->setPartyName(Ea::taParty->getName());
    }

    for (int i = 0; i < count; i++)
    {
        int id = msg.readInt32();
        std::string nick = msg.readString(24);
        std::string map = msg.readString(16);
        bool leader = msg.readInt8() == 0;
        bool online = msg.readInt8() == 0;

        if (Ea::taParty)
        {
            PartyMember *member = 0;
            if (oldParty)
            {
                //member = Ea::taParty->getMember(id);
                if (Ea::partyTab && names.find(nick) == names.end())
                {
                    Ea::partyTab->chatLog(strprintf(
                        _("%s has joined your party."),
                        nick.c_str()), BY_SERVER);
                }
            }
            member = Ea::taParty->addMember(id, nick);

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
            Ea::partyTab->chatLog(strprintf(
                _("%s is already a member of a party."),
                nick.c_str()), BY_SERVER);
            break;
        case 1:
            Ea::partyTab->chatLog(strprintf(
                _("%s refused your invitation."),
                nick.c_str()), BY_SERVER);
            break;
        case 2:
            Ea::partyTab->chatLog(strprintf(
                _("%s is now a member of your party."),
                nick.c_str()), BY_SERVER);
            break;
        case 3:
            Ea::partyTab->chatLog(strprintf(
                _("%s can't join your party because party is "
                "full."), nick.c_str()), BY_SERVER);
            break;
        default:
            Ea::partyTab->chatLog(strprintf(
                _("QQQ Unknown invite response for %s."),
                nick.c_str()), BY_SERVER);
            break;
    }
}

void PartyHandler::processPartyInvited(Net::MessageIn &msg)
{
    int id = msg.readInt32();
    std::string partyName = msg.readString(24);
    std::string nick("");
    Being *being;

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

        Ea::partyTab = new PartyTab();
    }

    // These seem to indicate the sharing mode for exp and items
    short exp = msg.readInt16();
    short item = msg.readInt16();

    if (!Ea::partyTab)
        return;

    switch (exp)
    {
        case PARTY_SHARE:
            if (mShareExp == PARTY_SHARE)
                break;
            mShareExp = PARTY_SHARE;
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Experience sharing enabled."), BY_SERVER);
            }
            break;
        case PARTY_SHARE_NO:
            if (mShareExp == PARTY_SHARE_NO)
                break;
            mShareExp = PARTY_SHARE_NO;
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Experience sharing disabled."), BY_SERVER);
            }
            break;
        case PARTY_SHARE_NOT_POSSIBLE:
            if (mShareExp == PARTY_SHARE_NOT_POSSIBLE)
                break;
            mShareExp = PARTY_SHARE_NOT_POSSIBLE;
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Experience sharing not possible."),
                    BY_SERVER);
            }
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
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Item sharing enabled."), BY_SERVER);
            }
            break;
        case PARTY_SHARE_NO:
            if (mShareItems == PARTY_SHARE_NO)
                break;
            mShareItems = PARTY_SHARE_NO;
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Item sharing disabled."), BY_SERVER);
            }
            break;
        case PARTY_SHARE_NOT_POSSIBLE:
            if (mShareItems == PARTY_SHARE_NOT_POSSIBLE)
                break;
            mShareItems = PARTY_SHARE_NOT_POSSIBLE;
            if (Ea::partyTab)
            {
                Ea::partyTab->chatLog(
                    _("Item sharing not possible."), BY_SERVER);
            }
            break;
        default:
            logger->log("QQQ Unknown party item option: %d\n",
                        exp);
            break;
    }
}

void PartyHandler::processPartyMove(Net::MessageIn &msg)
{
    int id = msg.readInt32();    // id
    PartyMember *m = 0;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        msg.skip(4);
        m->setX(msg.readInt16());    // x
        m->setY(msg.readInt16());    // y
        m->setOnline(msg.readInt8());     // online (if 0)
        msg.readString(24); // party
        msg.readString(24); // nick
        m->setMap(msg.readString(16)); // map
    }
    else
    {
        msg.skip(4);
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
    int id = msg.readInt32();
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
        SERVER_NOTICE(_("You have left the party."))
        delete Ea::partyTab;
        Ea::partyTab = 0;

        if (socialWindow && Ea::taParty)
            socialWindow->removeTab(Ea::taParty);
        player_node->setPartyName("");
    }
    else
    {
        if (Ea::partyTab)
        {
            Ea::partyTab->chatLog(strprintf(
                _("%s has left your party."),
                nick.c_str()), BY_SERVER);
        }
        if (actorSpriteManager)
        {
            Being *b = actorSpriteManager->findBeing(id);
            if (b && b->getType() == Being::PLAYER)
            {
                b->setParty(0);
                b->setPartyName("");
            }
        }
        if (Ea::taParty)
            Ea::taParty->removeMember(id);
    }
}

void PartyHandler::processPartyUpdateHp(Net::MessageIn &msg)
{
    int id = msg.readInt32();
    int hp = msg.readInt16();
    int maxhp = msg.readInt16();
    PartyMember *m = 0;
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
        if (Being *b = actorSpriteManager->findBeing(id))
            b->setParty(Ea::taParty);
    }
}  

void PartyHandler::processPartyUpdateCoords(Net::MessageIn &msg)
{
    int id = msg.readInt32(); // id
    PartyMember *m = 0;
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
    int msgLength = msg.readInt16() - 8;
    if (msgLength <= 0)
        return;

    int id = msg.readInt32();
    std::string chatMsg = msg.readString(msgLength);

    if (Ea::taParty && Ea::partyTab)
    {
        PartyMember *member = Ea::taParty->getMember(id);
        if (member)
        {
            Ea::partyTab->chatLog(member->getName(), chatMsg);
        }
        else
        {
            Ea::partyTab->chatLog(strprintf(
                _("An unknown member tried to say: %s"),
                chatMsg.c_str()), BY_SERVER);
        }
    }
}

} // namespace Ea
