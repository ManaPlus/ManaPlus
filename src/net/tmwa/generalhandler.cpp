/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/tmwa/generalhandler.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "gui/charselectdialog.h"
#include "gui/inventorywindow.h"
#include "gui/registerdialog.h"
#include "gui/skilldialog.h"
#include "gui/socialwindow.h"
#include "gui/statuswindow.h"

#include "net/ea/guildhandler.h"

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
#include "net/tmwa/skillhandler.h"
#include "net/tmwa/questhandler.h"

#include "net/tmwa/gui/guildtab.h"
#include "net/tmwa/gui/partytab.h"

#include "utils/gettext.h"

#include <list>

#include "debug.h"

extern Net::GeneralHandler *generalHandler;

namespace TmwAthena
{

ServerInfo charServer;
ServerInfo mapServer;

GeneralHandler::GeneralHandler() :
    MessageHandler(),
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
    mSkillHandler(new SkillHandler),
    mTradeHandler(new TradeHandler),
    mQuestHandler(new QuestHandler)
{
    static const uint16_t _messages[] =
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
    mNetwork = nullptr;
}

void GeneralHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("GeneralHandler::handleMessage")
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
                    {
                        errorMessage = _("Someone else is trying to use this "
                                         "account.");
                    }
                    else
                    {
                        errorMessage = _("This account is already logged in.");
                    }
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
    BLOCK_END("GeneralHandler::handleMessage")
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
    mNetwork->registerHandler(mSkillHandler.get());
    mNetwork->registerHandler(mTradeHandler.get());
    mNetwork->registerHandler(mPartyHandler.get());
    mNetwork->registerHandler(mQuestHandler.get());
}

void GeneralHandler::reload()
{
    if (mNetwork)
        mNetwork->disconnect();

    static_cast<LoginHandler*>(mLoginHandler.get())->clearWorlds();
    static_cast<CharServerHandler*>(
        mCharHandler.get())->setCharCreateDialog(nullptr);
    static_cast<CharServerHandler*>(
        mCharHandler.get())->setCharSelectDialog(nullptr);

    static_cast<PartyHandler*>(mPartyHandler.get())->reload();
}

void GeneralHandler::reloadPartially()
{
    static_cast<PartyHandler*>(mPartyHandler.get())->reload();
}

void GeneralHandler::unload()
{
    clearHandlers();
}

void GeneralHandler::flushNetwork()
{
    if (!mNetwork)
        return;

    BLOCK_START("GeneralHandler::flushNetwork 1")
    mNetwork->flush();
    BLOCK_END("GeneralHandler::flushNetwork 1")
    mNetwork->dispatchMessages();

    BLOCK_START("GeneralHandler::flushNetwork 3")
    if (mNetwork->getState() == Network::NET_ERROR)
    {
        if (!mNetwork->getError().empty())
            errorMessage = mNetwork->getError();
        else
            errorMessage = _("Got disconnected from server!");

        Client::setState(STATE_ERROR);
    }
    BLOCK_END("GeneralHandler::flushNetwork 3")
}

void GeneralHandler::clearHandlers()
{
    if (mNetwork)
        mNetwork->clearHandlers();
}

void GeneralHandler::processEvent(Channels channel,
                                  const DepricatedEvent &event)
{
    if (channel == CHANNEL_GAME)
    {
        if (event.getName() == EVENT_GUIWINDOWSLOADED)
        {
            if (inventoryWindow)
                inventoryWindow->setSplitAllowed(false);
            if (skillDialog)
                skillDialog->loadSkills();

            if (!statusWindow)
                return;

            // protection against double addition attributes.
            statusWindow->clearAttributes();

            statusWindow->addAttribute(STR, _("Strength"), "str", true, "");
            statusWindow->addAttribute(AGI, _("Agility"), "agi", true, "");
            statusWindow->addAttribute(VIT, _("Vitality"), "vit", true, "");
            statusWindow->addAttribute(INT, _("Intelligence"),
                "int", true, "");
            statusWindow->addAttribute(DEX, _("Dexterity"), "dex", true, "");
            statusWindow->addAttribute(LUK, _("Luck"), "luk", true, "");

            statusWindow->addAttribute(ATK, _("Attack"));
            statusWindow->addAttribute(DEF, _("Defense"));
            statusWindow->addAttribute(MATK, _("M.Attack"));
            statusWindow->addAttribute(MDEF, _("M.Defense"));
            // xgettext:no-c-format
            statusWindow->addAttribute(HIT, _("% Accuracy"));
            // xgettext:no-c-format
            statusWindow->addAttribute(FLEE, _("% Evade"));
            // xgettext:no-c-format
            statusWindow->addAttribute(CRIT, _("% Critical"));
            statusWindow->addAttribute(PlayerInfo::ATTACK_DELAY,
                _("Attack Delay"));
            statusWindow->addAttribute(PlayerInfo::WALK_SPEED,
                _("Walk Delay"));
            statusWindow->addAttribute(PlayerInfo::ATTACK_RANGE,
                _("Attack Range"));
            statusWindow->addAttribute(PlayerInfo::ATTACK_SPEED,
                _("Damage per sec."));
        }
        else if (event.getName() == EVENT_GUIWINDOWSUNLOADING)
        {
            if (socialWindow)
            {
                socialWindow->removeTab(Ea::taGuild);
                socialWindow->removeTab(Ea::taParty);
            }

            delete Ea::guildTab;
            Ea::guildTab = nullptr;

            delete Ea::partyTab;
            Ea::partyTab = nullptr;
        }
    }
}

} // namespace TmwAthena
