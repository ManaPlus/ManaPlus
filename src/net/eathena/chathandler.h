/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_EATHENA_CHATHANDLER_H
#define NET_EATHENA_CHATHANDLER_H

#include "net/ea/chathandler.h"

#include "enums/gui/chatmsgtype.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class MessageOut;

class ChatHandler final : public MessageHandler, public Ea::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void talk(const std::string &restrict text,
                  const std::string &restrict channel) const override final;

        void talkRaw(const std::string &text) const override final;

        void privateMessage(const std::string &restrict recipient,
                            const std::string &restrict text) override final;

        void channelMessage(const std::string &restrict channel,
                            const std::string &restrict text) override final;

        void joinChannel(const std::string &channel) override final;

        void who() const override final;

        void sendRaw(const std::string &args) const override final;

        void ignoreAll() const override final;

        void unIgnoreAll() const override final;

        void createChatRoom(const std::string &title,
                            const std::string &password,
                            const int limit,
                            const bool isPublic) override final;

        static void processRaw(MessageOut &restrict outMsg,
                               const std::string &restrict line);

        void ignore(const std::string &nick) const override final;

        void unIgnore(const std::string &nick) const override final;

        void requestIgnoreList() const override final;

        void battleTalk(const std::string &text) const override final;

        void joinChat(const ChatObject *const chat,
                      const std::string &password) const override final;

        void partChannel(const std::string &channel) override final;

        void talkPet(const std::string &restrict text,
                     const std::string &restrict channel) const override final;

    protected:
        static std::string extractChannelFromMessage(std::string &chatMsg);

        static void processChat(Net::MessageIn &msg);

        static void processColorChat(Net::MessageIn &msg);

        static void processChatContinue(std::string chatMsg,
                                        ChatMsgType::Type own);

        static void processWhisper(Net::MessageIn &msg);

        static void processWhisperResponse(Net::MessageIn &msg);

        static void processGmChat(Net::MessageIn &msg);

        static void processGmChat2(Net::MessageIn &msg);

        static void processChatIgnoreList(Net::MessageIn &msg);

        static void processFormatMessage(Net::MessageIn &msg);

        static void processFormatMessageNumber(Net::MessageIn &msg);

        static void processFormatMessageSkill(Net::MessageIn &msg);

        static void processChatDisplay(Net::MessageIn &msg);

        static void processChatJoinAck(Net::MessageIn &msg);

        static void processChatLeave(Net::MessageIn &msg);

        static void processJoinChannel(Net::MessageIn &msg);

        static void processWhisperContinue(const std::string &nick,
                                           std::string chatMsg);

        static void processBeingChat(Net::MessageIn &msg);

        static void processIgnoreNickAck(Net::MessageIn &msg);

        static void processChatCreateAck(Net::MessageIn &msg);

        static void processChatDestroy(Net::MessageIn &msg);

        static void processChatJoinFailed(Net::MessageIn &msg);

        static void processChatAddMember(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // NET_EATHENA_CHATHANDLER_H
