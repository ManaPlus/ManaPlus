/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/chattab.h"

#include "actorspritemanager.h"
#include "chatlog.h"
#include "commandhandler.h"
#include "configuration.h"
#include "localplayer.h"
#include "log.h"
#include "sound.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/tradetab.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "resources/iteminfo.h"
#include "resources/itemdb.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/widgets/tabbedarea.hpp>

#include "debug.h"

#define MAX_WORD_SIZE 50

ChatTab::ChatTab(const std::string &name) :
    Tab(),
    mAllowHightlight(true),
    mRemoveNames(false),
    mNoAway(false)
{
    setCaption(name);

    mTextOutput = new BrowserBox(BrowserBox::AUTO_WRAP);
    mTextOutput->setOpaque(false);
    mTextOutput->setMaxRow(static_cast<int>(
        config.getIntValue("ChatLogLength")));
    if (chatWindow)
        mTextOutput->setLinkHandler(chatWindow->mItemLinkHandler);
    mTextOutput->setAlwaysUpdate(false);

    mScrollArea = new ScrollArea(mTextOutput);
    mScrollArea->setScrollPolicy(gcn::ScrollArea::SHOW_NEVER,
                                 gcn::ScrollArea::SHOW_ALWAYS);
    mScrollArea->setScrollAmount(0, 1);
    mScrollArea->setOpaque(false);

    if (chatWindow)
        chatWindow->addTab(this);
}

ChatTab::~ChatTab()
{
    if (chatWindow)
        chatWindow->removeTab(this);

    delete mTextOutput;
    mTextOutput = 0;
    delete mScrollArea;
    mScrollArea = 0;
}

