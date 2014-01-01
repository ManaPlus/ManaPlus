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

#include "gui/widgets/tabs/gmtab.h"

#include "chatlogger.h"

#include "utils/gettext.h"

#include "debug.h"

GmTab::GmTab(const Widget2 *const widget) :
    // TRANSLATORS: gb tab name
    ChatTab(widget, _("GM"), "")
{
    setTabColor(&getThemeColor(Theme::GM_CHAT_TAB),
        &getThemeColor(Theme::GM_CHAT_TAB_OUTLINE));
    setHighlightedTabColor(&getThemeColor(Theme::GM_CHAT_TAB_HIGHLIGHTED),
        &getThemeColor(Theme::GM_CHAT_TAB_HIGHLIGHTED_OUTLINE));
    setSelectedTabColor(&getThemeColor(Theme::GM_CHAT_TAB_SELECTED),
        &getThemeColor(Theme::GM_CHAT_TAB_SELECTED_OUTLINE));
}

GmTab::~GmTab()
{
}

void GmTab::handleInput(const std::string &msg)
{
    ChatTab::handleInput("@wgm " + msg);
}

void GmTab::handleCommand(const std::string &msg A_UNUSED)
{
}

void GmTab::saveToLogFile(const std::string &msg) const
{
    if (chatLogger)
        chatLogger->log("#GM", msg);
}

void GmTab::getAutoCompleteList(StringVect &names A_UNUSED) const
{
}
