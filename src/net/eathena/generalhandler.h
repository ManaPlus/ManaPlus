/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_EATHENA_GENERALHANDLER_H
#define NET_EATHENA_GENERALHANDLER_H

#include "net/generalhandler.h"

namespace EAthena
{

class AdminHandler;
class BarterHandler;
class BeingHandler;
class BuySellHandler;
class CharServerHandler;
class ChatHandler;
class ClanHandler;
class GameHandler;
class GuildHandler;
class InventoryHandler;
class ItemHandler;
class LoginHandler;
class NpcHandler;
class PartyHandler;
class PetHandler;
class PlayerHandler;
class SkillHandler;
class TradeHandler;
class QuestHandler;
class ServerFeatures;
class AuctionHandler;
class AchievementHandler;
class AttendanceHandler;
class BankHandler;
class BattleGroundHandler;
class BuyingStoreHandler;
class CashShopHandler;
class ElementalHandler;
class FamilyHandler;
class FriendsHandler;
class HomunculusHandler;
class MailHandler;
class Mail2Handler;
class MapHandler;
class MarketHandler;
class MercenaryHandler;
class RefineHandler;
class RouletteHandler;
class SearchStoreHandler;
class VendingHandler;

class GeneralHandler final : public Net::GeneralHandler
{
    public:
        GeneralHandler();

        A_DELETE_COPY(GeneralHandler)

        ~GeneralHandler() override final;

        void load() const override final;

        void reload() const override final;

        void unload() const override final;

        void flushNetwork() const override final;

        void flushSend() const override final;

        void clearHandlers() const override final;

        void reloadPartially() const override final;

        void gameStarted() const override final;

        void gameEnded() const override final;

    protected:
        AdminHandler *mAdminHandler;
        BeingHandler *mBeingHandler;
        BuySellHandler *mBuySellHandler;
        CharServerHandler *mCharServerHandler;
        ChatHandler *mChatHandler;
        ClanHandler *mClanHandler;
        GameHandler *mGameHandler;
        GuildHandler *mGuildHandler;
        InventoryHandler *mInventoryHandler;
        ItemHandler *mItemHandler;
        LoginHandler *mLoginHandler;
        NpcHandler *mNpcHandler;
        PartyHandler *mPartyHandler;
        PetHandler *mPetHandler;
        PlayerHandler *mPlayerHandler;
        SkillHandler *mSkillHandler;
        TradeHandler *mTradeHandler;
        QuestHandler *mQuestHandler;
        ServerFeatures *mServerFeatures;
        Mail2Handler *mMail2Handler;
        MailHandler *mMailHandler;
        AuctionHandler *mAuctionHandler;
        AchievementHandler *mAchievementHandler;
        AttendanceHandler *mAttendanceHandler;
        CashShopHandler *mCashShopHandler;
        FamilyHandler *mFamilyHandler;
        BankHandler *mBankHandler;
        BattleGroundHandler *mBattleGroundHandler;
        MercenaryHandler *mMercenaryHandler;
        BuyingStoreHandler *mBuyingStoreHandler;
        HomunculusHandler *mHomunculusHandler;
        FriendsHandler *mFriendsHandler;
        ElementalHandler *mElementalHandler;
        MapHandler *mMapHandler;
        MarketHandler *mMarketHandler;
        BarterHandler *mBarterHandler;
        VendingHandler *mVendingHandler;
        RefineHandler *mRefineHandler;
        RouletteHandler *mRouletteHandler;
        SearchStoreHandler *mSearchStoreHandler;
};

}  // namespace EAthena

#endif  // NET_EATHENA_GENERALHANDLER_H
