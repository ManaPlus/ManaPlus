/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/tabs/chat/chattab.h"

#include "chatlogger.h"
#include "configuration.h"
#include "settings.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "const/sound.h"

#include "gui/chatlog.h"
#include "gui/windowmanager.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/helpwindow.h"

#include "gui/widgets/scrollarea.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/tabbedarea.h"

#include "input/inputmanager.h"

#include "net/chathandler.h"
#include "net/serverfeatures.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/gettext.h"

#ifdef WIN32
#include <sys/time.h>
#endif  // WIN32

#include <sstream>

#include "debug.h"

ChatTab *localChatTab = nullptr;
ChatTab *debugChatTab = nullptr;

static const unsigned int MAX_WORD_SIZE = 50;

ChatTab::ChatTab(const Widget2 *const widget,
                 const std::string &name,
                 const std::string &channel,
                 const std::string &logName,
                 const ChatTabTypeT &type) :
    Tab(widget),
    mTextOutput(new BrowserBox(this, BrowserBox::AUTO_WRAP, Opaque_true,
       "browserbox.xml")),
    mScrollArea(new ScrollArea(this, mTextOutput, Opaque_false)),
    mChannelName(channel),
    mLogName(logName),
    mType(type),
    mAllowHightlight(true),
    mRemoveNames(false),
    mNoAway(false),
    mShowOnline(false)
{
    setCaption(name);

    mTextOutput->setOpaque(Opaque_false);
    mTextOutput->setMaxRow(config.getIntValue("ChatLogLength"));
    if (chatWindow)
        mTextOutput->setLinkHandler(chatWindow->mItemLinkHandler);
    mTextOutput->setAlwaysUpdate(false);

    mScrollArea->setScrollPolicy(ScrollArea::SHOW_NEVER,
        ScrollArea::SHOW_ALWAYS);
    mScrollArea->setScrollAmount(0, 1);

    if (chatWindow)
        chatWindow->addTab(this);
}

ChatTab::~ChatTab()
{
    if (chatWindow)
        chatWindow->removeTab(this);

    delete2(mTextOutput);
    delete2(mScrollArea);
}

