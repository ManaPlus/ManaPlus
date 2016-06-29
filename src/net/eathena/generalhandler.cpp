/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"
#include "gui/widgets/tabs/chat/partytab.h"

#include "net/eathena/adminhandler.h"
#include "net/eathena/auctionhandler.h"
#include "net/eathena/bankhandler.h"
#include "net/eathena/battlegroundhandler.h"
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
#include "net/eathena/maphandler.h"
#include "net/eathena/mailhandler.h"
#include "net/eathena/markethandler.h"
#include "net/eathena/mercenaryhandler.h"
#include "net/eathena/network.h"
#include "net/eathena/npchandler.h"
#include "net/eathena/partyhandler.h"
#include "net/eathena/pethandler.h"
#include "net/eathena/playerhandler.h"
#include "net/eathena/roulettehandler.h"
#include "net/eathena/searchstorehandler.h"
#include "net/eathena/serverfeatures.h"
#include "net/eathena/tradehandler.h"
#include "net/eathena/skillhandler.h"
#include "net/eathena/questhandler.h"
#include "net/eathena/vendinghandler.h"

#include "resources/db/itemdbstat.h"
#include "resources/db/statdb.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::GeneralHandler *generalHandler;

namespace EAthena
{

GeneralHandler::GeneralHandler() :
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
    mBattleGroundHandler(new BattleGroundHandler),
    mMercenaryHandler(new MercenaryHandler),
    mBuyingStoreHandler(new BuyingStoreHandler),
    mHomunculusHandler(new HomunculusHandler),
    mFriendsHandler(new FriendsHandler),
    mElementalHandler(new ElementalHandler),
    mMapHandler(new MapHandler),
    mMarketHandler(new MarketHandler),
    mVendingHandler(new VendingHandler),
    mRouletteHandler(new RouletteHandler),
    mSearchStoreHandler(new SearchStoreHandler)
{
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

    StatDb::setStatsList(stats);
}

GeneralHandler::~GeneralHandler()
{
    delete2(Network::mInstance);
}

void GeneralHandler::load() const
{
    new Network;
    Network::mInstance->registerHandlers();
}

void GeneralHandler::reload() const
{
    if (Network::mInstance)
        Network::mInstance->disconnect();

    static_cast<LoginHandler*>(mLoginHandler)->clearWorlds();
    const CharServerHandler *const charHandler =
        static_cast<CharServerHandler*>(mCharServerHandler);
    charHandler->setCharCreateDialog(nullptr);
    charHandler->setCharSelectDialog(nullptr);
    static_cast<PartyHandler*>(mPartyHandler)->reload();
}

void GeneralHandler::reloadPartially() const
{
    static_cast<PartyHandler*>(mPartyHandler)->reload();
}

void GeneralHandler::unload() const
{
    clearHandlers();
}

void GeneralHandler::flushSend() const
{
    if (!Network::mInstance)
        return;

    Network::mInstance->flush();
}

void GeneralHandler::flushNetwork() const
{
    if (!Network::mInstance)
        return;

    Network::mInstance->flush();
    Network::mInstance->dispatchMessages();

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

        client->setState(State::ERROR);
    }
}

void GeneralHandler::clearHandlers() const
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

    statusWindow->addAttribute(Attributes::STR,
        // TRANSLATORS: player stat
        _("Strength"), "str", Modifiable_true);
    statusWindow->addAttribute(Attributes::AGI,
        // TRANSLATORS: player stat
        _("Agility"), "agi", Modifiable_true);
    statusWindow->addAttribute(Attributes::VIT,
        // TRANSLATORS: player stat
        _("Vitality"), "vit", Modifiable_true);
    statusWindow->addAttribute(Attributes::INT,
        // TRANSLATORS: player stat
        _("Intelligence"), "int", Modifiable_true);
    statusWindow->addAttribute(Attributes::DEX,
        // TRANSLATORS: player stat
        _("Dexterity"), "dex", Modifiable_true);
    statusWindow->addAttribute(Attributes::LUK,
        // TRANSLATORS: player stat
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
    // TRANSLATORS: player stat
    statusWindow->addAttribute(Attributes::KARMA, _("Karma"));
    // TRANSLATORS: player stat
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
