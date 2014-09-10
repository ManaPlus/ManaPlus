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
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/messagein.h"

#include "net/ea/gui/partytab.h"

#include "utils/delete2.h"

#include "resources/notifytypes.h"

#include "debug.h"

namespace Ea
{

PartyTab *partyTab = nullptr;
Party *taParty = nullptr;

PartyHandler::PartyHandler() :
    mShareExp(Net::PartyShare::UNKNOWN),
    mShareItems(Net::PartyShare::UNKNOWN)
{
    taParty = Party::getParty(1);
}

PartyHandler::~PartyHandler()
{
    delete2(partyTab);
}

void PartyHandler::join(const int partyId A_UNUSED) const
{
}

void PartyHandler::reload()
{
    taParty = Party::getParty(1);
}

void PartyHandler::clear() const
{
    taParty = nullptr;
}

void PartyHandler::processPartyCreate(Net::MessageIn &msg) const
{
    if (msg.readUInt8())
        NotifyManager::notify(NotifyTypes::PARTY_CREATE_FAILED);
    else
        NotifyManager::notify(NotifyTypes::PARTY_CREATED);
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
            if (being->getType() == ActorType::PLAYER)
                nick = being->getName();
        }
    }

    if (socialWindow)
        socialWindow->showPartyInvite(partyName, nick);
}

void PartyHandler::processPartySettingsContinue(const int16_t exp,
                                                const int16_t item)
{
    switch (exp)
    {
        case Net::PartyShare::YES:
            if (mShareExp == Net::PartyShare::YES)
                break;
            mShareExp = Net::PartyShare::YES;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_ON);
            break;
        case Net::PartyShare::NO:
            if (mShareExp == Net::PartyShare::NO)
                break;
            mShareExp = Net::PartyShare::NO;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_OFF);
            break;
        case Net::PartyShare::NOT_POSSIBLE:
            if (mShareExp == Net::PartyShare::NOT_POSSIBLE)
                break;
            mShareExp = Net::PartyShare::NOT_POSSIBLE;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_ERROR);
            break;
        default:
            logger->log("QQQ Unknown party exp option: %d\n", exp);
            break;
    }

    switch (item)
    {
        case Net::PartyShare::YES:
            if (mShareItems == Net::PartyShare::YES)
                break;
            mShareItems = Net::PartyShare::YES;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ON);
            break;
        case Net::PartyShare::NO:
            if (mShareItems == Net::PartyShare::NO)
                break;
            mShareItems = Net::PartyShare::NO;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_OFF);
            break;
        case Net::PartyShare::NOT_POSSIBLE:
            if (mShareItems == Net::PartyShare::NOT_POSSIBLE)
                break;
            mShareItems = Net::PartyShare::NOT_POSSIBLE;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ERROR);
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
        const bool online = msg.readUInt8() != 0;
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
        msg.readUInt8();     // online (if 0)
        msg.readString(24);  // party
        msg.readString(24);  // nick
        msg.readString(16);  // map
    }
}

void PartyHandler::processPartyLeave(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();
    const std::string nick = msg.readString(24);
    msg.readUInt8();     // fail
    if (!localPlayer)
        return;

    if (id == localPlayer->getId())
    {
        if (Ea::taParty)
        {
            Ea::taParty->removeFromMembers();
            Ea::taParty->clearMembers();
        }
        NotifyManager::notify(NotifyTypes::PARTY_LEFT);
        delete2(Ea::partyTab)

        if (socialWindow && Ea::taParty)
            socialWindow->removeTab(Ea::taParty);
        localPlayer->setPartyName("");
    }
    else
    {
        NotifyManager::notify(NotifyTypes::PARTY_USER_LEFT, nick);
        if (actorManager)
        {
            Being *const b = actorManager->findBeing(id);
            if (b && b->getType() == ActorType::PLAYER)
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

ChatTab *PartyHandler::getTab() const
{
    return partyTab;
}

void PartyHandler::createTab() const
{
    Ea::partyTab = new Ea::PartyTab(chatWindow);
    if (config.getBoolValue("showChatHistory"))
        Ea::partyTab->loadFromLogFile("#Party");
}

}  // namespace Ea
