/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/ea/adminhandler.h"

#include "net/chathandler.h"
#include "net/net.h"

#include <string>

#include "debug.h"

extern int serverVersion;

namespace Ea
{

const std::string GENERAL_CHANNEL;

void AdminHandler::kickName(const std::string &name) const
{
    Net::getChatHandler()->talk("@kick " + name, GENERAL_CHANNEL);
}

void AdminHandler::ban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::banName(const std::string &name) const
{
    Net::getChatHandler()->talk("@ban " + name, GENERAL_CHANNEL);
}

void AdminHandler::unban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::unbanName(const std::string &name) const
{
    Net::getChatHandler()->talk("@unban " + name, GENERAL_CHANNEL);
}

void AdminHandler::mute(const int playerId A_UNUSED, const int type A_UNUSED,
                        const int limit A_UNUSED) const
{
    return;
}

void AdminHandler::warp(const std::string &map, const int x, const int y) const
{
    Net::getChatHandler()->talk(strprintf(
        "@warp %s %d %d", map.c_str(), x, y), GENERAL_CHANNEL);
}

void AdminHandler::gotoName(const std::string &name) const
{
    Net::getChatHandler()->talk("@goto " + name, GENERAL_CHANNEL);
}

void AdminHandler::recallName(const std::string &name) const
{
    Net::getChatHandler()->talk("@recall " + name, GENERAL_CHANNEL);
}

void AdminHandler::reviveName(const std::string &name) const
{
    Net::getChatHandler()->talk("@revive " + name, GENERAL_CHANNEL);
}

void AdminHandler::ipcheckName(const std::string &name) const
{
    Net::getChatHandler()->talk("@ipcheck " + name, GENERAL_CHANNEL);
}

void AdminHandler::createItems(const int id, const int color,
                               const int amount) const
{
    if (serverVersion < 1)
    {
        Net::getChatHandler()->talk(strprintf("@item %d %d",
            id, amount), GENERAL_CHANNEL);
    }
    else
    {
        Net::getChatHandler()->talk(strprintf("@item %d %d %d",
            id, color, amount), GENERAL_CHANNEL);
    }
}

}  // namespace Ea
