/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/tabs/chat/whispertab.h"

#include "being/localplayer.h"

#include "net/chathandler.h"

#include "gui/windows/chatwindow.h"

#include "gui/widgets/windowcontainer.h"

#include "utils/chatutils.h"

#include "debug.h"

WhisperTab::WhisperTab(const Widget2 *const widget,
                       const std::string &caption,
                       const std::string &nick) :
    ChatTab(widget, caption, nick, nick, ChatTabType::WHISPER),
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
    newMsg = ChatWindow::doReplace(msg);
    chatHandler->privateMessage(mNick, newMsg);

    if (localPlayer)
        chatLog(localPlayer->getName(), newMsg);
    else
        chatLog("?", newMsg);
}

void WhisperTab::handleCommandStr(const std::string &msg)
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
        std::string str = textToMe(args);
        chatHandler->privateMessage(mNick, str);
        if (localPlayer)
            chatLog(localPlayer->getName(), str);
        else
            chatLog("?", str);
    }
    else
    {
        ChatTab::handleCommandStr(msg);
    }
}

bool WhisperTab::handleCommand(const std::string &restrict type,
                               const std::string &restrict args A_UNUSED)
{
    if (type == "close")
    {
        if (windowContainer)
            windowContainer->scheduleDelete(this);
        else
            delete this;
        if (chatWindow)
            chatWindow->defaultTab();
    }
    else
    {
        return false;
    }

    return true;
}

void WhisperTab::getAutoCompleteList(StringVect &names) const
{
    names.push_back(mNick);
}

void WhisperTab::getAutoCompleteCommands(StringVect& commands) const
{
    commands.push_back("/close");
}

void WhisperTab::setWhisperTabColors()
{
    setTabColors(ThemeColorId::WHISPER_TAB);
}

void WhisperTab::setWhisperTabOfflineColors()
{
    setTabColors(ThemeColorId::WHISPER_TAB_OFFLINE);
}
