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

#include "net/net.h"

#include "configuration.h"

#include "net/loginhandler.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/generalhandler.h"
#endif  // TMWA_SUPPORT

#include "net/eathena/generalhandler.h"

#include "utils/delete2.h"

#include "debug.h"

int evolPacketOffset = 0;

namespace Net
{
    class AuctionHandler;
    class AchievementHandler;
    class AttendanceHandler;
    class BankHandler;
    class BarterHandler;
    class BattleGroundHandler;
    class BuyingStoreHandler;
    class CashShopHandler;
    class ElementalHandler;
    class FamilyHandler;
    class FriendsHandler;
    class HomunculusHandler;
    class Mail2Handler;
    class MailHandler;
    class MapHandler;
    class MarketHandler;
    class MercenaryHandler;
    class SearchStoreHandler;
    class RouletteHandler;
    class VendingHandler;
    class AdminHandler;
    class BeingHandler;
    class BuySellHandler;
    class CharServerHandler;
    class ChatHandler;
    class ClanHandler;
    class GameHandler;
    class GuildHandler;
    class InventoryHandler;
    class NpcHandler;
    class PartyHandler;
    class PetHandler;
    class PlayerHandler;
    class QuestHandler;
    class ServerFeatures;
    class SkillHandler;
    class TradeHandler;
}  // namespace Net

Net::AdminHandler *adminHandler = nullptr;
Net::CharServerHandler *charServerHandler = nullptr;
Net::ChatHandler *chatHandler = nullptr;
Net::ClanHandler *clanHandler = nullptr;
Net::GeneralHandler *generalHandler = nullptr;
Net::InventoryHandler *inventoryHandler = nullptr;
Net::LoginHandler *loginHandler = nullptr;
Net::GameHandler *gameHandler = nullptr;
Net::GuildHandler *guildHandler = nullptr;
Net::NpcHandler *npcHandler = nullptr;
Net::PartyHandler *partyHandler = nullptr;
Net::PetHandler *petHandler = nullptr;
Net::PlayerHandler *playerHandler = nullptr;
Net::SkillHandler *skillHandler = nullptr;
Net::TradeHandler *tradeHandler = nullptr;
Net::BeingHandler *beingHandler = nullptr;
Net::BuySellHandler *buySellHandler = nullptr;
Net::ServerFeatures *serverFeatures = nullptr;
Net::AuctionHandler *auctionHandler = nullptr;
Net::AchievementHandler *achievementHandler = nullptr;
Net::AttendanceHandler *attendanceHandler = nullptr;
Net::BankHandler *bankHandler = nullptr;
Net::BattleGroundHandler *battleGroundHandler = nullptr;
Net::BuyingStoreHandler *buyingStoreHandler = nullptr;
Net::CashShopHandler *cashShopHandler = nullptr;
Net::ElementalHandler *elementalHandler = nullptr;
Net::FamilyHandler *familyHandler = nullptr;
Net::FriendsHandler *friendsHandler = nullptr;
Net::HomunculusHandler *homunculusHandler = nullptr;
Net::Mail2Handler *mail2Handler = nullptr;
Net::MailHandler *mailHandler = nullptr;
Net::MapHandler *mapHandler = nullptr;
Net::MarketHandler *marketHandler = nullptr;
Net::BarterHandler *barterHandler = nullptr;
Net::MercenaryHandler *mercenaryHandler = nullptr;
Net::RouletteHandler *rouletteHandler = nullptr;
Net::SearchStoreHandler *searchStoreHandler = nullptr;
Net::VendingHandler *vendingHandler = nullptr;
Net::QuestHandler *questHandler = nullptr;

namespace Net
{
ServerTypeT networkType = ServerType::UNKNOWN;
std::set<int> ignorePackets;

void connectToServer(const ServerInfo &server)
{
    BLOCK_START("Net::connectToServer")
    if (networkType == server.type &&
        generalHandler != nullptr)
    {
        generalHandler->reload();
    }
    else
    {
        if (networkType != ServerType::UNKNOWN &&
            generalHandler != nullptr)
        {
            generalHandler->unload();
            delete2(generalHandler)
        }

        switch (server.type)
        {
            case ServerType::EATHENA:
            case ServerType::EVOL2:
                new EAthena::GeneralHandler;
                break;
            case ServerType::TMWATHENA:
            case ServerType::UNKNOWN:
            default:
#ifdef TMWA_SUPPORT
                new TmwAthena::GeneralHandler;
#else  // TMWA_SUPPORT

                new EAthena::GeneralHandler;
#endif  // TMWA_SUPPORT

                break;
        }

        if (generalHandler != nullptr)
            generalHandler->load();

        networkType = server.type;
    }

    if (loginHandler != nullptr)
    {
        loginHandler->setServer(server);
        loginHandler->connect();
    }
    BLOCK_END("Net::connectToServer")
}

void unload()
{
    const GeneralHandler *const handler = generalHandler;
    if (handler != nullptr)
        handler->unload();
    ignorePackets.clear();
    delete2(generalHandler)
}

ServerTypeT getNetworkType()
{
    return networkType;
}

void loadIgnorePackets()
{
    const std::string str = config.getStringValue("ignorelogpackets");
    splitToIntSet(ignorePackets, str, ',');
}

bool isIgnorePacket(const int id)
{
    return ignorePackets.find(id) != ignorePackets.end();
}

}  // namespace Net
