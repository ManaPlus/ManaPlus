/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "gui/windows/chatwindow.h"

#include "input/inputmanager.h"

#include "net/guildhandler.h"

#include "utils/gettext.h"

#include "debug.h"

namespace EAthena
{
    extern Guild *taGuild;
}  // namespace EAthena

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
    if (type == "invite" && (EAthena::taGuild != nullptr))
    {
        guildHandler->invite(args);
    }
    else if (type == "leave" && (EAthena::taGuild != nullptr))
    {
        inputManager.executeChatCommand(InputAction::LEAVE_GUILD,
            std::string(),
            this);
    }
    else if (type == "kick" && (EAthena::taGuild != nullptr))
    {
        inputManager.executeChatCommand(InputAction::KICK_GUILD,
            args,
            this);
    }
    else if (type == "notice" && (EAthena::taGuild != nullptr))
    {
        inputManager.executeChatCommand(InputAction::GUILD_NOTICE,
            args,
            this);
    }
    else
    {
        return false;
    }

    return true;
}

void GuildTab::handleInput(const std::string &msg)
{
    if (EAthena::taGuild == nullptr)
        return;

    guildHandler->chat(ChatWindow::doReplace(msg));
}

void GuildTab::getAutoCompleteList(StringVect &names) const
{
    if (EAthena::taGuild != nullptr)
        EAthena::taGuild->getNames(names);
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
