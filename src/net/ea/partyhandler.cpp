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

#include "gui/widgets/tabs/chat/partytab.h"

#include "utils/delete2.h"

#include "resources/notifytypes.h"

#include "debug.h"

namespace Ea
{

Party *taParty = nullptr;
Net::PartyShare::Type PartyHandler::mShareExp = Net::PartyShare::UNKNOWN;
Net::PartyShare::Type PartyHandler::mShareItems = Net::PartyShare::UNKNOWN;

PartyHandler::PartyHandler() :
    Net::PartyHandler()
{
    mShareExp = Net::PartyShare::UNKNOWN;
    mShareItems = Net::PartyShare::UNKNOWN;
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

void PartyHandler::processPartyLeave(Net::MessageIn &msg)
{
    const int id = msg.readInt32("account id");
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
    const int id = msg.readInt32("id");
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
