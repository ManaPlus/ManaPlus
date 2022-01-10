/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/tabs/chat/gmtab.h"

#include "const/gui/chat.h"

#include "gui/windows/chatwindow.h"

#include "utils/gettext.h"

#include "net/chathandler.h"

#include "debug.h"

GmTab *gmChatTab = nullptr;

GmTab::GmTab(const Widget2 *const widget) :
    // TRANSLATORS: gb tab name
    ChatTab(widget, _("GM"), GM_CHANNEL, "#GM", ChatTabType::GM)
{
    setTabColors(ThemeColorId::GM_CHAT_TAB);
}

GmTab::~GmTab()
{
}

void GmTab::handleInput(const std::string &msg)
{
    chatHandler->channelMessage(mChannelName, ChatWindow::doReplace(msg));
}

void GmTab::handleCommandStr(const std::string &msg A_UNUSED)
{
}

void GmTab::getAutoCompleteList(StringVect &names A_UNUSED) const
{
}