void ChatTab::chatLog(std::string line, Own own,
                      bool ignoreRecord, bool tryRemoveColors)
{
    // Trim whitespace
    trim(line);

    if (line.empty())
        return;

    if (tryRemoveColors && own == BY_OTHER &&
        config.getBoolValue("removeColors"))
    {
        line = removeColors(line);
        if (line.empty())
            return;
    }

    unsigned lineLim = config.getIntValue("chatMaxCharLimit");
    if (lineLim > 0 && line.length() > lineLim)
        line = line.substr(0, lineLim);

    if (line.empty())
        return;

    CHATLOG tmp;
    tmp.own = own;
    tmp.nick = "";
    tmp.text = line;

    std::string::size_type pos = line.find(" : ");
    if (pos != std::string::npos)
    {
        if (line.length() <= pos + 3)
            return;

        tmp.nick = line.substr(0, pos);
        tmp.text = line.substr(pos + 3);
    }
    else
    {
        // Fix the owner of welcome message.
        if (line.length() > 7 && line.substr(0, 7) == "Welcome")
            own = BY_SERVER;
    }

    // *implements actions in a backwards compatible way*
    if ((own == BY_PLAYER || own == BY_OTHER) &&
        tmp.text.at(0) == '*' &&
        tmp.text.at(tmp.text.length()-1) == '*')
    {
        tmp.text[0] = ' ';
        tmp.text.erase(tmp.text.length() - 1);
        own = ACT_IS;
    }

    std::string lineColor = "##C";
    switch (own)
    {
        case BY_GM:
            if (tmp.nick.empty())
            {
                tmp.nick = std::string(_("Global announcement:"));
                tmp.nick += " ";
                lineColor = "##G";
            }
            else
            {
                tmp.nick = strprintf(_("Global announcement from %s:"),
                                     tmp.nick.c_str());
                tmp.nick += " ";
                lineColor = "##1"; // Equiv. to BrowserBox::RED
            }
            break;
        case BY_PLAYER:
            tmp.nick += ": ";
            lineColor = "##Y";
            break;
        case BY_OTHER:
        case BY_UNKNOWN:
            tmp.nick += ": ";
            lineColor = "##C";
            break;
        case BY_SERVER:
            tmp.nick = _("Server:");
            tmp.nick += " ";
            tmp.text = line;
            lineColor = "##S";
            break;
        case BY_CHANNEL:
            tmp.nick = "";
            // TODO: Use a predefined color
            lineColor = "##2"; // Equiv. to BrowserBox::GREEN
            break;
        case ACT_WHISPER:
            tmp.nick = strprintf(_("%s whispers: %s"), tmp.nick.c_str(), "");
            lineColor = "##W";
            break;
        case ACT_IS:
            lineColor = "##I";
            break;
        case BY_LOGGER:
            tmp.nick = "";
            tmp.text = line;
            lineColor = "##L";
            break;
        default:
            logger->log1("ChatTab::chatLog incorrect value in switch");
            break;
    }

    if (tmp.nick == ": ")
    {
        tmp.nick = "";
        lineColor = "##S";
    }

    // if configured, move magic messages log to debug chat tab
    if (localChatTab && this == localChatTab
        && ((config.getBoolValue("showMagicInDebug")
        && own == BY_PLAYER && tmp.text.length() > 1
        && tmp.text.length() > 1 && tmp.text.at(0) == '#'
        && tmp.text.at(1) != '#')
        || (config.getBoolValue("serverMsgInDebug") && (own == BY_SERVER
        || tmp.nick.empty()))))
    {
        if (debugChatTab)
            debugChatTab->chatLog(line, own, ignoreRecord, tryRemoveColors);
        return;
    }

    // Get the current system time
    time_t t;
    time(&t);

    if (config.getBoolValue("useLocalTime"))
    {
        struct tm *timeInfo;
        timeInfo = localtime(&t);
        line = strprintf("%s[%02d:%02d] %s%s", lineColor.c_str(),
            timeInfo->tm_hour, timeInfo->tm_min, tmp.nick.c_str(),
            tmp.text.c_str());
    }
    else
    {
        // Format the time string properly
        std::stringstream timeStr;
        timeStr << "[" << ((((t / 60) / 60) % 24 < 10) ? "0" : "")
            << static_cast<int>(((t / 60) / 60) % 24)
            << ":" << (((t / 60) % 60 < 10) ? "0" : "")
            << static_cast<int>((t / 60) % 60)
            << "] ";
        line = lineColor + timeStr.str() + tmp.nick + tmp.text;
    }

    if (config.getBoolValue("enableChatLog"))
        saveToLogFile(line);

    mTextOutput->setMaxRow(config.getIntValue("chatMaxLinesLimit"));

    // We look if the Vertical Scroll Bar is set at the max before
    // adding a row, otherwise the max will always be a row higher
    // at comparison.
    if (mScrollArea->getVerticalScrollAmount() >=
        mScrollArea->getVerticalMaxScroll())
    {
        addRow(line);
        mScrollArea->setVerticalScrollAmount(
            mScrollArea->getVerticalMaxScroll());
    }
    else
    {
        addRow(line);
    }

    if (chatWindow && this == localChatTab)
        chatWindow->addToAwayLog(line);

    mScrollArea->logic();
    if (own != BY_PLAYER)
    {
        if (own == BY_SERVER && (getType() == TAB_PARTY
            || getType() == TAB_GUILD))
        {
            return;
        }

        if (!getTabbedArea())
            return;

        if (this != getTabbedArea()->getSelectedTab())
        {
            if (getFlash() == 0)
            {
                if (player_node)
                {
                    std::string::size_type pos
                        = tmp.text.find(player_node->getName());
                    if (pos != std::string::npos)
                        setFlash(2);
                    else
                        setFlash(1);
                }
                else
                {
                    setFlash(1);
                }
            }
        }

        if (getAllowHighlight() && (this != getTabbedArea()->getSelectedTab()
            || (Client::getIsMinimized() || (!Client::getMouseFocused()
            && !Client::getInputFocused()))))
        {
            if (own != BY_SERVER)
                sound.playGuiSfx("system/newmessage.ogg");
        }
    }
}

