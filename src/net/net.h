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

#ifndef NET_NET_H
#define NET_NET_H

/**
 * \namespace Net
 *
 * The network communication layer. It is composed of a host of interfaces that
 * interact with different aspects of the game. They have different
 * implementations depending on the type of server the client is connecting to.
 */

#include "net/serverinfo.h"

namespace Net
{

class AdminHandler;
class BeingHandler;
class CharServerHandler;
class ChatHandler;
class GameHandler;
class GeneralHandler;
class GuildHandler;
class InventoryHandler;
class LoginHandler;
class NpcHandler;
class PartyHandler;
class PetHandler;
class PlayerHandler;
class SkillHandler;
class TradeHandler;
class BuySellHandler;

AdminHandler *getAdminHandler() A_WARN_UNUSED;
BeingHandler *getBeingHandler() A_WARN_UNUSED;
CharServerHandler *getCharServerHandler() A_WARN_UNUSED;
ChatHandler *getChatHandler() A_WARN_UNUSED;
GameHandler *getGameHandler() A_WARN_UNUSED;
GeneralHandler *getGeneralHandler() A_WARN_UNUSED;
GuildHandler *getGuildHandler() A_WARN_UNUSED;
InventoryHandler *getInventoryHandler() A_WARN_UNUSED;
LoginHandler *getLoginHandler() A_WARN_UNUSED;
NpcHandler *getNpcHandler() A_WARN_UNUSED;
PartyHandler *getPartyHandler() A_WARN_UNUSED;
PetHandler *getPetHandler() A_WARN_UNUSED;
PlayerHandler *getPlayerHandler() A_WARN_UNUSED;
SkillHandler *getSkillHandler() A_WARN_UNUSED;
TradeHandler *getTradeHandler() A_WARN_UNUSED;
BuySellHandler *getBuySellHandler() A_WARN_UNUSED;

ServerInfo::Type getNetworkType() A_WARN_UNUSED;

/**
 * Handles server detection and connection
 */
void connectToServer(const ServerInfo &server);

void unload();

}  // namespace Net

#endif  // NET_NET_H
