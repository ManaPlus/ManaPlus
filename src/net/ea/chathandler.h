/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef NET_EA_CHATHANDLER_H
#define NET_EA_CHATHANDLER_H

#include "net/chathandler.h"

#include <queue>

namespace Net
{
    class MessageIn;
}

namespace Ea
{

class ChatHandler notfinal : public Net::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        void talkPet(const std::string &restrict text,
                     const std::string &restrict channel) const override final;

        void me(const std::string &restrict text,
                const std::string &restrict channel) const override final;

        virtual void processBeingChat(Net::MessageIn &msg) const;

        virtual void processMVP(Net::MessageIn &msg) const;

        virtual void processIgnoreAllResponse(Net::MessageIn &msg) const;

        void clear() override final;

        void processWhisperContinue(const std::string &nick,
                                    std::string chatMsg) const;

        void processWhisperResponseContinue(const uint8_t type);

    protected:
        std::string getPopLastWhisperNick();

        std::string getLastWhisperNick();

        typedef std::queue<std::string> WhisperQueue;
        WhisperQueue mSentWhispers;
        int mMotdTime;
        bool mShowAllLang;
        bool mShowMotd;
        bool mSkipping;
};

}  // namespace Ea

#endif  // NET_EA_CHATHANDLER_H
