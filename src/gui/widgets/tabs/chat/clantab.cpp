/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/tabs/chat/clantab.h"

#include "soundmanager.h"

#include "const/sound.h"

#include "gui/windows/chatwindow.h"

#include "net/clanhandler.h"

#include "utils/gettext.h"

#include "debug.h"

ClanTab *clanTab = nullptr;

ClanTab::ClanTab(const Widget2 *const widget) :
    // TRANSLATORS: clan chat tab name
    ChatTab(widget, _("Clan"), "", "#Clan", ChatTabType::CLAN)
{
    setTabColors(ThemeColorId::CLAN_CHAT_TAB);
}

ClanTab::~ClanTab()
{
}

void ClanTab::handleInput(const std::string &msg)
{
    clanHandler->chat(ChatWindow::doReplace(msg));
}

void ClanTab::playNewMessageSound() const
{
    soundManager.playGuiSound(SOUND_GUILD);
}
