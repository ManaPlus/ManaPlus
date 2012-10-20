/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/widgets/guildchattab.h"

#include "chatlogger.h"
#include "commandhandler.h"
#include "guild.h"
#include "guildmanager.h"
#include "localplayer.h"
#include "sound.h"

#include "resources/iteminfo.h"
#include "resources/itemdb.h"

#include "utils/gettext.h"

#include "debug.h"

GuildChatTab::GuildChatTab(const Widget2 *const widget) :
    ChatTab(widget, _("Guild"))
{
    setTabColor(&getThemeColor(Theme::GUILD_CHAT_TAB));
    setHighlightedTabColor(&getThemeColor(
        Theme::GUILD_CHAT_TAB_HIGHLIGHTED));
    setSelectedTabColor(&getThemeColor(
        Theme::GUILD_CHAT_TAB_SELECTED));
}

GuildChatTab::~GuildChatTab()
{
}

bool GuildChatTab::handleCommand(const std::string &type,
                                 const std::string &args)
{
    if (type == "help")
    {
        if (args == "invite")
        {
            chatLog(_("Command: /invite <nick>"));
            chatLog(_("This command invites <nick> to the guild you're in."));
            chatLog(_("If the <nick> has spaces in it, enclose it in "
                            "double quotes (\")."));
        }
        else if (args == "leave")
        {
            chatLog(_("Command: /leave"));
            chatLog(_("This command causes the player to leave the guild."));
        }
        else
            return false;
    }
    else if (type == "invite" && guildManager)
    {
        guildManager->invite(args);
    }
    else if (type == "leave" && guildManager)
    {
        guildManager->leave();
    }
    else if (type == "kick" && guildManager)
    {
        guildManager->kick(args);
    }
    else if (type == "notice" && guildManager)
    {
        guildManager->notice(args);
    }
    else
    {
        return false;
    }

    return true;
}

void GuildChatTab::handleInput(const std::string &msg)
{
    if (!guildManager)
        return;

    if (chatWindow)
        guildManager->chat(chatWindow->doReplace(msg));
    else
        guildManager->chat(msg);
}

void GuildChatTab::showHelp()
{
    chatLog(_("/help > Display this help."));
    chatLog(_("/invite > Invite a player to your guild"));
    chatLog(_("/leave > Leave the guild you are in"));
    chatLog(_("/kick > Kick some one from the guild you are in"));
}

void GuildChatTab::getAutoCompleteList(StringVect &names) const
{
    if (!guildManager)
        return;

    guildManager->getNames(names);
    names.push_back("/notice ");
}

void GuildChatTab::saveToLogFile(std::string &msg)
{
    if (chatLogger)
        chatLogger->log("#Guild", msg);
}

void GuildChatTab::playNewMessageSound()
{
    sound.playGuiSound(SOUND_GUILD);
}
