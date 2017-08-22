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

#ifndef NET_MAIL2HANDLER_H
#define NET_MAIL2HANDLER_H

#include "enums/net/mailopentype.h"

#include <string>

#include "localconsts.h"

class Item;

namespace Net
{

class Mail2Handler notfinal
{
    public:
        Mail2Handler()
        { }

        A_DELETE_COPY(Mail2Handler)

        virtual ~Mail2Handler()
        { }

        virtual void openWriteMail(const std::string &receiver) const = 0;

        virtual void addItem(const Item *const item,
                             const int amount) const = 0;

        virtual void removeItem(const Item *const item,
                                const int amount) const = 0;

        virtual void sendMail(const std::string &to,
                              const std::string &title,
                              const std::string &body,
                              const int64_t &money) const = 0;

        virtual void nextPage(const MailOpenTypeT openType,
                              const int64_t mailId) const = 0;

        virtual void readMail(const MailOpenTypeT openType,
                              const int64_t mailId) const = 0;

        virtual void deleteMail(const MailOpenTypeT openType,
                                const int64_t mailId) const = 0;

        virtual void requestMoney(const MailOpenTypeT openType,
                                  const int64_t mailId) const = 0;

        virtual void requestItems(const MailOpenTypeT openType,
                                  const int64_t mailId) const = 0;

        virtual void refreshMailList(const MailOpenTypeT openType,
                                     const int64_t mailId) const = 0;

        virtual void openMailBox(const MailOpenTypeT openType) const = 0;

        virtual void closeMailBox() const = 0;

        virtual void cancelWriteMail() const = 0;

        virtual void requestCheckName(const std::string &name) const = 0;
};

}  // namespace Net

extern Net::Mail2Handler *mail2Handler;

#endif  // NET_MAIL2HANDLER_H
