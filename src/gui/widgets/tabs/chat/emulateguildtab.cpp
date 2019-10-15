/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifdef TMWA_SUPPORT

#include "gui/widgets/tabs/chat/emulateguildtab.h"

#include "configuration.h"
#include "soundmanager.h"

#include "const/sound.h"

#include "gui/windows/chatwindow.h"

#include "utils/gettext.h"

#include "net/tmwa/guildmanager.h"

#include "debug.h"

EmulateGuildTab::EmulateGuildTab(const Widget2 *const widget) :
    // TRANSLATORS: guild chat tab name
    ChatTab(widget, _("Guild"), "", "#Guild", ChatTabType::GUILD),
    ConfigListener()
{
    setTabColors(ThemeColorId::GUILD_CHAT_TAB);
    mShowOnline = config.getBoolValue("showGuildOnline");
    config.addListener("showGuildOnline", this);
}

EmulateGuildTab::~EmulateGuildTab()
{
    config.removeListeners(this);
    CHECKLISTENERS
}

bool EmulateGuildTab::handleCommand(const std::string &restrict type,
                                    const std::string &restrict args)
{
    if (type == "invite")
        GuildManager::invite(args);
    else if (type == "leave")
        GuildManager::leave();
    else if (type == "kick")
        GuildManager::kick(args);
    else if (type == "notice")
        GuildManager::notice(args);
    else
        return false;

    return true;
}

void EmulateGuildTab::handleInput(const std::string &msg)
{
    if (guildManager == nullptr)
        return;
    guildManager->chat(ChatWindow::doReplace(msg));
}

void EmulateGuildTab::getAutoCompleteList(StringVect &names) const
{
    if (guildManager == nullptr)
        return;

    GuildManager::getNames(names);
}

void EmulateGuildTab::getAutoCompleteCommands(StringVect &names) const
{
    names.push_back("/help");
    names.push_back("/invite ");
    names.push_back("/leave");
    names.push_back("/kick ");
    names.push_back("/notice ");
}

void EmulateGuildTab::playNewMessageSound() const
{
    soundManager.playGuiSound(SOUND_GUILD);
}

void EmulateGuildTab::optionChanged(const std::string &value)
{
    if (value == "showGuildOnline")
        mShowOnline = config.getBoolValue("showGuildOnline");
}

#endif  // TMWA_SUPPORT
