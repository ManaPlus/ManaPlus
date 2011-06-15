/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/setup_chat.h"
#include "gui/editdialog.h"
#include "gui/chatwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/chattab.h"

#include "configuration.h"
#include "localplayer.h"
#include "log.h"

#include "utils/gettext.h"

#include "debug.h"

#define ACTION_REMOVE_COLORS "remove colors"
#define ACTION_MAGIC_IN_DEBUG "magic in debug"
#define ACTION_ALLOW_COMMANDS_IN_CHATTABS "allow commands"
#define ACTION_SERVER_MSG_IN_DEBUG "server in debug"
#define ACTION_SHOW_CHAT_COLORS "show chat colors"
#define ACTION_MAX_CHAR_LIMIT "char limit"
#define ACTION_EDIT_CHAR_LIMIT "edit char limit"
#define ACTION_EDIT_CHAR_OK "edit char ok"
#define ACTION_MAX_LINES_LIMIT "lines limit"
#define ACTION_EDIT_LINES_LIMIT "edit lines limit"
#define ACTION_EDIT_LINES_OK "edit lines ok"
#define ACTION_CHAT_LOGGER "chat logger"
#define ACTION_TRADE_TAB "trade tab"
#define ACTION_HIDE_SHOP_MESSAGES "hide shop messages"
#define ACTION_SHOW_CHAT_HISTORY "show chat history"
#define ACTION_ENABLE_BATTLE_TAB "show battle tab"
#define ACTION_SHOW_BATTLE_EVENTS "show battle events"
#define ACTION_RESIZE_CHAT "resize chat"
#define ACTION_LOCAL_TIME "local time"

Setup_Chat::Setup_Chat() :
    mEditDialog(0)
{
    setName(_("Chat"));

    mRemoveColors = config.getBoolValue("removeColors");
    mRemoveColorsCheckBox = new CheckBox(
        _("Remove colors from received chat messages"),
        mRemoveColors, this, ACTION_REMOVE_COLORS);

    mMagicInDebug = config.getBoolValue("showMagicInDebug");
    mMagicInDebugCheckBox = new CheckBox(_("Log magic messages in debug tab"),
        mMagicInDebug, this, ACTION_MAGIC_IN_DEBUG);

    mAllowCommandsInChatTabs = config.getBoolValue(
        "allowCommandsInChatTabs");

    mAllowCommandsInChatTabsCheckBox = new CheckBox(
        _("Allow magic and GM commands in all chat tabs"),
        mAllowCommandsInChatTabs, this, ACTION_ALLOW_COMMANDS_IN_CHATTABS);

    mServerMsgInDebug = config.getBoolValue("serverMsgInDebug");
    mServerMsgInDebugCheckBox = new CheckBox(
        _("Show server messages in debug tab"),
        mServerMsgInDebug, this, ACTION_SERVER_MSG_IN_DEBUG);

    mEnableChatLogger = config.getBoolValue("enableChatLog");
    mEnableChatLoggerCheckBox = new CheckBox(_("Enable chat Log"),
        mEnableChatLogger, this, ACTION_CHAT_LOGGER);

    mEnableTradeTab = config.getBoolValue("enableTradeTab");
    mEnableTradeTabCheckBox = new CheckBox(_("Enable trade tab"),
        mEnableTradeTab, this, ACTION_TRADE_TAB);

    mHideShopMessages = config.getBoolValue("hideShopMessages");
    mHideShopMessagesCheckBox = new CheckBox(_("Hide shop messages"),
        mHideShopMessages, this, ACTION_HIDE_SHOP_MESSAGES);

    mShowChatHistory = config.getBoolValue("showChatHistory");
    mShowChatHistoryCheckBox = new CheckBox(_("Show chat history"),
        mShowChatHistory, this, ACTION_SHOW_CHAT_HISTORY);

    mEnableBattleTab = config.getBoolValue("enableBattleTab");
    mEnableBattleTabCheckBox = new CheckBox(_("Enable battle tab"),
        mEnableBattleTab, this, ACTION_ENABLE_BATTLE_TAB);

    mShowBattleEvents = config.getBoolValue("showBattleEvents");
    mShowBattleEventsCheckBox = new CheckBox(_("Show battle events"),
        mShowBattleEvents, this, ACTION_SHOW_BATTLE_EVENTS);

    mShowChatColors = config.getBoolValue("showChatColorsList");
    mShowChatColorsCheckBox = new CheckBox(_("Show chat colors list"),
        mShowChatColors, this, ACTION_SHOW_CHAT_COLORS);

    mMaxCharButton = new Button(_("Edit"), ACTION_EDIT_CHAR_LIMIT, this);
    int maxCharLimit = config.getIntValue("chatMaxCharLimit");
    mMaxChar = (maxCharLimit != 0);
    mMaxCharCheckBox = new CheckBox(_("Limit max chars in chat line"),
        mMaxChar, this, ACTION_MAX_CHAR_LIMIT);

    mMaxCharField = new IntTextField(maxCharLimit, 0, 500, mMaxChar, 20);

    mMaxLinesButton = new Button(_("Edit"), ACTION_EDIT_LINES_LIMIT, this);
    int maxLinesLimit = config.getIntValue("chatMaxLinesLimit");
    mMaxLines = (maxLinesLimit != 0);

    mMaxLinesCheckBox = new CheckBox(_("Limit max lines in chat"),
                                     mMaxLines,
                                     this, ACTION_MAX_LINES_LIMIT);

    mMaxLinesField = new IntTextField(maxLinesLimit, 0, 500, mMaxLines, 20);

    mHideChatInput = config.getBoolValue("hideChatInput");
    mHideChatInputCheckBox = new CheckBox(_("Resize chat tabs if need"),
        mHideChatInput, this, ACTION_RESIZE_CHAT);

    mLocalTime = config.getBoolValue("useLocalTime");
    mLocalTimeCheckBox = new CheckBox(_("Use local time"),
        mLocalTime, this, ACTION_LOCAL_TIME);

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mRemoveColorsCheckBox, 10);
    place(0, 1, mMagicInDebugCheckBox, 10);
    place(0, 2, mAllowCommandsInChatTabsCheckBox, 10);
    place(0, 3, mServerMsgInDebugCheckBox, 10);
    place(0, 4, mShowChatColorsCheckBox, 10);
    place(0, 5, mMaxCharCheckBox, 6);
    place(6, 5, mMaxCharField, 2);
    place(8, 5, mMaxCharButton, 2);
    place(0, 6, mEnableChatLoggerCheckBox, 10);
    place(0, 7, mMaxLinesCheckBox, 6);
    place(6, 7, mMaxLinesField, 2);
    place(8, 7, mMaxLinesButton, 2);
    place(0, 8, mEnableTradeTabCheckBox, 10);
    place(0, 9, mHideShopMessagesCheckBox, 10);
    place(0, 10, mShowChatHistoryCheckBox, 10);
    place(0, 11, mEnableBattleTabCheckBox, 10);
    place(0, 12, mShowBattleEventsCheckBox, 10);
    place(0, 13, mHideChatInputCheckBox, 10);
    place(0, 14, mLocalTimeCheckBox, 10);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);

    setDimension(gcn::Rectangle(0, 0, 500, 500));
}

