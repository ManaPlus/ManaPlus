/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/eathena/bankhandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern Net::BankHandler *bankHandler;
extern int packetVersion;

namespace EAthena
{

BankHandler::BankHandler()
{
    bankHandler = this;
}

void BankHandler::deposit(const int money) const
{
    if (packetVersion < 20130724)
        return;
    createOutPacket(CMSG_BANK_DEPOSIT);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::withdraw(const int money) const
{
    if (packetVersion < 20130724)
        return;
    createOutPacket(CMSG_BANK_WITHDRAW);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::check() const
{
    if (packetVersion < 20130724)
        return;
    createOutPacket(CMSG_BANK_CHECK);
    outMsg.writeInt32(0, "account id");
}

void BankHandler::open() const
{
    if (packetVersion < 20130724)
        return;
    createOutPacket(CMSG_BANK_OPEN);
    outMsg.writeInt32(0, "unused");
}

void BankHandler::close() const
{
    if (packetVersion < 20130724)
        return;
    createOutPacket(CMSG_BANK_CLOSE);
    outMsg.writeInt32(0, "unused");
}

}  // namespace EAthena
