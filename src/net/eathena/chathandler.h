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

#ifndef NET_EATHENA_CHATHANDLER_H
#define NET_EATHENA_CHATHANDLER_H

#include "net/chathandler.h"
#include "net/net.h"

#include "net/ea/chathandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

class ChatHandler final : public MessageHandler, public Ea::ChatHandler
{
    public:
        ChatHandler();

        A_DELETE_COPY(ChatHandler)

        void handleMessage(Net::MessageIn &msg) override;

        void talk(const std::string &text,
                  const std::string &channel) const override;

        void talkRaw(const std::string &text) const override;

        void privateMessage(const std::string &recipient,
                            const std::string &text) override;

        void who() const override;

        void sendRaw(const std::string &args) const override;

        void ignoreAll() const override;

        void unIgnoreAll() const override;

        static void processRaw(MessageOut &outMsg, const std::string &line);
};

}  // namespace EAthena

#endif // NET_EATHENA_CHATHANDLER_H
