/*
 *  The ManaPlus Client
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

#include "auctionmanager.h"

#include "client.h"
#include "configuration.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "debug.h"

bool AuctionManager::mEnableAuctionBot = false;

AuctionManager::AuctionManager()
{
}

AuctionManager::~AuctionManager()
{
}

void AuctionManager::init()
{
    if (auctionManager)
        return;

    int val = serverConfig.getValue("enableAuctionBot", -1);
    if (val == -1)
    {
        if (client->isTmw())
            val = 1;
        else
            val = 0;
        serverConfig.setValue("enableAuctionBot", val);
    }
    mEnableAuctionBot = (val != 0);
    if (mEnableAuctionBot)
        auctionManager = new AuctionManager();
}

void AuctionManager::send(const std::string &msg)
{
    if (Net::getChatHandler())
        Net::getChatHandler()->privateMessage("AuctionBot", msg);
}

/*
bool AuctionManager::processAuctionMessage(const std::string &msg)
{
    if (msg.size() > 4 && msg[0] == '#' && msg[1] == '#')
        msg = msg.substr(3);
    return false;
}
*/

void AuctionManager::clear()
{
}

void AuctionManager::reload()
{
}

void AuctionManager::sendMail(const std::string &mail)
{
    if (Net::getChatHandler())
        Net::getChatHandler()->privateMessage("AuctionBot", "!mail " + mail);
}
