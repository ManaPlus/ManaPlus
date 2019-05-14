/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "net/tmwa/adminhandler.h"
#include "net/tmwa/beinghandler.h"
#include "net/tmwa/buysellhandler.h"
#include "net/tmwa/chathandler.h"
#include "net/tmwa/clanhandler.h"
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
#include "net/tmwa/serverfeatures.h"
#include "net/tmwa/tradehandler.h"
#include "net/tmwa/skillhandler.h"
#include "net/tmwa/questhandler.h"

#include "net/tmwa/auctionhandler.h"
#include "net/tmwa/achievementhandler.h"
#include "net/tmwa/attendancehandler.h"
#include "net/tmwa/bankhandler.h"
#include "net/tmwa/barterhandler.h"
#include "net/tmwa/battlegroundhandler.h"
#include "net/tmwa/buyingstorehandler.h"
#include "net/tmwa/cashshophandler.h"
#include "net/tmwa/elementalhandler.h"
#include "net/tmwa/familyhandler.h"
#include "net/tmwa/friendshandler.h"
#include "net/tmwa/homunculushandler.h"
#include "net/tmwa/mail2handler.h"
#include "net/tmwa/mailhandler.h"
#include "net/tmwa/maphandler.h"
#include "net/tmwa/markethandler.h"
#include "net/tmwa/mercenaryhandler.h"
#include "net/tmwa/refinehandler.h"
#include "net/tmwa/roulettehandler.h"
#include "net/tmwa/searchstorehandler.h"
#include "net/tmwa/vendinghandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

namespace TmwAthena
{

GeneralHandler::GeneralHandler() :
    mAdminHandler(new AdminHandler),
    mBeingHandler(new BeingHandler),
    mBuySellHandler(new BuySellHandler),
    mCharServerHandler(new CharServerHandler),
    mChatHandler(new ChatHandler),
    mClanHandler(new ClanHandler),
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
    mAuctionHandler(new AuctionHandler),
    mAchievementHandler(new AchievementHandler),
    mAttendanceHandler(new AttendanceHandler),
    mBankHandler(new BankHandler),
    mBattleGroundHandler(new BattleGroundHandler),
    mBuyingStoreHandler(new BuyingStoreHandler),
    mCashShopHandler(new CashShopHandler),
    mElementalHandler(new ElementalHandler),
    mFamilyHandler(new FamilyHandler),
    mFriendsHandler(new FriendsHandler),
    mHomunculusHandler(new HomunculusHandler),
    mMail2Handler(new Mail2Handler),
    mMailHandler(new MailHandler),
    mMapHandler(new MapHandler),
    mMarketHandler(new MarketHandler),
    mBarterHandler(new BarterHandler),
    mMercenaryHandler(new MercenaryHandler),
    mRefineHandler(new RefineHandler),
    mRouletteHandler(new RouletteHandler),
    mSearchStoreHandler(new SearchStoreHandler),
    mVendingHandler(new VendingHandler)
{
    generalHandler = this;
    logger->log("Creating tmwa handler");
}

GeneralHandler::~GeneralHandler()
{
    delete2(Network::mInstance)

    delete2(mAdminHandler)
    delete2(mBeingHandler)
    delete2(mBuySellHandler)
    delete2(mCharServerHandler)
    delete2(mChatHandler)
    delete2(mClanHandler)
    delete2(mGameHandler)
    delete2(mGuildHandler)
    delete2(mInventoryHandler)
    delete2(mItemHandler)
    delete2(mLoginHandler)
    delete2(mNpcHandler)
    delete2(mPartyHandler)
    delete2(mPetHandler)
    delete2(mPlayerHandler)
    delete2(mSkillHandler)
    delete2(mTradeHandler)
    delete2(mQuestHandler)
    delete2(mServerFeatures)
    delete2(mAuctionHandler)
    delete2(mAchievementHandler)
    delete2(mAttendanceHandler)
    delete2(mBankHandler)
    delete2(mBattleGroundHandler)
    delete2(mBuyingStoreHandler)
    delete2(mCashShopHandler)
    delete2(mElementalHandler)
    delete2(mFamilyHandler)
    delete2(mFriendsHandler)
    delete2(mHomunculusHandler)
    delete2(mMail2Handler)
    delete2(mMailHandler)
    delete2(mMapHandler)
    delete2(mMarketHandler)
    delete2(mBarterHandler)
    delete2(mMercenaryHandler)
    delete2(mRefineHandler)
    delete2(mRouletteHandler)
    delete2(mSearchStoreHandler)
    delete2(mVendingHandler)
}

void GeneralHandler::load() const
{
    new Network;
    Network::mInstance->registerHandlers();
}

void GeneralHandler::reload() const
{
    if (Network::mInstance != nullptr)
        Network::mInstance->disconnect();

    static_cast<LoginHandler*>(mLoginHandler)->clearWorlds();
    const CharServerHandler *const charHandler =
        static_cast<CharServerHandler*>(mCharServerHandler);
    charHandler->setCharCreateDialog(nullptr);
    charHandler->setCharSelectDialog(nullptr);
    PartyHandler::reload();
}

void GeneralHandler::reloadPartially() const
{
    PartyHandler::reload();
}

void GeneralHandler::unload() const
{
    clearHandlers();
}

void GeneralHandler::flushSend() const
{
    if (Network::mInstance == nullptr)
        return;

    Network::mInstance->flush();
}

void GeneralHandler::flushNetwork() const
{
    if (Network::mInstance == nullptr)
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

        client->setState(State::ERROR);
    }
    BLOCK_END("GeneralHandler::flushNetwork 3")
}

void GeneralHandler::clearHandlers() const
{
    if (Network::mInstance != nullptr)
        Network::mInstance->clearHandlers();
}

void GeneralHandler::gameStarted() const
{
    if (skillDialog != nullptr)
        skillDialog->loadSkills();
}

void GeneralHandler::gameEnded() const
{
    if (socialWindow != nullptr)
        socialWindow->removeTab(Ea::taParty);

    delete2(partyTab)
}

}  // namespace TmwAthena
