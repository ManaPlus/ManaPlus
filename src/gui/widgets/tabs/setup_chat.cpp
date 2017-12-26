/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/gettext.h"

#include "debug.h"

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
    new SetupItemLabel(_("Window"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto hide chat window"),
        // TRANSLATORS: settings description
        _("Chat window will be automatically hidden when not in use.\n\n"
        "Hit Enter or hover mouse to show chat again."),
        "autohideChat", this, "autohideChatEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Protect chat focus"),
        // TRANSLATORS: settings description
        _("Enables aggressive protection of input focus in chat window.\n\n"
        "Note: no other text inputs will be allowed to receive text input "
        "when you typing in chat window."),
        "protectChatFocus", this, "protectChatFocusEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Colors"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Remove colors from received chat messages"),
        // TRANSLATORS: settings description
        _("Enable this setting to strip colors from incoming chat messages. "
        "All messages will use default chat text color if this enabled."),
        "removeColors", this, "removeColorsEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show chat colors list"),
        // TRANSLATORS: settings description
        _("Enable this setting to show color selection drop-down in chat "
        "window. Chat window will display color selection drop-down.\n\n"
        "It allows one to select default color of outgoing chat messages "
        "easily, but also occupies some space in chat window."),
        "showChatColorsList", this, "showChatColorsListEvent",
        MainConfig_true);


    // TRANSLATORS: settings option
    new SetupItemLabel(_("Commands"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Allow magic and GM commands in all chat tabs"),
        // TRANSLATORS: settings description
        _("Enable this setting to be able to type spells and GM commands in "
        "any tab."),
        "allowCommandsInChatTabs", this, "allowCommandsInChatTabsEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Limits"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Limit max chars in chat line"),
        // TRANSLATORS: settings description
        _("Limits how many characters will be shown in longest lines "
        "of text displayed in chat.\n\n"
        "Note: long lines can make client slower. Lines longer than this "
        "limit will be truncated."),
        "chatMaxCharLimit", this, "chatMaxCharLimitEvent", 0, 500,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemIntTextField(_("Limit max lines in chat"),
        // TRANSLATORS: settings description
        _("Limits how many lines chat will keep in scrollback buffer. Chat "
        "keeps specified number of last lines of text. Oldest lines exceeding "
        "this limit are discarded from scrollback buffer.\n\n"
        "Note: keeping too many lines in scroll buffer can slow client down."),
        "chatMaxLinesLimit", this, "chatMaxLinesLimitEvent", 0, 500,
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Logs"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable chat Log"),
        // TRANSLATORS: settings description
        _("If you enable this setting, chat logs will be written to disk.\n\n"
        "Note: chat logs can take noticeable amount of disk space over time."),
        "enableChatLog", this, "enableChatLogEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable debug chat Log"),
        // TRANSLATORS: settings description
        _("If you enable this, debug chat tab also will be logged to disk."),
        "enableDebugLog", this, "enableDebugLogEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show chat history"),
        // TRANSLATORS: settings description
        _("If this setting enabled, client will load old chat tabs content "
         "from logs on startup instead of starting with empty chat tabs."),
        "showChatHistory", this, "showChatHistoryEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show party online messages"),
        // TRANSLATORS: settings description
        _("If this setting is enabled, online status changes of party members"
        " will be shown in party tab of chat.\n\nThis adds some extra noise "
        "to chat, but allows one to see when your buddies are coming online."),
        "showPartyOnline", this, "showPartyOnlineEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show guild online messages"),
        // TRANSLATORS: settings description
        _("If this setting is enabled, online status changes of guild members"
        " will be shown in guild tab of chat.\n\nThis adds some extra noise "
        "to chat, but allows one to see when your buddies are coming online."),
        "showGuildOnline", this, "showGuildOnlineEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Messages"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Hide shop messages"),
        // TRANSLATORS: settings description
        _("If this setting enabled, no messages related to built-in ManaPlus "
        "shop will be displayed in chat. Disable this setting if you want "
        "to see shop-related messages.\n\nNote: technically, ManaPlus shop "
        "implemented as usual private messages with special content. If you "
        "disable this setting, you will be able to see these messages and get "
        "idea when other players are looking at your shop."),
        "hideShopMessages", this, "hideShopMessagesEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show MVP messages"),
        // TRANSLATORS: settings description
        _("Enable this setting to see MVP messages from server.\n\n"
        "Note: MVP messages are not used on TMW/Evol/etc servers, so this "
        "feature usually makes little difference."),
        "showMVP", this, "showMVPEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Tabs"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Put all whispers in tabs"),
        // TRANSLATORS: settings description
        _("If this setting enabled, all whispers (private messages) will "
        "be placed in separate tabs, separate tab for each player. If this "
        "setting disabled, all whispers will appear in General tab.\n\n"
        "Note: putting all whispers to single General tab is known to be "
        "confusing. Think twice before disabling this feature."),
        "whispertab", this, "whispertabEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Log magic messages in debug tab"),
        // TRANSLATORS: settings description
        _("If this setting is enabled, spell invocation will be shown in "
        "Debug tab. If disabled, it will be shown in General tab instead.\n\n"
        "Note: it does not affects server replies related to spells."),
        "showMagicInDebug", this, "showMagicInDebugEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show server messages in debug tab"),
        // TRANSLATORS: settings description
        _("If this setting is enabled, server messages will be shown in "
        "Debug tab of chat. If disabled, server messages will appear in "
        "General chat instead.\n\nNote: according to 4144, disabling this "
        "could also make you to lose some debug messages from client in "
        "Debug tab since these are fake server messages."),
        "serverMsgInDebug", this, "serverMsgInDebugEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable trade tab"),
        // TRANSLATORS: settings description
        _("Enables trade tab. Trade tab is basically some filter. Messages "
        "containing words typical for trades will go to Trade tab. This "
        "will make General tab less noisy. If this setting is disabled, all "
        "trade related players messages will stay in General tab."),
        "enableTradeTab", this, "enableTradeTabEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable gm tab"),
        // TRANSLATORS: settings description
        _("If enabled, GM tab will appear in chat. It displays text related "
        "GM activity.\n\nNote: this setting only makes difference for "
        "GMs (Game Masters) since this tab only appears for GMs."),
        "enableGmTab", this, "enableGmTabEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable language tab"),
        // TRANSLATORS: settings description
        _("If this feature enabled, language tab will appear if server "
        "supports this feature.\n\nNote: only supported by Evol server yet."),
        "enableLangTab", this, "enableLangTabEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show all languages messages"),
        // TRANSLATORS: settings description
        _("If this setting enabled and server supports different chats "
        "for different languages, you will see messages for all languages, "
        "regardless of your language preferences.\n\nNote: it only works "
        "on servers supporting language tabs feature, like Evol."),
        "showAllLang", this, "showAllLangEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable battle tab"),
        // TRANSLATORS: settings description
        _("If this setting enabled, Battle tab will appear in chat. This "
        "tab will contain messages related to battles, like damage and "
        "experience gain, if battle messages are enabled.\n\n"
        "Note: client restart required to take effect."),
        "enableBattleTab", this, "enableBattleTabEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show battle events"),
        // TRANSLATORS: settings description
        _("If this setting enabled, messages related to battle like damage "
        "or experience gain will be displayed in Debug or Battle tab. If "
        "disabled, no battle messages will be displayed."),
        "showBattleEvents", this, "showBattleEventsEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Resize chat tabs if need"),
        // TRANSLATORS: settings description
        _("If this feature enabled, text in chat will be automatically "
        "adjusted to adapt to appearance of chat input field when you "
        "typing message and when input field of chat disappears. If disabled, "
        "chat input area will always occupy its place, which could be "
        "otherwise usable for text.\n\n"
        "Note: its mostly about jumpy attitude vs "
        "less usable space for text."),
        "hideChatInput", this, "hideChatInputEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable trade spam filter"),
        "", "enableTradeFilter", this, "enableTradeFilterEvent",
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Time"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use local time"),
        // TRANSLATORS: settings description
        _("If this feature enabled, timestamps in chat will use local times. "
        "If disabled, server time will be used (often it is GMT+0)."),
        "useLocalTime", this, "useLocalTimeEvent",
        MainConfig_true);

    // TRANSLATORS: settings group
    new SetupItemLabel(_("Other"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Highlight words (separated by comma)"),
        // TRANSLATORS: settings description
        _("Here you can specify some extra words which will also cause "
        "highlighting. Use comma to separate words.\n\nNote: frequent "
        "highlights are annoying - use it with caution."),
        "highlightWords", this, "highlightWordsEvent",
        MainConfig_true, UseBase64_false);

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Globals ignore names (separated by comma)"),
        // TRANSLATORS: settings description
        _("This setting allows you to ignore some global messages if "
        "particular sender (NPC, GM) annoys you too much. Global will be "
        "moved to Debug instead.\n\nNote: careless use of this feature can "
        "make you to miss important announces!"),
        "globalsFilter", this, "globalsFilterEvent",
        MainConfig_true, UseBase64_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show emotes button in chat"),
        // TRANSLATORS: settings description
        _("If this setting enabled, button will appear near text input "
        "field. This button allows one to invoke composing window, which "
        "allows one to insert smiles and text formatting easily.\n\n"
        "Note: same window can also be invoked by hotkey when typing, usually "
        "F1 by default."),
        "showEmotesButton", this, "showEmotesButtonEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show motd server message on start"),
        // TRANSLATORS: settings description
        _("If this setting enabled, client will display server MOTD (message"
        " of the day) once you connect to server. Disable it to hide MOTD."),
        "showmotd", this, "showmotdEvent",
        MainConfig_true);

    setDimension(Rect(0, 0, 550, 350));
}

void Setup_Chat::apply()
{
    SetupTabScroll::apply();

    if (chatWindow != nullptr)
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
