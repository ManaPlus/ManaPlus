/*
 *  The ManaPlus Client
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

#ifndef NET_TMWA_MAILHANDLER_H
#define NET_TMWA_MAILHANDLER_H

#include "net/mailhandler.h"

namespace TmwAthena
{

class MailHandler final : public Net::MailHandler
{
    public:
        MailHandler();

        A_DELETE_COPY(MailHandler)

        ~MailHandler() override final;

        void refresh() const override final A_CONST;

        void readMessage(const int msgId) const override final A_CONST;

        void getAttach(const int msgId) const override final A_CONST;

        void deleteMessage(const int msgId) const override final A_CONST;

        void returnMessage(const int msgId) const override final A_CONST;

        void setAttach(const int index,
                       const int amount) const override final A_CONST;

        void setAttachMoney(const int money) const override final A_CONST;

        void resetAttach(const int flag) const override final A_CONST;

        void send(const std::string &name,
                  const std::string &title,
                  std::string message) const override final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_MAILHANDLER_H
