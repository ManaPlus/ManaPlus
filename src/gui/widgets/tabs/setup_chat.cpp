/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/tabs/setup_chat.h"

#include "gui/windows/chatwindow.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/gettext.h"

#include "debug.h"

extern int serverVersion;

Setup_Chat::Setup_Chat(const Widget2 *const widget) :
    SetupTabScroll(widget)
{
    // TRANSLATORS: settings tab name
    setName(_("Chat"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Window"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto hide chat window"), "",
        "autohideChat", this, "autohideChatEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Protect chat focus"), "",
        "protectChatFocus", this, "protectChatFocusEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Colors"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Remove colors from received chat messages"), "",
        "removeColors", this, "removeColorsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show chat colors list"), "",
        "showChatColorsList", this, "showChatColorsListEvent");


    // TRANSLATORS: settings option
    new SetupItemLabel(_("Commands"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Allow magic and GM commands in all chat tabs"),
        "", "allowCommandsInChatTabs", this, "allowCommandsInChatTabsEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Limits"), "", this);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Limit max chars in chat line"), "",
        "chatMaxCharLimit", this, "chatMaxCharLimitEvent", 0, 500);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Limit max lines in chat"), "",
        "chatMaxLinesLimit", this, "chatMaxLinesLimitEvent", 0, 500);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Logs"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable chat Log"), "",
        "enableChatLog", this, "enableChatLogEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable debug chat Log"), "",
        "enableDebugLog", this, "enableDebugLogEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show chat history"), "",
        "showChatHistory", this, "showChatHistoryEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show party online messages"), "",
        "showPartyOnline", this, "showPartyOnlineEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show guild online messages"), "",
        "showGuildOnline", this, "showGuildOnlineEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Messages"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Hide shop messages"), "",
        "hideShopMessages", this, "hideShopMessagesEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show MVP messages"), "",
        "showMVP", this, "showMVPEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Tabs"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Put all whispers in tabs"), "",
        "whispertab", this, "whispertabEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Log magic messages in debug tab"), "",
        "showMagicInDebug", this, "showMagicInDebugEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show server messages in debug tab"), "",
        "serverMsgInDebug", this, "serverMsgInDebugEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable trade tab"), "",
        "enableTradeTab", this, "enableTradeTabEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gm tab"), "",
        "enableGmTab", this, "enableGmTabEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable language tab"), "",
        "enableLangTab", this, "enableLangTabEvent", false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show all languages messages"), "",
        "showAllLang", this, "showAllLangEvent", false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable battle tab"), "",
        "enableBattleTab", this, "enableBattleTabEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show battle events"), "",
        "showBattleEvents", this, "showBattleEventsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Resize chat tabs if need"), "",
        "hideChatInput", this, "hideChatInputEvent");


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Time"), "", this);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use local time"), "",
        "useLocalTime", this, "useLocalTimeEvent");

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this);

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Highlight words (separated by comma)"), "",
        "highlightWords", this, "highlightWordsEvent");

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Globals ignore names (separated by comma)"), "",
        "globalsFilter", this, "globalsFilterEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show emotes button in chat"), "",
        "showEmotesButton", this, "showEmotesButtonEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show motd server message on start"), "",
        "showmotd", this, "showmotdEvent");

    setDimension(Rect(0, 0, 550, 350));
}

void Setup_Chat::apply()
{
    SetupTabScroll::apply();

    if (chatWindow)
    {
        chatWindow->adjustTabSize();
        chatWindow->parseHighlights();
    }
}

void Setup_Chat::externalUpdated()
{
    reread("enableLangTab");
    reread("showAllLang");
}
