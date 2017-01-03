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

#include "net/ea/chathandler.h"

#include "configuration.h"

#include "net/ea/chatrecv.h"

#include "debug.h"

namespace Ea
{

ChatHandler::ChatHandler()
{
    if (!ChatRecv::mSentWhispers.empty())
        ChatRecv::mSentWhispers.pop();
    ChatRecv::mMotdTime = 0;
    ChatRecv::mShowAllLang = serverConfig.getValue("showAllLang", 0);
    ChatRecv::mShowMotd = config.getBoolValue("showmotd");
    ChatRecv::mSkipping = true;
}

void ChatHandler::clear() const
{
    ChatRecv::mShowMotd = config.getBoolValue("showmotd");
    ChatRecv::mSkipping = true;
}

void ChatHandler::me(const std::string &restrict text,
                     const std::string &restrict channel) const
{
    // here need string duplication
    std::string action = strprintf("*%s*", text.c_str());
    talk(action, channel);
}

}  // namespace Ea
