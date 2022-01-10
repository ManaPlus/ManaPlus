/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "net/tmwa/mailhandler.h"

#include "debug.h"

namespace TmwAthena
{

MailHandler::MailHandler()
{
    mailHandler = this;
}

MailHandler::~MailHandler()
{
    mailHandler = nullptr;
}

void MailHandler::refresh() const
{
}

void MailHandler::readMessage(const int msgId A_UNUSED) const
{
}

void MailHandler::getAttach(const int msgId A_UNUSED) const
{
}

void MailHandler::deleteMessage(const int msgId A_UNUSED) const
{
}

void MailHandler::returnMessage(const int msgId A_UNUSED) const
{
}

void MailHandler::setAttach(const int index A_UNUSED,
                            const int amount A_UNUSED) const
{
}

void MailHandler::setAttachMoney(const int money A_UNUSED) const
{
}

void MailHandler::resetAttach(const int flag A_UNUSED) const
{
}

void MailHandler::send(const std::string &name A_UNUSED,
                       const std::string &title A_UNUSED,
                       std::string message A_UNUSED) const
{
}

}  // namespace TmwAthena
