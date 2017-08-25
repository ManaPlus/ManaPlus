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

#include "net/tmwa/mail2handler.h"

#include "debug.h"

namespace TmwAthena
{

Mail2Handler::Mail2Handler()
{
    mail2Handler = this;
}

Mail2Handler::~Mail2Handler()
{
    mail2Handler = nullptr;
}

void Mail2Handler::openWriteMail(const std::string &receiver A_UNUSED) const
{
}

void Mail2Handler::addItem(const Item *const item A_UNUSED,
                           const int amount A_UNUSED) const
{
}

void Mail2Handler::removeItem(const int index A_UNUSED,
                              const int amount A_UNUSED) const
{
}

void Mail2Handler::sendMail(const std::string &to A_UNUSED,
                            const std::string &title A_UNUSED,
                            const std::string &body A_UNUSED,
                            const int64_t &money A_UNUSED) const
{
}

void Mail2Handler::queueCheckName(const MailQueueTypeT type A_UNUSED,
                                  const std::string &to A_UNUSED,
                                  const std::string &title A_UNUSED,
                                  const std::string &body A_UNUSED,
                                  const int64_t &money A_UNUSED) const
{
}

void Mail2Handler::nextPage(const MailOpenTypeT openType A_UNUSED,
                            const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::readMail(const MailOpenTypeT openType A_UNUSED,
                            const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::deleteMail(const MailOpenTypeT openType A_UNUSED,
                              const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::requestMoney(const MailOpenTypeT openType A_UNUSED,
                                const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::requestItems(const MailOpenTypeT openType A_UNUSED,
                                const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::refreshMailList(const MailOpenTypeT openType A_UNUSED,
                                   const int64_t mailId A_UNUSED) const
{
}

void Mail2Handler::openMailBox(const MailOpenTypeT openType A_UNUSED) const
{
}

void Mail2Handler::closeMailBox() const
{
}

void Mail2Handler::cancelWriteMail() const
{
}

void Mail2Handler::requestCheckName(const std::string &name A_UNUSED) const
{
}

std::string Mail2Handler::getCheckedName() const
{
    return std::string();
}

}  // namespace TmwAthena