void Setup_Chat::action(const gcn::ActionEvent &event)
{
    if (event.getId() == ACTION_REMOVE_COLORS)
    {
        mRemoveColors = mRemoveColorsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_MAGIC_IN_DEBUG)
    {
        mMagicInDebug = mMagicInDebugCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_ALLOW_COMMANDS_IN_CHATTABS)
    {
        mAllowCommandsInChatTabs
            = mAllowCommandsInChatTabsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SERVER_MSG_IN_DEBUG)
    {
        mServerMsgInDebug = mServerMsgInDebugCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_CHAT_COLORS)
    {
        mShowChatColors = mShowChatColorsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_MAX_CHAR_LIMIT)
    {
        mMaxChar = mMaxCharCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_EDIT_CHAR_LIMIT)
    {
        mEditDialog =  new EditDialog("Limit max chars in chat line",
                                      toString(mMaxCharField->getValue()),
                                      ACTION_EDIT_CHAR_OK);
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == ACTION_EDIT_CHAR_OK)
    {
        mMaxCharField->setValue(atoi(mEditDialog->getMsg().c_str()));
    }
    else if (event.getId() == ACTION_MAX_LINES_LIMIT)
    {
        mMaxLines = mMaxLinesCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_EDIT_LINES_LIMIT)
    {
        mEditDialog =  new EditDialog("Limit max lines in chat",
                                      toString(mMaxLinesField->getValue()),
                                      ACTION_EDIT_LINES_OK);
        mEditDialog->addActionListener(this);
    }
    else if (event.getId() == ACTION_EDIT_LINES_OK)
    {
        mMaxLinesField->setValue(atoi(mEditDialog->getMsg().c_str()));
    }
    else if (event.getId() == ACTION_CHAT_LOGGER)
    {
        mEnableChatLogger = mEnableChatLoggerCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_TRADE_TAB)
    {
        mEnableTradeTab = mEnableTradeTabCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_HIDE_SHOP_MESSAGES)
    {
        mHideShopMessages = mHideShopMessagesCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_CHAT_HISTORY)
    {
        mShowChatHistory = mShowChatHistoryCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_ENABLE_BATTLE_TAB)
    {
        mEnableBattleTab = mEnableBattleTabCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_SHOW_BATTLE_EVENTS)
    {
        mShowBattleEvents = mShowBattleEventsCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_RESIZE_CHAT)
    {
        mHideChatInput = mHideChatInputCheckBox->isSelected();
    }
    else if (event.getId() == ACTION_LOCAL_TIME)
    {
        mLocalTime = mLocalTimeCheckBox->isSelected();
    }
}

