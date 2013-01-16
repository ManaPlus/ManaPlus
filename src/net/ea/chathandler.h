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

#ifndef NET_EA_CHATHANDLER_H
#define NET_EA_CHATHANDLER_H

#include "net/chathandler.h"
#include "net/messagein.h"
#include "net/messageout.h"
#include "net/net.h"

#include <queue>

namespace Ea
{

class ChatHandler : public Net::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        virtual void talk(const std::string &text) = 0;

        virtual void talkRaw(const std::string &text) = 0;

        void me(const std::string &text);

        virtual void privateMessage(const std::string &recipient,
                                    const std::string &text) = 0;

        void channelList();

        void enterChannel(const std::string &channel,
                          const std::string &password);

        void quitChannel(int channelId);

        void sendToChannel(int channelId, const std::string &text);

        void userList(const std::string &channel);

        void setChannelTopic(int channelId, const std::string &text);

        void setUserMode(int channelId, const std::string &name, int mode);

        void kickUser(int channelId, const std::string &name);

        virtual void who() = 0;

        virtual void sendRaw(const std::string &args) = 0;

        virtual void processWhisperResponse(Net::MessageIn &msg);

        virtual void processWhisper(Net::MessageIn &msg);

        virtual void processBeingChat(Net::MessageIn &msg);

        virtual void processChat(Net::MessageIn &msg, bool normalChat);

        virtual void processMVP(Net::MessageIn &msg);

        virtual void processIgnoreAllResponse(Net::MessageIn &msg);

    protected:
        typedef std::queue<std::string> WhisperQueue;
        WhisperQueue mSentWhispers;
};

} // namespace Ea

#endif // NET_EA_CHATHANDLER_H
