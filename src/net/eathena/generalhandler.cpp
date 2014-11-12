/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/generalhandler.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "being/attributes.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"

#include "net/ea/guildhandler.h"

#include "gui/widgets/tabs/chat/guildtab.h"
#include "gui/widgets/tabs/chat/partytab.h"

#include "net/eathena/adminhandler.h"
#include "net/eathena/auctionhandler.h"
#include "net/eathena/bankhandler.h"
#include "net/eathena/beinghandler.h"
#include "net/eathena/buyingstorehandler.h"
#include "net/eathena/buysellhandler.h"
#include "net/eathena/cashshophandler.h"
#include "net/eathena/chathandler.h"
#include "net/eathena/charserverhandler.h"
#include "net/eathena/elementalhandler.h"
#include "net/eathena/familyhandler.h"
#include "net/eathena/friendshandler.h"
#include "net/eathena/gamehandler.h"
#include "net/eathena/guildhandler.h"
#include "net/eathena/homunculushandler.h"
#include "net/eathena/inventoryhandler.h"
#include "net/eathena/itemhandler.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/mailhandler.h"
#include "net/eathena/mercenaryhandler.h"
#include "net/eathena/network.h"
#include "net/eathena/npchandler.h"
#include "net/eathena/partyhandler.h"
#include "net/eathena/pethandler.h"
#include "net/eathena/playerhandler.h"
#include "net/eathena/protocol.h"
#include "net/eathena/serverfeatures.h"
#include "net/eathena/tradehandler.h"
#include "net/eathena/skillhandler.h"
#include "net/eathena/questhandler.h"

#include "resources/db/itemdbstat.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::GeneralHandler *generalHandler;

namespace EAthena
{

ServerInfo charServer;
ServerInfo mapServer;

GeneralHandler::GeneralHandler() :
    MessageHandler(),
    mAdminHandler(new AdminHandler),
    mBeingHandler(new BeingHandler(config.getBoolValue("EnableSync"))),
    mBuySellHandler(new BuySellHandler),
    mCharServerHandler(new CharServerHandler),
    mChatHandler(new ChatHandler),
    mGameHandler(new GameHandler),
    mGuildHandler(new GuildHandler),
    mInventoryHandler(new InventoryHandler),
    mItemHandler(new ItemHandler),
    mLoginHandler(new LoginHandler),
    mNpcHandler(new NpcHandler),
    mPartyHandler(new PartyHandler),
    mPetHandler(new PetHandler),
    mPlayerHandler(new PlayerHandler),
    mSkillHandler(new SkillHandler),
    mTradeHandler(new TradeHandler),
    mQuestHandler(new QuestHandler),
    mServerFeatures(new ServerFeatures),
    mMailHandler(new MailHandler),
    mAuctionHandler(new AuctionHandler),
    mCashShopHandler(new CashShopHandler),
    mFamilyHandler(new FamilyHandler),
    mBankHandler(new BankHandler),
    mMercenaryHandler(new MercenaryHandler),
    mBuyingStoreHandler(new BuyingStoreHandler),
    mHomunculusHandler(new HomunculusHandler),
    mFriendsHandler(new FriendsHandler),
    mElementalHandler(new ElementalHandler)
{
    static const uint16_t _messages[] =
    {
        SMSG_CONNECTION_PROBLEM,
        SMSG_MAP_NOT_FOUND,
        0
    };
    handledMessages = _messages;
    generalHandler = this;

    std::vector<ItemDB::Stat> stats;
    stats.push_back(ItemDB::Stat("str", _("Strength %s")));
    stats.push_back(ItemDB::Stat("agi", _("Agility %s")));
    stats.push_back(ItemDB::Stat("vit", _("Vitality %s")));
    stats.push_back(ItemDB::Stat("int", _("Intelligence %s")));
    stats.push_back(ItemDB::Stat("dex", _("Dexterity %s")));
    stats.push_back(ItemDB::Stat("luck", _("Luck %s")));

    ItemDB::setStatsList(stats);
}

GeneralHandler::~GeneralHandler()
{
    delete2(mNetwork);
}

void GeneralHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_CONNECTION_PROBLEM:
            processConnectionProblem(msg);
            break;

        case SMSG_MAP_NOT_FOUND:
            processMapNotFound(msg);
            break;

        default:
            break;
    }
}

void GeneralHandler::processConnectionProblem(Net::MessageIn &msg)
{
    const uint8_t code = msg.readUInt8("flag");
    logger->log("Connection problem: %u", static_cast<unsigned int>(code));

    switch (code)
    {
        case 0:
            errorMessage = _("Authentication failed.");
            break;
        case 1:
            errorMessage = _("No servers available.");
            break;
        case 2:
            if (client->getState() == STATE_GAME)
            {
                errorMessage = _("Someone else is trying to use "
                    "this account.");
            }
            else
            {
                errorMessage = _("This account is already logged in.");
            }
            break;
        case 3:
            errorMessage = _("Speed hack detected.");
            break;
        case 4:
            errorMessage = _("Server full.");
            break;
        case 5:
            errorMessage = _("Sorry, you are underaged.");
            break;
        case 8:
            errorMessage = _("Duplicated login.");
            break;
        case 9:
            errorMessage = _("To many connections from same ip.");
            break;
        case 10:
            errorMessage = _("Not paid for this time.");
            break;
        case 11:
            errorMessage = _("Pay suspended.");
            break;
        case 12:
            errorMessage = _("Pay changed.");
            break;
        case 13:
            errorMessage = _("Pay wrong ip.");
            break;
        case 14:
            errorMessage = _("Pay game room.");
            break;
        case 15:
            errorMessage = _("Disconnect forced by GM.");
            break;
        case 16:
        case 17:
            errorMessage = _("Ban japan refuse.");
            break;
        case 18:
            errorMessage = _("Remained other account.");
            break;
        case 100:
            errorMessage = _("Ip unfair.");
            break;
        case 101:
            errorMessage = _("Ip count all.");
            break;
        case 102:
            errorMessage = _("Ip count.");
            break;
        case 103:
        case 104:
            errorMessage = _("Memory.");
            break;
        case 105:
            errorMessage = _("Han valid.");
            break;
        case 106:
            errorMessage = _("Ip limited access.");
            break;
        case 107:
            errorMessage = _("Over characters list.");
            break;
        case 108:
            errorMessage = _("Ip blocked.");
            break;
        case 109:
            errorMessage = _("Invalid password count.");
            break;
        case 110:
            errorMessage = _("Not allowed race.");
            break;
        default:
            errorMessage = _("Unknown connection error.");
            break;
    }
    client->setState(STATE_ERROR);
}

