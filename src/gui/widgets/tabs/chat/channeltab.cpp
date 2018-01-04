/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/widgets/tabs/chat/channeltab.h"

#include "gui/widgets/windowcontainer.h"

#include "gui/windows/chatwindow.h"

#include "net/chathandler.h"

#include "debug.h"

ChannelTab::ChannelTab(const Widget2 *const widget,
                       const std::string &channel) :
    ChatTab(widget, channel, channel, channel, ChatTabType::CHANNEL)
{
    setTabColors(ThemeColorId::CHANNEL_CHAT_TAB);
}

ChannelTab::~ChannelTab()
{
    if (chatWindow != nullptr)
        chatWindow->removeChannel(mChannelName);
}

void ChannelTab::handleInput(const std::string &msg)
{
    std::string newMsg;
    newMsg = ChatWindow::doReplace(msg);
    chatHandler->channelMessage(mChannelName, newMsg);
}

bool ChannelTab::handleCommand(const std::string &restrict type,
                               const std::string &restrict args A_UNUSED)
{
    if (type == "close")
    {
        if (windowContainer != nullptr)
            windowContainer->scheduleDelete(this);
        else
            delete this;
        if (chatWindow != nullptr)
            chatWindow->defaultTab();
    }
    else
    {
        return false;
    }

    return true;
}

void ChannelTab::getAutoCompleteCommands(StringVect& commands) const
{
    commands.push_back("/close");
    commands.push_back(mChannelName);
}
