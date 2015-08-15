/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/ea/adminhandler.h"

#include "notifymanager.h"

#include "enums/resources/notifytypes.h"

#include "gui/chatconsts.h"

#include "net/chathandler.h"
#include "net/messagein.h"
#include "net/serverfeatures.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace Ea
{

void AdminHandler::kickName(const std::string &name) const
{
    chatHandler->talk("@kick " + name, GENERAL_CHANNEL);
}

void AdminHandler::ban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::banName(const std::string &name) const
{
    chatHandler->talk("@ban " + name, GENERAL_CHANNEL);
}

void AdminHandler::unban(const int playerId A_UNUSED) const
{
    // Not supported
}

void AdminHandler::unbanName(const std::string &name) const
{
    chatHandler->talk("@unban " + name, GENERAL_CHANNEL);
}

void AdminHandler::reviveName(const std::string &name) const
{
    chatHandler->talk("@revive " + name, GENERAL_CHANNEL);
}

void AdminHandler::ipcheckName(const std::string &name) const
{
    chatHandler->talk("@ipcheck " + name, GENERAL_CHANNEL);
}

void AdminHandler::createItems(const int id,
                               const ItemColor color,
                               const int amount) const
{
    if (!serverFeatures->haveItemColors())
    {
        chatHandler->talk(strprintf("@item %d %d",
            id, amount), GENERAL_CHANNEL);
    }
    else
    {
        chatHandler->talk(strprintf("@item %d %d %d",
            id, toInt(color, int), amount), GENERAL_CHANNEL);
    }
}

void AdminHandler::processKickAck(Net::MessageIn &msg)
{
    if (msg.readInt32("flag") == 0)
        NotifyManager::notify(NotifyTypes::KICK_FAIL);
    else
        NotifyManager::notify(NotifyTypes::KICK_SUCCEED);
}

}  // namespace Ea
