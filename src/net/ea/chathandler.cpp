/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "net/ea/chathandler.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "guildmanager.h"
#include "localplayer.h"
#include "notifymanager.h"
#include "playerrelations.h"

#include "gui/chatwindow.h"
#include "gui/shopwindow.h"

#include "gui/widgets/chattab.h"
#include "gui/widgets/gmtab.h"

#include <string>

#include "debug.h"

namespace Ea
{

ChatHandler::ChatHandler()
{
}

void ChatHandler::me(const std::string &text)
{
    std::string action = strprintf("*%s*", text.c_str());
    talk(action, GENERAL_CHANNEL);
}

void ChatHandler::channelList()
{
}

void ChatHandler::enterChannel(const std::string &channel A_UNUSED,
                               const std::string &password A_UNUSED)
{
}

void ChatHandler::quitChannel(int channelId A_UNUSED)
{
}

void ChatHandler::sendToChannel(int channelId A_UNUSED,
                                const std::string &text A_UNUSED)
{
}

void ChatHandler::userList(const std::string &channel A_UNUSED)
{
}

void ChatHandler::setChannelTopic(int channelId A_UNUSED,
                                  const std::string &text A_UNUSED)
{
}

void ChatHandler::setUserMode(int channelId A_UNUSED,
                              const std::string &name A_UNUSED,
                              int mode A_UNUSED)
{
}

void ChatHandler::kickUser(int channelId A_UNUSED,
                           const std::string &name A_UNUSED)
{
}

void ChatHandler::processWhisperResponse(Net::MessageIn &msg)
{
    std::string nick;

    if (mSentWhispers.empty())
    {
        nick = "user";
    }
    else
    {
        nick = mSentWhispers.front();
        mSentWhispers.pop();
    }

    const int type = msg.readInt8();
    switch (type)
    {
        case 0x00:
            // Success (don't need to report)
            break;
        case 0x01:
            if (chatWindow)
            {
                chatWindow->addWhisper(nick,
                    strprintf(_("Whisper could not be "
                    "sent, %s is offline."), nick.c_str()), BY_SERVER);
            }
            break;
        case 0x02:
            if (chatWindow)
            {
                chatWindow->addWhisper(nick,
                    strprintf(_("Whisper could not "
                    "be sent, ignored by %s."), nick.c_str()),
                    BY_SERVER);
            }
            break;
        default:
            if (logger)
            {
                logger->log("QQQ SMSG_WHISPER_RESPONSE:"
                            + toString(type));
            }
    }
}

void ChatHandler::processWhisper(Net::MessageIn &msg)
{
    const int chatMsgLength = msg.readInt16() - 28;
    std::string nick = msg.readString(24);

    if (chatMsgLength <= 0)
        return;

    std::string chatMsg = msg.readString(chatMsgLength);
    // ignoring future whisper messages
    if (chatMsg.find("\302\202G") == 0 || chatMsg.find("\302\202A") == 0)
        return;
    // remove first unicode space if this is may be whisper command.
    if (chatMsg.find("\302\202!") == 0)
        chatMsg = chatMsg.substr(2);

    if (nick != "Server")
    {
        if (guildManager && GuildManager::getEnableGuildBot()
            && nick == "guild" && guildManager->processGuildMessage(chatMsg))
        {
            return;
        }

        if (player_relations.hasPermission(nick, PlayerRelation::WHISPER))
        {
            const bool tradeBot = config.getBoolValue("tradebot");
            const bool showMsg = !config.getBoolValue("hideShopMessages");
            if (player_relations.hasPermission(nick, PlayerRelation::TRADE))
            {
                if (shopWindow)
                {   //commands to shop from player
                    if (chatMsg.find("!selllist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && chatWindow)
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::SELL);
                        }
                    }
                    else if (chatMsg.find("!buylist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && chatWindow)
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!buyitem ") == 0)
                    {
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (tradeBot)
                        {
                            shopWindow->processRequest(nick, chatMsg,
                                ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!sellitem ") == 0)
                    {
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (tradeBot)
                        {
                            shopWindow->processRequest(nick, chatMsg,
                                ShopWindow::SELL);
                        }
                    }
                    else if (chatMsg.length() > 3
                             && chatMsg.find("\302\202") == 0)
                    {
                        chatMsg = chatMsg.erase(0, 2);
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (chatMsg.find("B1") == 0 || chatMsg.find("S1") == 0)
                            shopWindow->showList(nick, chatMsg);
                    }
                    else if (chatWindow)
                    {
                        chatWindow->addWhisper(nick, chatMsg);
                    }
                }
                else if (chatWindow)
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
            else
            {
                if (chatWindow && (showMsg || (chatMsg.find("!selllist") != 0
                    && chatMsg.find("!buylist") != 0)))
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
        }
    }
    else if (localChatTab)
    {
        if (gmChatTab && strStartWith(chatMsg, "[GM] "))
        {
            chatMsg = chatMsg.substr(5);
            const size_t pos = chatMsg.find(": ", 0);
            if (pos == std::string::npos)
            {
                gmChatTab->chatLog(chatMsg);
            }
            else
            {
                gmChatTab->chatLog(chatMsg.substr(0, pos),
                    chatMsg.substr(pos + 2));
            }
        }
        else
        {
            localChatTab->chatLog(chatMsg, BY_SERVER);
        }
    }
}

void ChatHandler::processBeingChat(Net::MessageIn &msg)
{
    if (!actorSpriteManager)
        return;

    const int chatMsgLength = msg.readInt16() - 8;
    Being *const being = actorSpriteManager->findBeing(msg.readInt32());

    if (!being || chatMsgLength <= 0)
        return;

    std::string chatMsg = msg.readRawString(chatMsgLength);

    if (being->getType() == Being::PLAYER)
        being->setTalkTime();

    const size_t pos = chatMsg.find(" : ", 0);
    std::string sender_name = ((pos == std::string::npos)
        ? "" : chatMsg.substr(0, pos));

    if (sender_name != being->getName() && being->getType() == Being::PLAYER)
    {
        if (!being->getName().empty())
            sender_name = being->getName();
    }
    else
    {
        chatMsg.erase(0, pos + 3);
    }

    trim(chatMsg);

    // We use getIgnorePlayer instead of ignoringPlayer here
    // because ignorePlayer' side effects are triggered
    // right below for Being::IGNORE_SPEECH_FLOAT.
    if (player_relations.checkPermissionSilently(sender_name,
        PlayerRelation::SPEECH_LOG) && chatWindow)
    {
        chatWindow->resortChatLog(removeColors(sender_name)
            .append(" : ").append(chatMsg), BY_OTHER);
    }

    if (player_relations.hasPermission(sender_name,
        PlayerRelation::SPEECH_FLOAT))
    {
        being->setSpeech(chatMsg);
    }
}

void ChatHandler::processChat(Net::MessageIn &msg, bool normalChat)
{
    const int chatMsgLength = msg.readInt16() - 4;

    if (chatMsgLength <= 0)
        return;

    std::string chatMsg = msg.readRawString(chatMsgLength);
    const size_t pos = chatMsg.find(" : ", 0);

    if (normalChat)
    {
        if (chatWindow)
            chatWindow->resortChatLog(chatMsg, BY_PLAYER);

        const std::string senseStr = "You sense the following: ";
        if (actorSpriteManager && !chatMsg.find(senseStr))
        {
            actorSpriteManager->parseLevels(
                chatMsg.substr(senseStr.size()));
        }

        if (pos != std::string::npos)
            chatMsg.erase(0, pos + 3);

        trim(chatMsg);

        if (player_node)
            player_node->setSpeech(chatMsg);
    }
    else if (localChatTab)
    {
        localChatTab->chatLog(chatMsg, BY_GM);
    }
}

void ChatHandler::processMVP(Net::MessageIn &msg)
{
    // Display MVP player
    const int id = msg.readInt32(); // id
    if (localChatTab && actorSpriteManager && config.getBoolValue("showMVP"))
    {
        const Being *const being = actorSpriteManager->findBeing(id);
        if (!being)
            NotifyManager::notify(NotifyManager::MVP_PLAYER, "");
        else
            NotifyManager::notify(NotifyManager::MVP_PLAYER, being->getName());
    }
}

void ChatHandler::processIgnoreAllResponse(Net::MessageIn &msg)
{
    const int action = msg.readInt8();
    const int fail = msg.readInt8();
    if (!localChatTab)
        return;

    switch (action)
    {
        case 0:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyManager::WHISPERS_IGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyManager::
                        WHISPERS_IGNORE_FAILED);
                    break;
            }
            break;
        }
        case 1:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyManager::WHISPERS_UNIGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyManager::
                        WHISPERS_UNIGNORE_FAILED);
                    break;
            }
            break;
        }
        default:
            // unknown result
            break;
    }
}

} // namespace Ea