void ChatTab::chatLog(const std::string &nick, std::string msg)
{
    Own byWho = (nick == player_node->getName() ? BY_PLAYER : BY_OTHER);
    if (byWho == BY_OTHER && config.getBoolValue("removeColors"))
        msg = removeColors(msg);
    chatLog(nick + " : " + msg, byWho, false, false);
}

void ChatTab::chatInput(const std::string &message)
{
    std::string msg = message;
    trim(msg);

    if (msg.empty())
        return;

    // Check for item link
    std::string::size_type start = msg.find('[');
    while (start + 1 < msg.size() && start != std::string::npos
           && msg[start + 1] != '@')
    {
        std::string::size_type end = msg.find(']', start);
        if (start + 1 != end && end != std::string::npos)
        {
            // Catch multiple embeds and ignore them
            // so it doesn't crash the client.
            while ((msg.find('[', start + 1) != std::string::npos) &&
                   (msg.find('[', start + 1) < end))
            {
                start = msg.find('[', start + 1);
            }

            std::string temp = "";
            if (start + 1 < msg.length() && end < msg.length()
                && end > start + 1)
            {
                temp = msg.substr(start + 1, end - start - 1);

                const ItemInfo &itemInfo = ItemDB::get(temp);
                if (itemInfo.getId() != 0)
                {
                    msg.insert(end, "@@");
                    msg.insert(start + 1, "|");
                    msg.insert(start + 1, toString(itemInfo.getId()));
                    msg.insert(start + 1, "@@");
                }
            }
        }
        start = msg.find('[', start + 1);
    }

    // Prepare ordinary message
    if (msg[0] != '/')
        handleInput(msg);
    else
        handleCommand(std::string(msg, 1));
}

void ChatTab::scroll(int amount)
{
    int range = mScrollArea->getHeight() / 8 * amount;
    gcn::Rectangle scr;
    scr.y = mScrollArea->getVerticalScrollAmount() + range;
    scr.height = abs(range);
    mTextOutput->showPart(scr);
}

void ChatTab::clearText()
{
    mTextOutput->clearRows();
}

void ChatTab::handleInput(const std::string &msg)
{
    if (chatWindow)
        Net::getChatHandler()->talk(chatWindow->doReplace(msg));
    else
        Net::getChatHandler()->talk(msg);
}

void ChatTab::handleCommand(const std::string &msg)
{
    if (commandHandler)
        commandHandler->handleCommands(msg, this);
}

bool ChatTab::handleCommands(const std::string &type, const std::string &args)
{
    // need split to commands and call each

    return handleCommand(type, args);
}

void ChatTab::saveToLogFile(std::string &msg)
{
    if (getType() == TAB_INPUT && chatLogger)
        chatLogger->log(msg);
}

int ChatTab::getType() const
{
    if (getCaption() == "General" || getCaption() == _("General"))
        return TAB_INPUT;
    else if (getCaption() == "Debug" || getCaption() == _("Debug"))
        return TAB_DEBUG;
    else
        return TAB_UNKNOWN;
}

void ChatTab::addRow(std::string &line)
{
    std::string::size_type idx = 0;

    for (unsigned int f = 0; f < line.length(); f++)
    {
        if (line.at(f) == ' ')
        {
            idx = f;
        }
        else if (f - idx > MAX_WORD_SIZE)
        {
            line.insert(f, " ");
            idx = f;
        }
    }
    mTextOutput->addRow(line);
}

void ChatTab::loadFromLogFile(std::string name)
{
    if (chatLogger)
    {
        std::list<std::string> list;
        chatLogger->loadLast(name, list, 5);
        std::list<std::string>::iterator i = list.begin();
        while (i != list.end())
        {
            std::string line = "##9" + *i;
            addRow(line);
            ++i;
        }
    }
}

void ChatTab::addNewRow(std::string &line)
{
    if (mScrollArea->getVerticalScrollAmount() >=
        mScrollArea->getVerticalMaxScroll())
    {
        addRow(line);
        mScrollArea->setVerticalScrollAmount(
            mScrollArea->getVerticalMaxScroll());
    }
    else
    {
        addRow(line);
    }
    mScrollArea->logic();
}