/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#include "net/tmwa/chathandler.h"

#include "actorspritemanager.h"
#include "being.h"
#include "configuration.h"
#include "event.h"
#include "game.h"
#include "localplayer.h"
#include "playerrelations.h"
#include "log.h"

#include "gui/chat.h"
#include "gui/shopwindow.h"

#include "gui/widgets/chattab.h"

#include "net/messagein.h"
#include "net/messageout.h"

#include "net/tmwa/protocol.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <string>

extern Net::ChatHandler *chatHandler;

namespace TmwAthena
{

ChatHandler::ChatHandler()
{
    static const Uint16 _messages[] =
    {
        SMSG_BEING_CHAT,
        SMSG_PLAYER_CHAT,
        SMSG_WHISPER,
        SMSG_WHISPER_RESPONSE,
        SMSG_GM_CHAT,
        SMSG_MVP, // MVP
        0
    };
    handledMessages = _messages;
    chatHandler = this;
}

void ChatHandler::handleMessage(Net::MessageIn &msg)
{
    if (!localChatTab)
        return;

    Being *being;
    std::string chatMsg;
    std::string nick;
    int chatMsgLength;

    switch (msg.getId())
    {
        case SMSG_WHISPER_RESPONSE:
        {
            if (mSentWhispers.empty())
            {
                nick = "user";
            }
            else
            {
                nick = mSentWhispers.front();
                mSentWhispers.pop();
            }

            int type = msg.readInt8();
            switch (type)
            {
                case 0x00:
                    // Success (don't need to report)
                    break;
                case 0x01:
                    if (chatWindow)
                    {
                        chatWindow->whisper(nick,
                            strprintf(_("Whisper could not be "
                            "sent, %s is offline."), nick.c_str()), BY_SERVER);
                    }
                    break;
                case 0x02:
                    if (chatWindow)
                    {
                        chatWindow->whisper(nick,
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
            break;
        }

        // Received whisper
        case SMSG_WHISPER:
        {
            chatMsgLength = msg.readInt16() - 28;
            nick = msg.readString(24);

            if (chatMsgLength <= 0)
                break;

            chatMsg = msg.readString(chatMsgLength);

            if (nick != "Server")
            {
                if (player_relations.hasPermission(
                    nick, PlayerRelation::WHISPER))
                {
                    bool tradeBot = config.getBoolValue("tradebot");
                    bool showMsg = !config.getBoolValue("hideShopMessages");
                    if (player_relations.hasPermission(
                        nick, PlayerRelation::TRADE))
                    {
                        if (shopWindow)
                        {   //commands to shop from player
                            if (chatMsg.find("!selllist ") == 0)
                            {
                                if (tradeBot)
                                {
                                    if (showMsg && chatWindow)
                                        chatWindow->whisper(nick, chatMsg);
                                    shopWindow->giveList(nick,
                                        ShopWindow::SELL);
                                }
                            }
                            else if (chatMsg.find("!buylist ") == 0)
                            {
                                if (tradeBot)
                                {
                                    if (showMsg && chatWindow)
                                        chatWindow->whisper(nick, chatMsg);
                                    shopWindow->giveList(nick,
                                        ShopWindow::BUY);
                                }
                            }
                            else if (chatMsg.find("!buyitem ") == 0)
                            {
                                if (showMsg && chatWindow)
                                    chatWindow->whisper(nick, chatMsg);
                                if (tradeBot)
                                {
                                    shopWindow->processRequest(nick, chatMsg,
                                            ShopWindow::BUY);
                                }
                            }
                            else if (chatMsg.find("!sellitem ") == 0)
                            {
                                if (showMsg && chatWindow)
                                    chatWindow->whisper(nick, chatMsg);
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
                                    chatWindow->whisper(nick, chatMsg);
                                if (chatMsg.find("B1") == 0
                                    || chatMsg.find("S1") == 0)
                                {
                                    shopWindow->showList(nick, chatMsg);
                                }
                            }
                            else if (chatWindow)
                            {
                                chatWindow->whisper(nick, chatMsg);
                            }
                        }
                        else if (chatWindow)
                        {
                            chatWindow->whisper(nick, chatMsg);
                        }
                    }
                    else
                    {
                        if (chatWindow && (showMsg
                            || (chatMsg.find("!selllist")
                            != 0 && chatMsg.find("!buylist") != 0)))
                        {
                            chatWindow->whisper(nick, chatMsg);
                        }
                    }
                }
            }
            else if (localChatTab)
            {
                localChatTab->chatLog(chatMsg, BY_SERVER);
            }

            break;
        }

        // Received speech from being
        case SMSG_BEING_CHAT:
        {
            if (!actorSpriteManager)
                return;

            chatMsgLength = msg.readInt16() - 8;
            being = actorSpriteManager->findBeing(msg.readInt32());

            if (!being || chatMsgLength <= 0)
                break;

            std::string str2;
            chatMsg = msg.readRawString(chatMsgLength);

            if (being->getType() == Being::PLAYER)
                being->setTalkTime();

            std::string::size_type pos = chatMsg.find(" : ", 0);
            std::string sender_name = ((pos == std::string::npos)
                                       ? "" : chatMsg.substr(0, pos));

            if (sender_name != being->getName()
                && being->getType() == Being::PLAYER)
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
                chatWindow->resortChatLog(removeColors(sender_name) + " : "
                                          + chatMsg, BY_OTHER);
            }

            if (player_relations.hasPermission(sender_name,
                PlayerRelation::SPEECH_FLOAT))
            {
                being->setSpeech(chatMsg, SPEECH_TIME);
            }
            break;
        }

        case SMSG_PLAYER_CHAT:
        case SMSG_GM_CHAT:
        {
            chatMsgLength = msg.readInt16() - 4;

            if (chatMsgLength <= 0)
                break;

            std::string str2;
            chatMsg = msg.readRawString(chatMsgLength);
            std::string::size_type pos = chatMsg.find(" : ", 0);

            if (msg.getId() == SMSG_PLAYER_CHAT)
            {
                if (chatWindow)
                    chatWindow->resortChatLog(chatMsg, BY_PLAYER);
//                if (localChatTab)
//                    localChatTab->chatLog(chatMsg, BY_PLAYER);

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
                    player_node->setSpeech(chatMsg, SPEECH_TIME);
            }
            else if (localChatTab)
            {
                localChatTab->chatLog(chatMsg, BY_GM);
            }
            break;
        }

        case SMSG_MVP:
        {
            // Display MVP player
            int id = msg.readInt32(); // id
            if (localChatTab && actorSpriteManager)
            {
                being = actorSpriteManager->findBeing(id);
                if (!being)
                {
                    localChatTab->chatLog(_("MVP player."), BY_SERVER);
                }
                else
                {
                    localChatTab->chatLog(_("MVP player: ")
                        + being->getName(), BY_SERVER);
                }
            }
            break;
        }

        default:
            break;
    }
}

void ChatHandler::talk(const std::string &text)
{
    if (!player_node)
        return;

    std::string mes = player_node->getName() + " : " + text;
//    std::string mes = player_node->getName() + "zzzz : " + text;

    MessageOut outMsg(CMSG_CHAT_MESSAGE);
    // Added + 1 in order to let eAthena parse admin commands correctly
    outMsg.writeInt16(static_cast<short>(mes.length() + 4 + 1));
    outMsg.writeString(mes, static_cast<int>(mes.length() + 1));
}

void ChatHandler::talkRaw(const std::string &mes)
{
    MessageOut outMsg(CMSG_CHAT_MESSAGE);
    // Added + 1 in order to let eAthena parse admin commands correctly
    outMsg.writeInt16(static_cast<short>(mes.length() + 4 + 1));
    outMsg.writeString(mes, static_cast<int>(mes.length() + 1));
}

void ChatHandler::me(const std::string &text)
{
    std::string action = strprintf("*%s*", text.c_str());

    talk(action);
}

void ChatHandler::privateMessage(const std::string &recipient,
                                 const std::string &text)
{
    MessageOut outMsg(CMSG_CHAT_WHISPER);
    outMsg.writeInt16(static_cast<short>(text.length() + 28));
    outMsg.writeString(recipient, 24);
    outMsg.writeString(text, static_cast<int>(text.length()));
    mSentWhispers.push(recipient);
}

void ChatHandler::channelList()
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::enterChannel(const std::string &channel _UNUSED_,
                               const std::string &password _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::quitChannel(int channelId _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::sendToChannel(int channelId _UNUSED_,
                                const std::string &text _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::userList(const std::string &channel _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::setChannelTopic(int channelId _UNUSED_,
                                  const std::string &text _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::setUserMode(int channelId _UNUSED_,
                              const std::string &name _UNUSED_,
                              int mode _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::kickUser(int channelId _UNUSED_,
                           const std::string &name _UNUSED_)
{
    SERVER_NOTICE(_("Channels are not supported!"))
}

void ChatHandler::who()
{
    MessageOut outMsg(CMSG_WHO_REQUEST);
}

void ChatHandler::sendRaw(const std::string &args)
{
    std::string line = args;
    std::string str;
    MessageOut *outMsg = 0;

    if (line == "")
        return;

    std::string::size_type pos = line.find(" ");
    if (pos != std::string::npos)
    {
        str = line.substr(0, pos);
        outMsg = new MessageOut(static_cast<short>(atoi(str.c_str())));
        line = line.substr(pos + 1);
        pos = line.find(" ");
    }
    else
    {
        outMsg = new MessageOut(static_cast<short>(atoi(line.c_str())));
        delete outMsg;
        return;
    }

    while (pos != std::string::npos)
    {
        str = line.substr(0, pos);
        processRaw(*outMsg, str);
        line = line.substr(pos + 1);
        pos = line.find(" ");
    }
    if (outMsg)
    {
        if (line != "")
            processRaw(*outMsg, line);
        delete outMsg;
    }
}

void ChatHandler::processRaw(MessageOut &outMsg, std::string &line)
{
    std::string::size_type pos = line.find(":");
    if (pos == std::string::npos)
    {
        int i = atoi(line.c_str());
        if (line.length() <= 3)
            outMsg.writeInt8(static_cast<char>(i));
        else if (line.length() <= 5)
            outMsg.writeInt16(static_cast<short>(i));
        else
            outMsg.writeInt32(i);
    }
    else
    {
        std::string header = line.substr(0, pos);
        std::string data = line.substr(pos + 1);
        if (header.length() != 1)
            return;

        int i = 0;

        switch (header[0])
        {
            case '1':
            case '2':
            case '4':
                i = atoi(data.c_str());
                break;
            default:
                break;
        }

        switch (header[0])
        {
            case '1':
                outMsg.writeInt8(static_cast<char>(i));
                break;
            case '2':
                outMsg.writeInt16(static_cast<short>(i));
                break;
            case '4':
                outMsg.writeInt32(i);
                break;
            case 'c':
            {
                pos = line.find(",");
                if (pos != std::string::npos)
                {
                    unsigned short x = static_cast<unsigned short>(
                            atoi(data.substr(0, pos).c_str()));
                    data = data.substr(pos + 1);
                    pos = line.find(",");
                    if (pos == std::string::npos)
                        break;

                    unsigned short y = static_cast<unsigned short>(
                        atoi(data.substr(0, pos).c_str()));
                    int dir = atoi(data.substr(pos + 1).c_str());
                    outMsg.writeCoordinates(x, y,
                        static_cast<unsigned char>(dir));
                }
                break;
            }
            case 't':
                outMsg.writeString(data, static_cast<int>(data.length()));
                break;
            default:
                break;
        }
    }
}

} // namespace TmwAthena

