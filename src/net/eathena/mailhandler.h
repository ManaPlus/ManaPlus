/*
 *  The ManaPlus Client
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

#ifndef NET_EATHENA_MAILHANDLER_H
#define NET_EATHENA_MAILHANDLER_H

#include "net/mailhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class MessageOut;

class MailHandler final : public MessageHandler, public Net::MailHandler
{
    public:
        MailHandler();

        A_DELETE_COPY(MailHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void refresh() override final;

        void readMessage(const int msgId) override final;

        void getAttach(const int msgId) override final;

        void deleteMessage(const int msgId) override final;

        void returnMessage(const int msgId) override final;

        void setAttach(const int index, const int amount) override final;

        void resetAttach(const int flag) override final;

        void send(const std::string &name,
                  const std::string &title,
                  std::string message) override final;

    protected:
        static void processMailOpen(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // NET_EATHENA_MAILHANDLER_H
