/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/tmwa/generalhandler.h"

#include "client.h"
#include "configuration.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"
#include "gui/widgets/tabs/chat/partytab.h"

#include "net/tmwa/generalrecv.h"

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
#include "net/tmwa/pethandler.h"
#include "net/tmwa/playerhandler.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/serverfeatures.h"
#include "net/tmwa/tradehandler.h"
#include "net/tmwa/skillhandler.h"
#include "net/tmwa/questhandler.h"

#ifdef EATHENA_SUPPORT
#include "net/tmwa/auctionhandler.h"
#include "net/tmwa/bankhandler.h"
#include "net/tmwa/battlegroundhandler.h"
#include "net/tmwa/buyingstorehandler.h"
#include "net/tmwa/cashshophandler.h"
#include "net/tmwa/elementalhandler.h"
#include "net/tmwa/familyhandler.h"
#include "net/tmwa/friendshandler.h"
#include "net/tmwa/homunculushandler.h"
#include "net/tmwa/mailhandler.h"
#include "net/tmwa/maphandler.h"
#include "net/tmwa/markethandler.h"
#include "net/tmwa/mercenaryhandler.h"
#include "net/tmwa/roulettehandler.h"
#include "net/tmwa/searchstorehandler.h"
#include "net/tmwa/vendinghandler.h"
#endif

#include "resources/db/itemdbstat.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::GeneralHandler *generalHandler;

namespace TmwAthena
{

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
#ifdef EATHENA_SUPPORT
    mServerFeatures(new ServerFeatures),
    mAuctionHandler(new AuctionHandler),
    mBankHandler(new BankHandler),
    mBattleGroundHandler(new BattleGroundHandler),
    mBuyingStoreHandler(new BuyingStoreHandler),
    mCashShopHandler(new CashShopHandler),
    mElementalHandler(new ElementalHandler),
    mFamilyHandler(new FamilyHandler),
    mFriendsHandler(new FriendsHandler),
    mHomunculusHandler(new HomunculusHandler),
    mMailHandler(new MailHandler),
    mMapHandler(new MarketHandler),
    mMarketHandler(new MarketHandler),
    mMercenaryHandler(new MercenaryHandler),
    mRouletteHandler(new RouletteHandler),
    mSearchStoreHandler(new SearchStoreHandler),
    mVendingHandler(new VendingHandler)
#else
    mServerFeatures(new ServerFeatures)
#endif
{
    static const uint16_t _messages[] =
    {
        SMSG_CONNECTION_PROBLEM,
        0
    };
    handledMessages = _messages;
    generalHandler = this;

    std::vector<ItemDB::Stat> stats;
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("str", _("Strength %s")));
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("agi", _("Agility %s")));
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("vit", _("Vitality %s")));
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("int", _("Intelligence %s")));
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("dex", _("Dexterity %s")));
    // TRANSLATORS: player stat
    stats.push_back(ItemDB::Stat("luck", _("Luck %s")));

    ItemDB::setStatsList(stats);
}

GeneralHandler::~GeneralHandler()
{
    delete2(Network::mInstance);
}

void GeneralHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("GeneralHandler::handleMessage")

    switch (msg.getId())
    {
        case SMSG_CONNECTION_PROBLEM:
            GeneralRecv::processConnectionProblem(msg);
            break;

        default:
            break;
    }
    BLOCK_END("GeneralHandler::handleMessage")
}

void GeneralHandler::load()
{
    new Network;
    Network::mInstance->registerHandlers();
}

void GeneralHandler::reload()
{
    if (Network::mInstance)
        Network::mInstance->disconnect();

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

void GeneralHandler::flushSend()
{
    if (!Network::mInstance)
        return;

    Network::mInstance->flush();
}

void GeneralHandler::flushNetwork()
{
    if (!Network::mInstance)
        return;

    BLOCK_START("GeneralHandler::flushNetwork 1")
    Network::mInstance->flush();
    BLOCK_END("GeneralHandler::flushNetwork 1")
    Network::mInstance->dispatchMessages();

    BLOCK_START("GeneralHandler::flushNetwork 3")
    if (Network::mInstance->getState() == Network::NET_ERROR)
    {
        if (!Network::mInstance->getError().empty())
        {
            errorMessage = Network::mInstance->getError();
        }
        else
        {
            // TRANSLATORS: error message
            errorMessage = _("Got disconnected from server!");
        }

        client->setState(STATE_ERROR);
    }
    BLOCK_END("GeneralHandler::flushNetwork 3")
}

void GeneralHandler::clearHandlers()
{
    if (Network::mInstance)
        Network::mInstance->clearHandlers();
}

void GeneralHandler::gameStarted() const
{
    if (skillDialog)
        skillDialog->loadSkills();

    if (!statusWindow)
        return;

    // protection against double addition attributes.
    statusWindow->clearAttributes();

    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::STR,
        _("Strength"), "str", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::AGI,
        _("Agility"), "agi", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::VIT,
        _("Vitality"), "vit", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::INT,
        _("Intelligence"), "int", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::DEX,
        _("Dexterity"), "dex", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::LUK,
        _("Luck"), "luk", Modifiable_true);
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::ATK, _("Attack"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::DEF, _("Defense"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::MATK, _("M.Attack"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::MDEF, _("M.Defense"));
    // TRANSLATORS: player stat
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::HIT, _("% Accuracy"));
    // TRANSLATORS: player stat
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::FLEE, _("% Evade"));
    // TRANSLATORS: player stat
    // xgettext:no-c-format
    statusWindow->addAttribute(Attributes::CRIT, _("% Critical"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::ATTACK_DELAY, _("Attack Delay"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::WALK_SPEED, _("Walk Delay"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::ATTACK_RANGE, _("Attack Range"));
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::ATTACK_SPEED, _("Damage per sec."));
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

}  // namespace TmwAthena
