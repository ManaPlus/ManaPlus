/*
 *  The Mana Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *
 *  This file is part of The Mana Client.
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

#include "actorspritemanager.h"
#include "event.h"
#include "localplayer.h"
#include "log.h"

#include "gui/socialwindow.h"

#include "net/messagein.h"
#include "net/messageout.h"

#include "net/tmwa/protocol.h"

#include "net/tmwa/gui/partytab.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#define PARTY_ID 1

extern Net::PartyHandler *partyHandler;

namespace TmwAthena
{

PartyTab *partyTab = 0;
Party *taParty;

PartyHandler::PartyHandler():
    mShareExp(PARTY_SHARE_UNKNOWN), mShareItems(PARTY_SHARE_UNKNOWN)
{
    static const Uint16 _messages[] =
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
    taParty = Party::getParty(1);
}

PartyHandler::~PartyHandler()
{
    delete partyTab;
    partyTab = 0;
}

void PartyHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_PARTY_CREATE:
            if (msg.readInt8())
                SERVER_NOTICE(_("Could not create party."))
            else
                SERVER_NOTICE(_("Party successfully created."))
            break;
        case SMSG_PARTY_INFO:
            {
                if (!taParty)
                {
                    logger->log1("error: party empty in SMSG_PARTY_INFO");
                    taParty = Party::getParty(1);
                }
                if (!player_node)
                    logger->log1("error: player_node==0 in SMSG_PARTY_INFO");

                if (taParty)
                    taParty->clearMembers();

                int length = msg.readInt16();
                if (taParty)
                    taParty->setName(msg.readString(24));

                int count = (length - 28) / 46;
                if (player_node && taParty)
                {
                    player_node->setParty(taParty);
                    player_node->setPartyName(taParty->getName());
                }

                for (int i = 0; i < count; i++)
                {
                    int id = msg.readInt32();
                    std::string nick = msg.readString(24);
                    std::string map = msg.readString(16);
                    bool leader = msg.readInt8() == 0;
                    bool online = msg.readInt8() == 0;

                    if (taParty)
                    {
                        PartyMember *member = taParty->addMember(id, nick);
                        if (member)
                        {
                            member->setLeader(leader);
                            member->setOnline(online);
                            member->setMap(map);
                        }
                    }
                }

                if (taParty)
                    taParty->sort();

                if (player_node && taParty)
                {
                    player_node->setParty(taParty);
                    player_node->setPartyName(taParty->getName());
                }
            }
            break;
        case SMSG_PARTY_INVITE_RESPONSE:
            {
                if (!partyTab)
                    break;

                std::string nick = msg.readString(24);

                switch (msg.readInt8())
                {
                    case 0:
                        partyTab->chatLog(strprintf(
                            _("%s is already a member of a party."),
                            nick.c_str()), BY_SERVER);
                        break;
                    case 1:
                        partyTab->chatLog(strprintf(
                            _("%s refused your invitation."),
                            nick.c_str()), BY_SERVER);
                        break;
                    case 2:
                        partyTab->chatLog(strprintf(
                            _("%s is now a member of your party."),
                            nick.c_str()), BY_SERVER);
                        break;
                    case 3:
                        partyTab->chatLog(strprintf(
                            _("%s cant joid your party because party is "
                            "full."), nick.c_str()), BY_SERVER);
                        break;
                    default:
                        partyTab->chatLog(strprintf(
                            _("QQQ Unknown invite response for %s."),
                            nick.c_str()), BY_SERVER);
                        break;
                }
                break;
            }
        case SMSG_PARTY_INVITED:
            {
                int id = msg.readInt32();
                std::string partyName = msg.readString(24);
                std::string nick = "";
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
                break;
            }
        case SMSG_PARTY_SETTINGS:
            {
                if (!partyTab)
                {
                    if (!chatWindow)
                        break;

                    partyTab = new PartyTab();
                }

                // These seem to indicate the sharing mode for exp and items
                short exp = msg.readInt16();
                short item = msg.readInt16();

                if (!partyTab)
                    break;

                switch (exp)
                {
                    case PARTY_SHARE:
                        if (mShareExp == PARTY_SHARE)
                            break;
                        mShareExp = PARTY_SHARE;
                        if (partyTab)
                        {
                            partyTab->chatLog(
                                _("Experience sharing enabled."), BY_SERVER);
                        }
                        break;
                    case PARTY_SHARE_NO:
                        if (mShareExp == PARTY_SHARE_NO)
                            break;
                        mShareExp = PARTY_SHARE_NO;
                        if (partyTab)
                        {
                            partyTab->chatLog(
                                _("Experience sharing disabled."), BY_SERVER);
                        }
                        break;
                    case PARTY_SHARE_NOT_POSSIBLE:
                        if (mShareExp == PARTY_SHARE_NOT_POSSIBLE)
                            break;
                        mShareExp = PARTY_SHARE_NOT_POSSIBLE;
                        if (partyTab)
                        {
                            partyTab->chatLog(
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
                        if (partyTab)
                        {
                            partyTab->chatLog(
                                _("Item sharing enabled."), BY_SERVER);
                        }
                        break;
                    case PARTY_SHARE_NO:
                        if (mShareItems == PARTY_SHARE_NO)
                            break;
                        mShareItems = PARTY_SHARE_NO;
                        if (partyTab)
                        {
                            partyTab->chatLog(
                                _("Item sharing disabled."), BY_SERVER);
                        }
                        break;
                    case PARTY_SHARE_NOT_POSSIBLE:
                        if (mShareItems == PARTY_SHARE_NOT_POSSIBLE)
                            break;
                        mShareItems = PARTY_SHARE_NOT_POSSIBLE;
                        if (partyTab)
                        {
                            partyTab->chatLog(
                                _("Item sharing not possible."), BY_SERVER);
                        }
                        break;
                    default:
                        logger->log("QQQ Unknown party item option: %d\n",
                                    exp);
                        break;
                }
                break;
            }
        case SMSG_PARTY_MOVE:
            {
                int id = msg.readInt32();    // id
                PartyMember *m = 0;
                if (taParty)
                    m = taParty->getMember(id);
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
            break;
        case SMSG_PARTY_LEAVE:
            {
                int id = msg.readInt32();
                std::string nick = msg.readString(24);
                msg.readInt8();     // fail
                if (player_node && id == player_node->getId())
                {
                    if (taParty)
                    {
                        taParty->removeFromMembers();
                        taParty->clearMembers();
                    }
                    SERVER_NOTICE(_("You have left the party."))
                    delete partyTab;
                    partyTab = 0;

                    if (socialWindow && taParty)
                        socialWindow->removeTab(taParty);
                }
                else
                {
                    if (partyTab)
                    {
                        partyTab->chatLog(strprintf(
                            _("%s has left your party."),
                            nick.c_str()), BY_SERVER);
                    }
                    if (actorSpriteManager)
                    {
                        Being *b = actorSpriteManager->findBeing(id);
                        if (b && b->getType() == Being::PLAYER)
                            b->setParty(0);
                    }
                    if (taParty)
                        taParty->removeMember(id);
                }
                break;
            }
        case SMSG_PARTY_UPDATE_HP:
            {
                int id = msg.readInt32();
                int hp = msg.readInt16();
                int maxhp = msg.readInt16();
                PartyMember *m = 0;
                if (taParty)
                    m = taParty->getMember(id);
                if (m)
                {
                    m->setHp(hp);
                    m->setMaxHp(maxhp);
                }

                // The server only sends this when the member is in range, so
                // lets make sure they get the party hilight.
                if (actorSpriteManager && taParty)
                {
                    if (Being *b = actorSpriteManager->findBeing(id))
                        b->setParty(taParty);
                }
            }
            break;
        case SMSG_PARTY_UPDATE_COORDS:
            {
                int id = msg.readInt32(); // id
                PartyMember *m = 0;
                if (taParty)
                    m = taParty->getMember(id);
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
            break;
        case SMSG_PARTY_MESSAGE:
            {
                int msgLength = msg.readInt16() - 8;
                if (msgLength <= 0)
                    return;

                int id = msg.readInt32();
                std::string chatMsg = msg.readString(msgLength);

                if (taParty)
                {
                    PartyMember *member = taParty->getMember(id);
                    if (partyTab)
                    {
                        if (member)
                        {
                            partyTab->chatLog(member->getName(), chatMsg);
                        }
                        else
                        {
                            partyTab->chatLog(strprintf(
                                _("An unknown member tried to say: %s"),
                                chatMsg.c_str()), BY_SERVER);
                        }
                    }
                }
            }
            break;

        default:
            break;
    }
}

void PartyHandler::create(const std::string &name)
{
    MessageOut outMsg(CMSG_PARTY_CREATE);
    outMsg.writeString(name.substr(0, 23), 24);
}

void PartyHandler::join(int partyId _UNUSED_)
{
    // TODO?
}

void PartyHandler::invite(Being *being)
{
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_INVITE);
        outMsg.writeInt32(being->getId());
    }
}

void PartyHandler::invite(const std::string &name)
{
    if (!actorSpriteManager)
        return;

    Being* being = actorSpriteManager->findBeingByName(name, Being::PLAYER);
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_INVITE);
        outMsg.writeInt32(being->getId());
    }
}

void PartyHandler::inviteResponse(const std::string &inviter _UNUSED_,
                                  bool accept)
{
    if (player_node)
    {
        MessageOut outMsg(CMSG_PARTY_INVITED);
        outMsg.writeInt32(player_node->getId());
        outMsg.writeInt32(accept ? 1 : 0);
    }
}

void PartyHandler::leave()
{
    MessageOut outMsg(CMSG_PARTY_LEAVE);
}

void PartyHandler::kick(Being *being)
{
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_KICK);
        outMsg.writeInt32(being->getId());
        outMsg.writeString("", 24); //Unused
    }
}

void PartyHandler::kick(const std::string &name)
{
    if (!taParty)
        return;

    PartyMember *m = taParty->getMember(name);
    if (!m)
    {
        if (partyTab)
        {
            partyTab->chatLog(strprintf(_("%s is not in your party!"),
                              name.c_str()), BY_SERVER);
        }
        return;
    }

    MessageOut outMsg(CMSG_PARTY_KICK);
    outMsg.writeInt32(m->getID());
    outMsg.writeString(name, 24); //Unused
}

void PartyHandler::chat(const std::string &text)
{
    MessageOut outMsg(CMSG_PARTY_MESSAGE);
    outMsg.writeInt16(text.length() + 4);
    outMsg.writeString(text, text.length());
}

void PartyHandler::requestPartyMembers()
{
    // Our eAthena doesn't have this message
    // Not needed anyways
}

void PartyHandler::setShareExperience(PartyShare share)
{
    if (share == PARTY_SHARE_NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(share);
    outMsg.writeInt16(mShareItems);
}

void PartyHandler::setShareItems(PartyShare share)
{
    if (share == PARTY_SHARE_NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(mShareExp);
    outMsg.writeInt16(share);
}

} // namespace TmwAthena
