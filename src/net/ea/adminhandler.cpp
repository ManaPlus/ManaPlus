/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

namespace Ea
{

void AdminHandler::kick(const std::string &name)
{
    Net::getChatHandler()->talk("@kick " + name);
}

void AdminHandler::ban(int playerId _UNUSED_)
{
    // Not supported
}

void AdminHandler::ban(const std::string &name)
{
    Net::getChatHandler()->talk("@ban " + name);
}

void AdminHandler::unban(int playerId _UNUSED_)
{
    // Not supported
}

void AdminHandler::unban(const std::string &name)
{
    Net::getChatHandler()->talk("@unban " + name);
}

void AdminHandler::mute(int playerId _UNUSED_, int type _UNUSED_,
                        int limit _UNUSED_)
{
    return; // Still looking into this
}

} // namespace Ea
