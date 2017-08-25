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

#ifndef NET_TMWA_MAIL2HANDLER_H
#define NET_TMWA_MAIL2HANDLER_H

#include "net/mail2handler.h"

namespace TmwAthena
{

class Mail2Handler final : public Net::Mail2Handler
{
    public:
        Mail2Handler();

        A_DELETE_COPY(Mail2Handler)

        ~Mail2Handler();

        void openWriteMail(const std::string &receiver) const override final;

        void addItem(const Item *const item,
                     const int amount) const override final;

        void removeItem(const int index,
                        const int amount) const override final;

        void sendMail(const std::string &to,
                      const std::string &title,
                      const std::string &body,
                      const int64_t &money) const override final;

        void queueCheckName(const MailQueueTypeT type,
                            const std::string &to,
                            const std::string &title,
                            const std::string &body,
                            const int64_t &money) const override final;

        void nextPage(const MailOpenTypeT openType,
                      const int64_t mailId) const override final;

        void readMail(const MailOpenTypeT openType,
                      const int64_t mailId) const override final;

        void deleteMail(const MailOpenTypeT openType,
                        const int64_t mailId) const override final;

        void requestMoney(const MailOpenTypeT openType,
                          const int64_t mailId) const override final;

        void requestItems(const MailOpenTypeT openType,
                          const int64_t mailId) const override final;

        void refreshMailList(const MailOpenTypeT openType,
                             const int64_t mailId) const override final;

        void openMailBox(const MailOpenTypeT openType) const override final;

        void closeMailBox() const override final;

        void cancelWriteMail() const override final;

        void requestCheckName(const std::string &name) const override final;

        std::string getCheckedName() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_MAIL2HANDLER_H
