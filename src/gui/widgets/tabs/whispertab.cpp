/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/tabs/whispertab.h"

#include "chatlogger.h"
#include "commands.h"

#include "being/localplayer.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "gui/widgets/windowcontainer.h"

#include "utils/gettext.h"

#include "debug.h"

WhisperTab::WhisperTab(const Widget2 *const widget, const std::string &nick) :
    ChatTab(widget, nick, ""),
    mNick(nick)
{
    setWhisperTabColors();
}

WhisperTab::~WhisperTab()
{
    if (chatWindow)
        chatWindow->removeWhisper(mNick);
}

void WhisperTab::handleInput(const std::string &msg)
{
    std::string newMsg;
    if (chatWindow)
        newMsg = chatWindow->doReplace(msg);
    else
        newMsg = msg;
    Net::getChatHandler()->privateMessage(mNick, newMsg);

    if (player_node)
        chatLog(player_node->getName(), newMsg);
    else
        chatLog("?", newMsg);
}

void WhisperTab::handleCommand(const std::string &msg)
{
    if (msg == "close")
    {
        delete this;
        return;
    }

    const size_t pos = msg.find(' ');
    const std::string type(msg, 0, pos);
    const std::string args(msg, pos == std::string::npos
        ? msg.size() : pos + 1);

    if (type == "me")
    {
        std::string str = strprintf("*%s*", args.c_str());
        Net::getChatHandler()->privateMessage(mNick, str);
        if (player_node)
            chatLog(player_node->getName(), str);
        else
            chatLog("?", str);
    }
    else
    {
        ChatTab::handleCommand(msg);
    }
}

void WhisperTab::showHelp()
{
    // TRANSLATORS: whisper tab help
    chatLog(_("/ignore > Ignore the other player"));
    // TRANSLATORS: whisper tab help
    chatLog(_("/unignore > Stop ignoring the other player"));
    // TRANSLATORS: whisper tab help
    chatLog(_("/close > Close the whisper tab"));
}

bool WhisperTab::handleCommand(const std::string &restrict type,
                               const std::string &restrict args)
{
    if (type == "help")
    {
        if (args == "close")
        {
            // TRANSLATORS: whisper tab help
            chatLog(_("Command: /close"));
            // TRANSLATORS: whisper tab help
            chatLog(_("This command closes the current whisper tab."));
        }
        else if (args == "ignore")
        {
            // TRANSLATORS: whisper tab help
            chatLog(_("Command: /ignore"));
            // TRANSLATORS: whisper tab help
            chatLog(_("This command ignores the other player regardless of "
                      "current relations."));
        }
        else if (args == "unignore")
        {
            // TRANSLATORS: whisper tab help
            chatLog(_("Command: /unignore <player>"));
            // TRANSLATORS: whisper tab help
            chatLog(_("This command stops ignoring the other player if they "
                      "are being ignored."));
        }
        else
        {
            return false;
        }
    }
    else if (type == "close")
    {
        if (windowContainer)
            windowContainer->scheduleDelete(this);
        else
            delete this;
        if (chatWindow)
            chatWindow->defaultTab();
    }
    else if (type == "ignore")
    {
        Commands::ignore(mNick, this);
    }
    else if (type == "unignore")
    {
        Commands::unignore(mNick, this);
    }
    else
    {
        return false;
    }

    return true;
}

void WhisperTab::saveToLogFile(const std::string &msg) const
{
    if (chatLogger)
        chatLogger->log(getNick(), msg);
}

void WhisperTab::getAutoCompleteList(StringVect &names) const
{
    names.push_back(mNick);
}

void WhisperTab::setWhisperTabColors()
{
    setTabColor(&getThemeColor(Theme::WHISPER_TAB),
        &getThemeColor(Theme::WHISPER_TAB_OUTLINE));
    setHighlightedTabColor(&getThemeColor(Theme::WHISPER_TAB_HIGHLIGHTED),
        &getThemeColor(Theme::WHISPER_TAB_HIGHLIGHTED_OUTLINE));
    setSelectedTabColor(&getThemeColor(Theme::WHISPER_TAB_SELECTED),
        &getThemeColor(Theme::WHISPER_TAB_SELECTED_OUTLINE));
}

void WhisperTab::setWhisperTabOfflineColors()
{
    setTabColor(&getThemeColor(Theme::WHISPER_TAB_OFFLINE),
        &getThemeColor(Theme::WHISPER_TAB_OFFLINE_OUTLINE));
    setHighlightedTabColor(&getThemeColor(
        Theme::WHISPER_TAB_OFFLINE_HIGHLIGHTED), &getThemeColor(
        Theme::WHISPER_TAB_OFFLINE_HIGHLIGHTED_OUTLINE));
    setSelectedTabColor(&getThemeColor(Theme::WHISPER_TAB_OFFLINE_SELECTED),
        &getThemeColor(Theme::WHISPER_TAB_OFFLINE_SELECTED_OUTLINE));
}
