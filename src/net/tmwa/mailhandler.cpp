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

#include "net/tmwa/mailhandler.h"

#include "debug.h"

extern Net::MailHandler *mailHandler;

namespace TmwAthena
{

MailHandler::MailHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        0
    };
    handledMessages = _messages;
    mailHandler = this;
}

void MailHandler::handleMessage(Net::MessageIn &msg A_UNUSED)
{
}

void MailHandler::refresh()
{
}

void MailHandler::readMessage(const int msgId A_UNUSED)
{
}

void MailHandler::getAttach(const int msgId A_UNUSED)
{
}

void MailHandler::deleteMessage(const int msgId A_UNUSED)
{
}

void MailHandler::returnMessage(const int msgId A_UNUSED)
{
}

void MailHandler::setAttach(const int index A_UNUSED,
                            const int amount A_UNUSED)
{
}

void MailHandler::resetAttach(const int flag A_UNUSED)
{
}

}  // namespace TmwAthena
