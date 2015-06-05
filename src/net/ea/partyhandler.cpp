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

#include "net/ea/partyhandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/messagein.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "utils/delete2.h"

#include "resources/notifytypes.h"

#include "debug.h"

namespace Ea
{

Party *taParty = nullptr;
PartyShareT PartyHandler::mShareExp = PartyShare::UNKNOWN;
PartyShareT PartyHandler::mShareItems = PartyShare::UNKNOWN;

PartyHandler::PartyHandler() :
    Net::PartyHandler()
{
    mShareExp = PartyShare::UNKNOWN;
    mShareItems = PartyShare::UNKNOWN;
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

void PartyHandler::processPartyCreate(Net::MessageIn &msg)
{
    if (msg.readUInt8("flag"))
        NotifyManager::notify(NotifyTypes::PARTY_CREATE_FAILED);
    else
        NotifyManager::notify(NotifyTypes::PARTY_CREATED);
}

void PartyHandler::processPartySettingsContinue(Net::MessageIn &msg,
                                                const PartyShareT exp,
                                                const PartyShareT item)
{
    switch (exp)
    {
        case PartyShare::YES:
            if (mShareExp == PartyShare::YES)
                break;
            mShareExp = PartyShare::YES;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_ON);
            break;
        case PartyShare::NO:
            if (mShareExp == PartyShare::NO)
                break;
            mShareExp = PartyShare::NO;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_OFF);
            break;
        case PartyShare::NOT_POSSIBLE:
            if (mShareExp == PartyShare::NOT_POSSIBLE)
                break;
            mShareExp = PartyShare::NOT_POSSIBLE;
            NotifyManager::notify(NotifyTypes::PARTY_EXP_SHARE_ERROR);
            break;
        default:
        case PartyShare::UNKNOWN:
            UNIMPLIMENTEDPACKET;
            break;
    }

    switch (item)
    {
        case PartyShare::YES:
            if (mShareItems == PartyShare::YES)
                break;
            mShareItems = PartyShare::YES;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ON);
            break;
        case PartyShare::NO:
            if (mShareItems == PartyShare::NO)
                break;
            mShareItems = PartyShare::NO;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_OFF);
            break;
        case PartyShare::NOT_POSSIBLE:
            if (mShareItems == PartyShare::NOT_POSSIBLE)
                break;
            mShareItems = PartyShare::NOT_POSSIBLE;
            NotifyManager::notify(NotifyTypes::PARTY_ITEM_SHARE_ERROR);
            break;
        default:
        case PartyShare::UNKNOWN:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void PartyHandler::processPartyLeave(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const std::string nick = msg.readString(24, "nick");
    const int reason = msg.readUInt8("flag");
    if (!localPlayer)
        return;

    if (id == localPlayer->getId())
    {
        switch (reason)
        {
            case 0:
            default:
                NotifyManager::notify(NotifyTypes::PARTY_LEFT);
                break;

            case 1:
                NotifyManager::notify(NotifyTypes::PARTY_KICKED);
                break;

            case 2:
                NotifyManager::notify(NotifyTypes::PARTY_LEFT_DENY);
                break;

            case 3:
                NotifyManager::notify(NotifyTypes::PARTY_KICK_DENY);
                break;
        }

        if (reason >= 2)
            return;

        if (Ea::taParty)
        {
            Ea::taParty->removeFromMembers();
            Ea::taParty->clearMembers();
        }

        delete2(partyTab)

        if (socialWindow && Ea::taParty)
            socialWindow->removeTab(Ea::taParty);
        localPlayer->setPartyName("");
    }
    else
    {
        switch (reason)
        {
            case 0:
            default:
                NotifyManager::notify(NotifyTypes::PARTY_USER_LEFT, nick);
                break;

            case 1:
                NotifyManager::notify(NotifyTypes::PARTY_USER_KICKED, nick);
                break;

            case 2:
                NotifyManager::notify(NotifyTypes::PARTY_USER_LEFT_DENY, nick);
                break;

            case 3:
                NotifyManager::notify(NotifyTypes::PARTY_USER_KICK_DENY, nick);
                break;
        }

        if (reason >= 2)
            return;

        if (actorManager)
        {
            Being *const b = actorManager->findBeing(id);
            if (b && b->getType() == ActorType::Player)
            {
                b->setParty(nullptr);
                b->setPartyName("");
            }
        }
        if (Ea::taParty)
            Ea::taParty->removeMember(id);
    }
}

void PartyHandler::processPartyUpdateCoords(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    PartyMember *m = nullptr;
    if (Ea::taParty)
        m = Ea::taParty->getMember(id);
    if (m)
    {
        m->setX(msg.readInt16("x"));
        m->setY(msg.readInt16("y"));
    }
    else
    {
        msg.readInt16("x");
        msg.readInt16("y");
    }
}

ChatTab *PartyHandler::getTab() const
{
    return partyTab;
}

void PartyHandler::createTab()
{
    partyTab = new PartyTab(chatWindow);
    if (config.getBoolValue("showChatHistory"))
        partyTab->loadFromLogFile("#Party");
}

}  // namespace Ea