void Setup_Chat::cancel()
{
    mRemoveColors = config.getBoolValue("removeColors");
    mRemoveColorsCheckBox->setSelected(mRemoveColors);

    mMagicInDebug = config.getBoolValue("showMagicInDebug");
    mMagicInDebugCheckBox->setSelected(mMagicInDebug);

    mAllowCommandsInChatTabs
        = config.getBoolValue("allowCommandsInChatTabs");
    mAllowCommandsInChatTabsCheckBox->setSelected(mAllowCommandsInChatTabs);

    mServerMsgInDebug = config.getBoolValue("serverMsgInDebug");
    mServerMsgInDebugCheckBox->setSelected(mServerMsgInDebug);

    mShowChatColors = config.getBoolValue("showChatColorsList");
    mShowChatColorsCheckBox->setSelected(mShowChatColors);

    int maxCharLimit = config.getIntValue("chatMaxCharLimit");
    mMaxChar = (maxCharLimit != 0);
    mMaxCharCheckBox->setSelected(mMaxChar);
    mMaxCharField->setValue(maxCharLimit);
    mMaxCharField->setEnabled(mMaxChar);

    int maxLinesLimit = config.getIntValue("chatMaxLinesLimit");
    mMaxLines = (maxLinesLimit != 0);
    mMaxLinesCheckBox->setSelected(mMaxLines);
    mMaxLinesField->setValue(maxLinesLimit);
    mMaxLinesField->setEnabled(mMaxLines);

    mEnableChatLogger = config.getBoolValue("enableChatLog");
    mEnableChatLoggerCheckBox->setSelected(mEnableChatLogger);

    mEnableTradeTab = config.getBoolValue("enableTradeTab");
    mEnableTradeTabCheckBox->setSelected(mEnableTradeTab);

    mHideShopMessages = config.getBoolValue("hideShopMessages");
    mHideShopMessagesCheckBox->setSelected(mHideShopMessages);

    mShowChatHistory = config.getBoolValue("showChatHistory");
    mShowChatHistoryCheckBox->setSelected(mShowChatHistory);

    mEnableBattleTab = config.getBoolValue("enableBattleTab");
    mEnableBattleTabCheckBox->setSelected(mEnableBattleTab);

    mShowBattleEvents = config.getBoolValue("showBattleEvents");
    mShowBattleEventsCheckBox->setSelected(mShowBattleEvents);

    mHideChatInput = config.getBoolValue("hideChatInput");
    mHideChatInputCheckBox->setSelected(mHideChatInput);

    mLocalTime = config.getBoolValue("useLocalTime");
    mLocalTimeCheckBox->setSelected(mLocalTime);
}

void Setup_Chat::apply()
{
    config.setValue("removeColors", mRemoveColors);
    config.setValue("showMagicInDebug", mMagicInDebug);
    config.setValue("allowCommandsInChatTabs", mAllowCommandsInChatTabs);
    config.setValue("serverMsgInDebug", mServerMsgInDebug);
    config.setValue("showChatColorsList", mShowChatColors);
    if (mMaxChar)
        config.setValue("chatMaxCharLimit", mMaxCharField->getValue());
    else
        config.setValue("chatMaxCharLimit", 0);
    if (mMaxLines)
        config.setValue("chatMaxLinesLimit", mMaxLinesField->getValue());
    else
        config.setValue("chatMaxLinesLimit", 0);
    config.setValue("enableChatLog", mEnableChatLogger);
    config.setValue("enableTradeTab", mEnableTradeTab);
    config.setValue("hideShopMessages", mHideShopMessages);
    config.setValue("showChatHistory", mShowChatHistory);
    config.setValue("enableBattleTab", mEnableBattleTab);
    config.setValue("showBattleEvents", mShowBattleEvents);
    config.setValue("hideChatInput", mHideChatInput);
    config.setValue("useLocalTime", mLocalTime);

    if (chatWindow)
        chatWindow->adjustTabSize();
}