void ChatTab::chatLog(std::string line,
                      ChatMsgTypeT own,
                      const IgnoreRecord ignoreRecord,
                      const TryRemoveColors tryRemoveColors)
{
    // Trim whitespace
    trim(line);

    if (line.empty())
        return;

    if (tryRemoveColors == TryRemoveColors_true &&
        own == ChatMsgType::BY_OTHER &&
        config.getBoolValue("removeColors"))
    {
        line = removeColors(line);
        if (line.empty())
            return;
    }

    const unsigned lineLim = config.getIntValue("chatMaxCharLimit");
    if (lineLim > 0 && line.length() > lineLim)
        line = line.substr(0, lineLim);

    if (line.empty())
        return;

    CHATLOG tmp;
    tmp.own = own;
    tmp.nick.clear();
    tmp.text = line;

    const size_t pos = line.find(" : ");
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
            own = ChatMsgType::BY_SERVER;
    }

    // *implements actions in a backwards compatible way*
    if ((own == ChatMsgType::BY_PLAYER || own == ChatMsgType::BY_OTHER) &&
        tmp.text.at(0) == '*' &&
        tmp.text.at(tmp.text.length()-1) == '*')
    {
        tmp.text[0] = ' ';
        tmp.text.erase(tmp.text.length() - 1);
        own = ChatMsgType::ACT_IS;
    }

    std::string lineColor("##C");
    switch (own)
    {
        case ChatMsgType::BY_GM:
            if (tmp.nick.empty())
            {
                // TRANSLATORS: chat message
                tmp.nick = std::string(_("Global announcement:")).append(" ");
                lineColor = "##G";
            }
            else
            {
                // TRANSLATORS: chat message
                tmp.nick = strprintf(_("Global announcement from %s:"),
                                     tmp.nick.c_str()).append(" ");
                lineColor = "##g";  // Equiv. to BrowserBox::RED
            }
            break;
        case ChatMsgType::BY_PLAYER:
            tmp.nick.append(": ");
            lineColor = "##Y";
            break;
        case ChatMsgType::BY_OTHER:
        case ChatMsgType::BY_UNKNOWN:
            tmp.nick.append(": ");
            lineColor = "##C";
            break;
        case ChatMsgType::BY_SERVER:
            // TRANSLATORS: chat message
            tmp.nick.clear();
            tmp.text = line;
            lineColor = "##S";
            break;
        case ChatMsgType::BY_CHANNEL:
            tmp.nick.clear();
            lineColor = "##2";  // Equiv. to BrowserBox::GREEN
            break;
        case ChatMsgType::ACT_WHISPER:
            // TRANSLATORS: chat message
            tmp.nick = strprintf(_("%s whispers: %s"), tmp.nick.c_str(), "");
            lineColor = "##W";
            break;
        case ChatMsgType::ACT_IS:
            lineColor = "##I";
            break;
        case ChatMsgType::BY_LOGGER:
            tmp.nick.clear();
            tmp.text = line;
            lineColor = "##L";
            break;
        default:
            break;
    }

    if (tmp.nick == ": ")
    {
        tmp.nick.clear();
        lineColor = "##S";
    }

    // if configured, move magic messages log to debug chat tab
    if (!serverFeatures->haveChatChannels()
        && localChatTab && this == localChatTab
        && ((config.getBoolValue("showMagicInDebug")
        && own == ChatMsgType::BY_PLAYER
        && tmp.text.length() > 1
        && tmp.text.at(0) == '#'
        && tmp.text.at(1) != '#')
        || (config.getBoolValue("serverMsgInDebug")
        && (own == ChatMsgType::BY_SERVER
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
        const tm *const timeInfo = localtime(&t);
        if (timeInfo)
        {
            line = strprintf("%s[%02d:%02d] %s%s", lineColor.c_str(),
                timeInfo->tm_hour, timeInfo->tm_min, tmp.nick.c_str(),
                tmp.text.c_str());
        }
        else
        {
            line = strprintf("%s %s%s", lineColor.c_str(),
                tmp.nick.c_str(), tmp.text.c_str());
        }
    }
    else
    {
        // Format the time string properly
        std::stringstream timeStr;
        timeStr << "[" << ((((t / 60) / 60) % 24 < 10) ? "0" : "")
            << CAST_S32(((t / 60) / 60) % 24)
            << ":" << (((t / 60) % 60 < 10) ? "0" : "")
            << CAST_S32((t / 60) % 60)
            << "] ";
        line = std::string(lineColor).append(timeStr.str()).append(
            tmp.nick).append(tmp.text);
    }

    if (config.getBoolValue("enableChatLog"))
        saveToLogFile(line);

    mTextOutput->setMaxRow(config.getIntValue("chatMaxLinesLimit"));

    // We look if the Vertical Scroll Bar is set at the max before
    // adding a row, otherwise the max will always be a row higher
    // at comparison.
    if (mScrollArea->getVerticalScrollAmount() + 2 >=
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
    if (own != ChatMsgType::BY_PLAYER)
    {
        if (own == ChatMsgType::BY_SERVER &&
            (getType() == ChatTabType::PARTY ||
            getType() == ChatTabType::CHANNEL ||
            getType() == ChatTabType::GUILD))
        {
            return;
        }

        const TabbedArea *const tabArea = getTabbedArea();
        if (!tabArea)
            return;

        const bool notFocused = WindowManager::getIsMinimized() ||
            (!settings.mouseFocused && !settings.inputFocused);

        if (this != tabArea->getSelectedTab() || notFocused)
        {
            if (getFlash() == 0)
            {
                if (chatWindow && chatWindow->findHighlight(tmp.text))
                {
                    setFlash(2);
                    soundManager.playGuiSound(SOUND_HIGHLIGHT);
                }
                else
                {
                    setFlash(1);
                }
            }
            else if (getFlash() == 2)
            {
                if (chatWindow && chatWindow->findHighlight(tmp.text))
                    soundManager.playGuiSound(SOUND_HIGHLIGHT);
            }
        }

        if ((getAllowHighlight() ||
            own == ChatMsgType::BY_GM) &&
            (this != tabArea->getSelectedTab() ||
            notFocused))
        {
            if (own == ChatMsgType::BY_GM)
            {
                if (chatWindow)
                    chatWindow->unHideWindow();
                soundManager.playGuiSound(SOUND_GLOBAL);
            }
            else if (own != ChatMsgType::BY_SERVER)
            {
                if (chatWindow)
                    chatWindow->unHideWindow();
                playNewMessageSound();
            }
            WindowManager::newChatMessage();
        }
    }
}

void ChatTab::chatLog(const std::string &nick, std::string msg)
{
    if (!localPlayer)
        return;

    const ChatMsgTypeT byWho = (nick == localPlayer->getName()
        ? ChatMsgType::BY_PLAYER : ChatMsgType::BY_OTHER);
    if (byWho == ChatMsgType::BY_OTHER && config.getBoolValue("removeColors"))
        msg = removeColors(msg);
    chatLog(std::string(nick).append(" : ").append(msg),
        byWho,
        IgnoreRecord_false,
        TryRemoveColors_false);
}

void ChatTab::chatInput(const std::string &message)
{
    std::string msg = message;
    trim(msg);

    if (msg.empty())
        return;

    replaceItemLinks(msg);
    replaceVars(msg);

    switch (msg[0])
    {
        case '/':
            handleCommandStr(std::string(msg, 1));
            break;
        case '?':
            if (msg.size() > 1 &&
                msg[1] != '!' &&
                msg[1] != '?' &&
                msg[1] != '.' &&
                msg[1] != ' ' &&
                msg[1] != ',')
            {
                handleHelp(std::string(msg, 1));
            }
            else
            {
                handleInput(msg);
            }
            break;
        default:
            handleInput(msg);
            break;
    }
}

void ChatTab::scroll(const int amount)
{
    const int range = mScrollArea->getHeight() / 8 * amount;
    Rect scr;
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
    chatHandler->talk(ChatWindow::doReplace(msg),
        mChannelName);
}

void ChatTab::handleCommandStr(const std::string &msg)
{
    const size_t pos = msg.find(' ');
    const std::string type(msg, 0, pos);
    std::string args(msg, pos == std::string::npos ? msg.size() : pos + 1);

    args = trim(args);
    if (!handleCommand(type, args))
        inputManager.executeChatCommand(type, args, this);
}

void ChatTab::handleHelp(const std::string &msg)
{
    if (helpWindow)
    {
        helpWindow->search(msg);
        helpWindow->requestMoveToTop();
    }
}

bool ChatTab::handleCommands(const std::string &type, const std::string &args)
{
    // need split to commands and call each

    return handleCommand(type, args);
}

void ChatTab::saveToLogFile(const std::string &msg) const
{
    if (chatLogger)
    {
        if (getType() == ChatTabType::INPUT)
        {
            chatLogger->log(msg);
        }
        else if (getType() == ChatTabType::DEBUG)
        {
            if (config.getBoolValue("enableDebugLog"))
                chatLogger->log("#Debug", msg);
        }
        else if (!mLogName.empty())
        {
            chatLogger->log(mLogName, msg);
        }
    }
}

void ChatTab::addRow(std::string &line)
{
    if (line.find("[@@http") == std::string::npos)
    {
        size_t idx = 0;
        for (size_t f = 0; f < line.length(); f++)
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
    }
    mTextOutput->addRow(line);
}

void ChatTab::loadFromLogFile(const std::string &name)
{
    if (chatLogger)
    {
        std::list<std::string> list;
        chatLogger->loadLast(name, list, 5);
        std::list<std::string>::const_iterator i = list.begin();
        while (i != list.end())
        {
            std::string line("##o" + *i);
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

void ChatTab::playNewMessageSound() const
{
    soundManager.playGuiSound(SOUND_WHISPER);
}

void ChatTab::showOnline(const std::string &nick,
                         const Online online)
{
    if (!mShowOnline)
        return;

    if (online == Online_true)
    {
        // TRANSLATORS: chat message
        chatLog(strprintf(_("%s is now Online."), nick.c_str()),
            ChatMsgType::BY_SERVER);
    }
    else
    {
        // TRANSLATORS: chat message
        chatLog(strprintf(_("%s is now Offline."), nick.c_str()),
            ChatMsgType::BY_SERVER);
    }
}
