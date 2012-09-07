/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/setup_chat.h"

#include "gui/chatwindow.h"
#include "gui/editdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "configuration.h"
#include "localplayer.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Chat::Setup_Chat() :
    SetupTabScroll()
{
    setName(_("Chat"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemLabel(_("Window"), "", this);

    new SetupItemCheckBox(_("Auto hide chat window."), "",
        "autohideChat", this, "autohideChatEvent");


    new SetupItemLabel(_("Colors"), "", this);

    new SetupItemCheckBox(_("Remove colors from received chat messages"), "",
        "removeColors", this, "removeColorsEvent");

    new SetupItemCheckBox(_("Show chat colors list"), "",
        "showChatColorsList", this, "showChatColorsListEvent");


    new SetupItemLabel(_("Commands"), "", this);

    new SetupItemCheckBox(_("Allow magic and GM commands in all chat tabs"),
        "", "allowCommandsInChatTabs", this, "allowCommandsInChatTabsEvent");


    new SetupItemLabel(_("Limits"), "", this);

    new SetupItemIntTextField(_("Limit max chars in chat line"), "",
        "chatMaxCharLimit", this, "chatMaxCharLimitEvent", 0, 500);

    new SetupItemIntTextField(_("Limit max lines in chat"), "",
        "chatMaxLinesLimit", this, "chatMaxLinesLimitEvent", 0, 500);


    new SetupItemLabel(_("Logs"), "", this);

    new SetupItemCheckBox(_("Enable chat Log"), "",
        "enableChatLog", this, "enableChatLogEvent");

    new SetupItemCheckBox(_("Show chat history"), "",
        "showChatHistory", this, "showChatHistoryEvent");


    new SetupItemLabel(_("Messages"), "", this);

    new SetupItemCheckBox(_("Hide shop messages"), "",
        "hideShopMessages", this, "hideShopMessagesEvent");


    new SetupItemLabel(_("Tabs"), "", this);

    new SetupItemCheckBox(_("Put all whispers in tabs"), "",
        "whispertab", this, "whispertabEvent");

    new SetupItemCheckBox(_("Log magic messages in debug tab"), "",
        "showMagicInDebug", this, "showMagicInDebugEvent");

    new SetupItemCheckBox(_("Show server messages in debug tab"), "",
        "serverMsgInDebug", this, "serverMsgInDebugEvent");

    new SetupItemCheckBox(_("Enable trade tab"), "",
        "enableTradeTab", this, "enableTradeTabEvent");

    new SetupItemCheckBox(_("Enable battle tab"), "",
        "enableBattleTab", this, "enableBattleTabEvent");

    new SetupItemCheckBox(_("Show battle events"), "",
        "showBattleEvents", this, "showBattleEventsEvent");

    new SetupItemCheckBox(_("Resize chat tabs if need"), "",
        "hideChatInput", this, "hideChatInputEvent");


    new SetupItemLabel(_("Time"), "", this);

    new SetupItemCheckBox(_("Use local time"), "",
        "useLocalTime", this, "useLocalTimeEvent");

    new SetupItemLabel(_("Other"), "", this);

    new SetupItemTextField(_("Highlight words (separated by comma)"), "",
        "highlightWords", this, "highlightWordsEvent");

    new SetupItemCheckBox(_("Show MVP messages"), "",
        "showMVP", this, "showMVPEvent");

    setDimension(gcn::Rectangle(0, 0, 550, 350));
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
