/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "net/tmwa/generalhandler.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "gui/charselectdialog.h"
#include "gui/inventorywindow.h"
#include "gui/register.h"
#include "gui/skilldialog.h"
#include "gui/socialwindow.h"
#include "gui/statuswindow.h"

#include "net/messagein.h"
#include "net/messageout.h"
#include "net/serverinfo.h"

#include "net/ea/guildhandler.h"

#include "net/ea/gui/guildtab.h"
#include "net/ea/gui/partytab.h"

#include "net/tmwa/adminhandler.h"
#include "net/tmwa/beinghandler.h"
#include "net/tmwa/buysellhandler.h"
#include "net/tmwa/chathandler.h"
#include "net/tmwa/charserverhandler.h"
#include "net/tmwa/gamehandler.h"
#include "net/tmwa/guildhandler.h"
#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/itemhandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/network.h"
#include "net/tmwa/npchandler.h"
#include "net/tmwa/partyhandler.h"
#include "net/tmwa/playerhandler.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/tradehandler.h"
#include "net/tmwa/specialhandler.h"

#include "net/tmwa/gui/guildtab.h"
#include "net/tmwa/gui/partytab.h"

#include "resources/itemdb.h"

#include "utils/gettext.h"

#include <list>

#include "debug.h"

extern Net::GeneralHandler *generalHandler;

