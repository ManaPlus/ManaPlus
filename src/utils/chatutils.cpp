/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "utils/chatutils.h"

#include "guild.h"
#include "guildmanager.h"

#include "being/localplayer.h"

#include "gui/chatconsts.h"

#include "gui/widgets/tabs/whispertab.h"
#include "gui/widgets/tabs/chattabtype.h"

#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"

#include "debug.h"

extern unsigned int tmwServerVersion;

void outStringNormal(ChatTab *const tab,
                     const std::string &str,
                     const std::string &def)
{
    if (!localPlayer)
        return;

    if (!tab)
    {
        Net::getChatHandler()->talk(str, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTabType::PARTY:
        {
            Net::getPartyHandler()->chat(str);
            break;
        }
        case ChatTabType::GUILD:
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                {
                    if (tmwServerVersion > 0)
                        return;
                    Net::getGuildHandler()->chat(guild->getId(), str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
            }
            break;
        }
        case ChatTabType::WHISPER:
        {
            const WhisperTab *const whisper
                = static_cast<const WhisperTab *const>(tab);
            tab->chatLog(localPlayer->getName(), str);
            Net::getChatHandler()->privateMessage(whisper->getNick(), str);
            break;
        }
        default:
            Net::getChatHandler()->talk(def, GENERAL_CHANNEL);
            break;
    }
}