void GeneralHandler::processMapNotFound(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 4;
    msg.readString(sz, "map name?");
    errorMessage = _("Map not found");
    client->setState(STATE_ERROR);
}

void GeneralHandler::load()
{
    (new Network)->registerHandler(this);

    if (!mNetwork)
        return;

    mNetwork->registerHandler(mAdminHandler);
    mNetwork->registerHandler(mBeingHandler);
    mNetwork->registerHandler(mBuySellHandler);
    mNetwork->registerHandler(mChatHandler);
    mNetwork->registerHandler(mCharServerHandler);
    mNetwork->registerHandler(mGameHandler);
    mNetwork->registerHandler(mGuildHandler);
    mNetwork->registerHandler(mInventoryHandler);
    mNetwork->registerHandler(mItemHandler);
    mNetwork->registerHandler(mLoginHandler);
    mNetwork->registerHandler(mNpcHandler);
    mNetwork->registerHandler(mPlayerHandler);
    mNetwork->registerHandler(mSkillHandler);
    mNetwork->registerHandler(mTradeHandler);
    mNetwork->registerHandler(mPartyHandler);
    mNetwork->registerHandler(mPetHandler);
    mNetwork->registerHandler(mQuestHandler);
    mNetwork->registerHandler(mMailHandler);
    mNetwork->registerHandler(mAuctionHandler);
    mNetwork->registerHandler(mCashShopHandler);
    mNetwork->registerHandler(mFamilyHandler);
    mNetwork->registerHandler(mBankHandler);
    mNetwork->registerHandler(mMercenaryHandler);
    mNetwork->registerHandler(mBuyingStoreHandler);
    mNetwork->registerHandler(mHomunculusHandler);
    mNetwork->registerHandler(mFriendsHandler);
    mNetwork->registerHandler(mElementalHandler);
}

void GeneralHandler::reload()
{
    if (mNetwork)
        mNetwork->disconnect();

    static_cast<LoginHandler*>(mLoginHandler)->clearWorlds();
    CharServerHandler *const charHandler = static_cast<CharServerHandler*>(
        mCharServerHandler);
    charHandler->setCharCreateDialog(nullptr);
    charHandler->setCharSelectDialog(nullptr);
    static_cast<PartyHandler*>(mPartyHandler)->reload();
}

void GeneralHandler::reloadPartially() const
{
    static_cast<PartyHandler*>(mPartyHandler)->reload();
}

void GeneralHandler::unload()
{
    clearHandlers();
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

        client->setState(STATE_ERROR);
    }
}

void GeneralHandler::clearHandlers()
{
    if (mNetwork)
        mNetwork->clearHandlers();
}

void GeneralHandler::gameStarted() const
{
    if (inventoryWindow)
        inventoryWindow->setSplitAllowed(false);
    if (skillDialog)
        skillDialog->loadSkills();

    if (!statusWindow)
        return;

    // protection against double addition attributes.
    statusWindow->clearAttributes();

    statusWindow->addAttribute(Attributes::STR, _("Strength"), "str", true);
    statusWindow->addAttribute(Attributes::AGI, _("Agility"), "agi", true);
    statusWindow->addAttribute(Attributes::VIT, _("Vitality"), "vit", true);
    statusWindow->addAttribute(Attributes::INT,
        _("Intelligence"), "int", true);
    statusWindow->addAttribute(Attributes::DEX, _("Dexterity"), "dex", true);
    statusWindow->addAttribute(Attributes::LUK, _("Luck"), "luk", true);

    statusWindow->addAttribute(Attributes::ATK, _("Attack"));
    statusWindow->addAttribute(Attributes::DEF, _("Defense"));
    statusWindow->addAttribute(Attributes::MATK, _("M.Attack"));
    statusWindow->addAttribute(Attributes::MDEF, _("M.Defense"));
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::HIT, _("% Accuracy"));
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::FLEE, _("% Evade"));
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::CRIT, _("% Critical"));
    statusWindow->addAttribute(Attributes::ATTACK_DELAY, _("Attack Delay"));
    statusWindow->addAttribute(Attributes::WALK_SPEED, _("Walk Delay"));
    statusWindow->addAttribute(Attributes::ATTACK_RANGE, _("Attack Range"));
    statusWindow->addAttribute(Attributes::ATTACK_SPEED, _("Damage per sec."));
    statusWindow->addAttribute(Attributes::KARMA, _("Karma"));
    statusWindow->addAttribute(Attributes::MANNER, _("Manner"));
}

void GeneralHandler::gameEnded() const
{
    if (socialWindow)
    {
        socialWindow->removeTab(Ea::taGuild);
        socialWindow->removeTab(Ea::taParty);
    }

    delete2(guildTab);
    delete2(partyTab);
}

}  // namespace EAthena
