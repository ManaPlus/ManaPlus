/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "net/ea/gui/guildtab.h"

#include "chatlogger.h"
#include "configuration.h"
#include "guild.h"
#include "soundconsts.h"
#include "soundmanager.h"

#include "net/net.h"

#include "net/ea/guildhandler.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{
extern Guild *taGuild;

GuildTab::GuildTab(const Widget2 *const widget) :
    // TRANSLATORS: guild chat tab name
    ChatTab(widget, _("Guild"), "")
{
    setTabColor(&getThemeColor(Theme::GUILD_CHAT_TAB),
        &getThemeColor(Theme::GUILD_CHAT_TAB_OUTLINE));
    setHighlightedTabColor(&getThemeColor(Theme::GUILD_CHAT_TAB_HIGHLIGHTED),
        &getThemeColor(Theme::GUILD_CHAT_TAB_HIGHLIGHTED_OUTLINE));
    setSelectedTabColor(&getThemeColor(Theme::GUILD_CHAT_TAB_SELECTED),
        &getThemeColor(Theme::GUILD_CHAT_TAB_SELECTED_OUTLINE));
    mShowOnline = config.getBoolValue("showGuildOnline");
    config.addListener("showGuildOnline", this);
}

GuildTab::~GuildTab()
{
    config.removeListeners(this);
    CHECKLISTENERS
}

bool GuildTab::handleCommand(const std::string &restrict type,
                             const std::string &restrict args)
{
/*
    else if (type == "create" || type == "new")
    {
        if (args.empty())
            chatLog(_("Guild name is missing."), BY_SERVER);
        else
            Net::getGuildHandler()->create(args);
    }
*/
    if (type == "invite" && taGuild)
    {
        Net::getGuildHandler()->invite(taGuild->getId(), args);
    }
    else if (type == "leave" && taGuild)
    {
        Net::getGuildHandler()->leave(taGuild->getId());
    }
    else if (type == "kick" && taGuild)
    {
        Net::getGuildHandler()->kick(taGuild->getMember(args), "");
    }
    else if (type == "notice" && taGuild)
    {
        std::string str1 = args.substr(0, 60);
        std::string str2("");
        if (args.size() > 60)
            str2 = args.substr(60);
        Net::getGuildHandler()->changeNotice(taGuild->getId(), str1, str2);
    }
    else
    {
        return false;
    }

    return true;
}

void GuildTab::handleInput(const std::string &msg)
{
    if (!taGuild)
        return;

    Net::getGuildHandler()->chat(taGuild->getId(),
        ChatWindow::doReplace(msg));
}

void GuildTab::getAutoCompleteList(StringVect &names) const
{
    if (taGuild)
        taGuild->getNames(names);
}

void GuildTab::getAutoCompleteCommands(StringVect &names) const
{
    names.push_back("/help");
    names.push_back("/invite ");
    names.push_back("/kick ");
    names.push_back("/notice ");
    names.push_back("/leave");
}

void GuildTab::saveToLogFile(const std::string &msg) const
{
    if (chatLogger)
        chatLogger->log("#Guild", msg);
}

void GuildTab::playNewMessageSound() const
{
    soundManager.playGuiSound(SOUND_GUILD);
}

void GuildTab::optionChanged(const std::string &value)
{
    if (value == "showGuildOnline")
        mShowOnline = config.getBoolValue("showGuildOnline");
}

}  // namespace Ea
