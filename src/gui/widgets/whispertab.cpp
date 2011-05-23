/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/widgets/whispertab.h"

#include "chatlog.h"
#include "commandhandler.h"
#include "localplayer.h"
#include "log.h"

#include "gui/theme.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

WhisperTab::WhisperTab(const std::string &nick) :
    ChatTab(nick),
    mNick(nick)
{
    setTabColor(&Theme::getThemeColor(Theme::WHISPER));
}

WhisperTab::~WhisperTab()
{
    if (chatWindow)
        chatWindow->removeWhisper(mNick);
}

void WhisperTab::handleInput(const std::string &msg)
{
//    if (msg.empty())
//    {
//        chatLog(_("Cannot send empty chat!"), BY_SERVER, false);
//        return;
//    }

    if (chatWindow)
    {
        Net::getChatHandler()->privateMessage(mNick,
            chatWindow->doReplace(msg));
    }
    else
    {
        Net::getChatHandler()->privateMessage(mNick, msg);
    }

    if (player_node)
        chatLog(player_node->getName(), msg);
    else
        chatLog("?", msg);
}

void WhisperTab::handleCommand(const std::string &msg)
{
    if (msg == "close")
    {
        delete this;
        return;
    }

    std::string::size_type pos = msg.find(' ');
    std::string type(msg, 0, pos);
    std::string args(msg, pos == std::string::npos
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
    chatLog(_("/ignore > Ignore the other player"));
    chatLog(_("/unignore > Stop ignoring the other player"));
    chatLog(_("/close > Close the whisper tab"));
}

bool WhisperTab::handleCommand(const std::string &type,
                               const std::string &args)
{
    if (type == "help")
    {
        if (args == "close")
        {
            chatLog(_("Command: /close"));
            chatLog(_("This command closes the current whisper tab."));
        }
        else if (args == "ignore")
        {
            chatLog(_("Command: /ignore"));
            chatLog(_("This command ignores the other player regardless of "
                      "current relations."));
        }
        else if (args == "unignore")
        {
            chatLog(_("Command: /unignore <player>"));
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
        delete this;
        if (chatWindow)
            chatWindow->defaultTab();
    }
    else if (type == "ignore")
    {
        if (commandHandler)
            commandHandler->handleIgnore(mNick, this);
    }
    else if (type == "unignore")
    {
        if (commandHandler)
            commandHandler->handleUnignore(mNick, this);
    }
    else
    {
        return false;
    }

    return true;
}

void WhisperTab::saveToLogFile(std::string &msg)
{
    if (chatLogger)
        chatLogger->log(getNick(), msg);
}
