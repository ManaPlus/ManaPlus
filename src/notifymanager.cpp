/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "notifymanager.h"

#include "guildmanager.h"
#include "notifications.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "gui/widgets/tabs/chattab.h"

#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"

#include "resources/db/sounddb.h"

namespace NotifyManager
{
    static ChatTab *getGuildTab()
    {
        const Guild *const guild = player_node->getGuild();
        if (guild)
        {
            if (guild->getServerGuild())
                return Net::getGuildHandler()->getTab();
            else if (guildManager)
                return guildManager->getTab();
        }
        return nullptr;
    }

    static void chatLog(ChatTab *const tab, const std::string &str)
    {
        if (tab)
            tab->chatLog(str, BY_SERVER);
        else if (debugChatTab)
            debugChatTab->chatLog(str, BY_SERVER);
    }

    void notify(const unsigned int message)
    {
        if (message >= TYPE_END || !localChatTab)
            return;
        const NotificationInfo &info = notifications[message];
        switch (info.flags)
        {
            case EMPTY:
                localChatTab->chatLog(gettext(info.text),
                    BY_SERVER);
                break;

            case GUILD:
            {
                if (!player_node)
                    return;
                ChatTab *const tab = getGuildTab();
                chatLog(tab, gettext(info.text));
                break;
            }

            case PARTY:
            {
                ChatTab *const tab = Net::getPartyHandler()->getTab();
                chatLog(tab, gettext(info.text));
                break;
            }

            case SPEECH:
            {
                if (player_node)
                    player_node->setSpeech(gettext(info.text));
                break;
            }

            case INT:
            case STRING:
            case GUILD_STRING:
            case PARTY_STRING:
            default:
                break;
        }
        soundManager.playSfx(SoundDB::getSound(message));
    }

    void notify(const unsigned int message, const int num)
    {
        if (message >= TYPE_END || !localChatTab)
            return;
        const NotificationInfo &info = notifications[message];
        if (info.flags == INT)
        {
            localChatTab->chatLog(strprintf(gettext(info.text),
                num), BY_SERVER);
        }
        soundManager.playSfx(SoundDB::getSound(message));
    }

    void notify(const unsigned int message, const std::string &str)
    {
        if (message >= TYPE_END || !localChatTab)
            return;
        const NotificationInfo &info = notifications[message];
        switch (info.flags)
        {
            case STRING:
            {
                localChatTab->chatLog(strprintf(gettext(info.text),
                    str.c_str()), BY_SERVER);
                break;
            }
            case GUILD_STRING:
            {
                ChatTab *const tab = getGuildTab();
                chatLog(tab, strprintf(gettext(info.text), str.c_str()));
                break;
            }
            case PARTY_STRING:
            {
                ChatTab *const tab = Net::getPartyHandler()->getTab();
                chatLog(tab, strprintf(gettext(info.text), str.c_str()));
                break;
            }
            case EMPTY:
            case INT:
            case GUILD:
            case PARTY:
            case SPEECH:
            default:
                break;
        }
        soundManager.playSfx(SoundDB::getSound(message));
    }

    int getIndexBySound(const std::string &sound)
    {
        for (int f = 0; f < TYPE_END; f ++)
        {
            if (notifications[f].sound == sound)
                return f;
        }
        return 0;
    }
}  // namespace NotifyManager
