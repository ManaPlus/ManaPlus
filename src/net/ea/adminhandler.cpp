/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "game.h"

#include "net/serverfeatures.h"

#include "utils/gmfunctions.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace Ea
{

void AdminHandler::kickName(const std::string &name) const
{
    Gm::runCommand("kick", name);
}

void AdminHandler::ban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::banName(const std::string &name) const
{
    Gm::runCommand("ban", name);
}

void AdminHandler::unban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::unbanName(const std::string &name) const
{
    Gm::runCommand("unban", name);
}

void AdminHandler::ipcheckName(const std::string &name) const
{
    Gm::runCommand("ipcheck", name);
}

void AdminHandler::createItems(const int id,
                               const ItemColor color A_UNUSED,
                               const int amount) const
{
    Gm::runCommand("item",
        strprintf("%d %d", id, amount));
}

void AdminHandler::slide(const int x, const int y) const
{
    if (serverFeatures->haveSlide())
    {
        Gm::runCommand("slide",
            strprintf("%d %d", x, y));
    }
    else
    {
        warp(Game::instance()->getCurrentMapName(), x, y);
    }
}

void AdminHandler::spawn(const std::string &name) const
{
    Gm::runCommand("spawn", name);
}

}  // namespace Ea