namespace TmwAthena
{

ServerInfo charServer;
ServerInfo mapServer;

GeneralHandler::GeneralHandler():
    mAdminHandler(new AdminHandler),
    mBeingHandler(new BeingHandler(config.getBoolValue("EnableSync"))),
    mBuySellHandler(new BuySellHandler),
    mCharHandler(new CharServerHandler),
    mChatHandler(new ChatHandler),
    mGameHandler(new GameHandler),
    mGuildHandler(new GuildHandler),
    mInventoryHandler(new InventoryHandler),
    mItemHandler(new ItemHandler),
    mLoginHandler(new LoginHandler),
    mNpcHandler(new NpcHandler),
    mPartyHandler(new PartyHandler),
    mPlayerHandler(new PlayerHandler),
    mSpecialHandler(new SpecialHandler),
    mTradeHandler(new TradeHandler)
{
    static const Uint16 _messages[] =
    {
        SMSG_CONNECTION_PROBLEM,
        0
    };
    handledMessages = _messages;
    generalHandler = this;

    std::vector<ItemDB::Stat> stats;
    stats.push_back(ItemDB::Stat("str", _("Strength %+d")));
    stats.push_back(ItemDB::Stat("agi", _("Agility %+d")));
    stats.push_back(ItemDB::Stat("vit", _("Vitality %+d")));
    stats.push_back(ItemDB::Stat("int", _("Intelligence %+d")));
    stats.push_back(ItemDB::Stat("dex", _("Dexterity %+d")));
    stats.push_back(ItemDB::Stat("luck", _("Luck %+d")));

    ItemDB::setStatsList(stats);

    listen(CHANNEL_GAME);
}

GeneralHandler::~GeneralHandler()
{
    delete mNetwork;
    mNetwork = 0;
}

void GeneralHandler::handleMessage(Net::MessageIn &msg)
{
    int code;

    switch (msg.getId())
    {
        case SMSG_CONNECTION_PROBLEM:
            code = msg.readInt8();
            logger->log("Connection problem: %i", code);

            switch (code)
            {
                case 0:
                    errorMessage = _("Authentication failed.");
                    break;
                case 1:
                    errorMessage = _("No servers available.");
                    break;
                case 2:
                    if (Client::getState() == STATE_GAME)
                        errorMessage = _("Someone else is trying to use this "
                                         "account.");
                    else
                        errorMessage = _("This account is already logged in.");
                    break;
                case 3:
                    errorMessage = _("Speed hack detected.");
                    break;
                case 8:
                    errorMessage = _("Duplicated login.");
                    break;
                default:
                    errorMessage = _("Unknown connection error.");
                    break;
            }
            Client::setState(STATE_ERROR);
            break;

        default:
            break;
    }
}

void GeneralHandler::load()
{
    (new Network)->registerHandler(this);

    if (!mNetwork)
        return;

    mNetwork->registerHandler(mAdminHandler.get());
    mNetwork->registerHandler(mBeingHandler.get());
    mNetwork->registerHandler(mBuySellHandler.get());
    mNetwork->registerHandler(mChatHandler.get());
    mNetwork->registerHandler(mCharHandler.get());
    mNetwork->registerHandler(mGameHandler.get());
    mNetwork->registerHandler(mGuildHandler.get());
    mNetwork->registerHandler(mInventoryHandler.get());
    mNetwork->registerHandler(mItemHandler.get());
    mNetwork->registerHandler(mLoginHandler.get());
    mNetwork->registerHandler(mNpcHandler.get());
    mNetwork->registerHandler(mPlayerHandler.get());
    mNetwork->registerHandler(mSpecialHandler.get());
    mNetwork->registerHandler(mTradeHandler.get());
    mNetwork->registerHandler(mPartyHandler.get());
}

void GeneralHandler::reload()
{
    if (mNetwork)
        mNetwork->disconnect();

    static_cast<LoginHandler*>(mLoginHandler.get())->clearWorlds();
    static_cast<CharServerHandler*>(
        mCharHandler.get())->setCharCreateDialog(0);
    static_cast<CharServerHandler*>(
        mCharHandler.get())->setCharSelectDialog(0);

    static_cast<PartyHandler*>(mPartyHandler.get())->reload();
}

void GeneralHandler::reloadPartially()
{
    static_cast<PartyHandler*>(mPartyHandler.get())->reload();
}

void GeneralHandler::unload()
{
    if (mNetwork)
        mNetwork->clearHandlers();
}

void GeneralHandler::flushNetwork()
{
    if (!mNetwork)
        return;

    mNetwork->flush();
    mNetwork->dispatchMessages();

    if (mNetwork->getState() == Network::NET_ERROR)
    {
        if (!mNetwork->getError().empty())
            errorMessage = mNetwork->getError();
        else
            errorMessage = _("Got disconnected from server!");

        Client::setState(STATE_ERROR);
    }
}

void GeneralHandler::clearHandlers()
{
    if (mNetwork)
        mNetwork->clearHandlers();
}

void GeneralHandler::event(Mana::Channels channel,
                           const Mana::Event &event)
{
    if (channel == Mana::CHANNEL_GAME)
    {
        if (event.getName() == Mana::EVENT_GUIWINDOWSLOADED)
        {
            if (inventoryWindow)
                inventoryWindow->setSplitAllowed(false);
            if (skillDialog)
                skillDialog->loadSkills("ea-skills.xml");

            if (!statusWindow)
                return;

            statusWindow->addAttribute(STR, _("Strength"), true, "");
            statusWindow->addAttribute(AGI, _("Agility"), true, "");
            statusWindow->addAttribute(VIT, _("Vitality"), true, "");
            statusWindow->addAttribute(INT, _("Intelligence"), true, "");
            statusWindow->addAttribute(DEX, _("Dexterity"), true, "");
            statusWindow->addAttribute(LUK, _("Luck"), true, "");

            statusWindow->addAttribute(ATK, _("Attack"), false, "");
            statusWindow->addAttribute(DEF, _("Defense"), false, "");
            statusWindow->addAttribute(MATK, _("M.Attack"), false, "");
            statusWindow->addAttribute(MDEF, _("M.Defense"), false, "");
            // xgettext:no-c-format
            statusWindow->addAttribute(HIT, _("% Accuracy"), false, "");
            // xgettext:no-c-format
            statusWindow->addAttribute(FLEE, _("% Evade"), false, "");
            // xgettext:no-c-format
            statusWindow->addAttribute(CRIT, _("% Critical"), false, "");
            statusWindow->addAttribute(ATTACK_DELAY, _("Attack Delay"),
                                       false, "");
            statusWindow->addAttribute(WALK_SPEED, _("Walk Delay"),
                                       false, "");
            statusWindow->addAttribute(ATTACK_RANGE, _("Attack Range"),
                                       false, "");
            statusWindow->addAttribute(ATTACK_SPEED, _("Damage per sec."),
                                       false, "");
        }
        else if (event.getName() == Mana::EVENT_GUIWINDOWSUNLOADING)
        {
            if (socialWindow)
            {
                socialWindow->removeTab(Ea::taGuild);
                socialWindow->removeTab(Ea::taParty);
            }

            delete Ea::guildTab;
            Ea::guildTab = 0;

            delete Ea::partyTab;
            Ea::partyTab = 0;
        }
    }
}

} // namespace TmwAthena
