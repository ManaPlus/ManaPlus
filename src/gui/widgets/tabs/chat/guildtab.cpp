/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "gui/widgets/tabs/chat/guildtab.h"

#include "configuration.h"
#include "soundmanager.h"

#include "const/sound.h"

#include "net/guildhandler.h"

#include "gui/windows/chatwindow.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{
    extern Guild *taGuild;
}  // namespace Ea

GuildTab::GuildTab(const Widget2 *const widget) :
    // TRANSLATORS: guild chat tab name
    ChatTab(widget, _("Guild"), "", "#Guild", ChatTabType::GUILD)
{
    setTabColors(ThemeColorId::GUILD_CHAT_TAB);
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
    if (type == "invite" && Ea::taGuild)
    {
        guildHandler->invite(args);
    }
    else if (type == "leave" && Ea::taGuild)
    {
        guildHandler->leave(Ea::taGuild->getId());
    }
    else if (type == "kick" && Ea::taGuild)
    {
        guildHandler->kick(Ea::taGuild->getMember(args), "");
    }
    else if (type == "notice" && Ea::taGuild)
    {
        std::string str1 = args.substr(0, 60);
        std::string str2("");
        if (args.size() > 60)
            str2 = args.substr(60);
        guildHandler->changeNotice(Ea::taGuild->getId(), str1, str2);
    }
    else
    {
        return false;
    }

    return true;
}

void GuildTab::handleInput(const std::string &msg)
{
    if (!Ea::taGuild)
        return;

    guildHandler->chat(ChatWindow::doReplace(msg));
}

void GuildTab::getAutoCompleteList(StringVect &names) const
{
    if (Ea::taGuild)
        Ea::taGuild->getNames(names);
}

void GuildTab::getAutoCompleteCommands(StringVect &names) const
{
    names.push_back("/help");
    names.push_back("/invite ");
    names.push_back("/kick ");
    names.push_back("/notice ");
    names.push_back("/leave");
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
