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

typedef std::queue<std::string> WhisperQueue;

class ChatHandler notfinal : public Net::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        void me(const std::string &restrict text,
                const std::string &restrict channel) const override final;

        void clear() override final;

    protected:
        static void processMVPEffect(Net::MessageIn &msg);

        static void processIgnoreAllResponse(Net::MessageIn &msg);

        static void processWhisperResponseContinue(Net::MessageIn &msg,
                                                   const uint8_t type);

        static std::string getPopLastWhisperNick();

        static std::string getLastWhisperNick();

        static WhisperQueue mSentWhispers;
        static int mMotdTime;
        static bool mShowAllLang;
        static bool mShowMotd;
        static bool mSkipping;
};

}  // namespace Ea

#endif  // NET_EA_CHATHANDLER_H
