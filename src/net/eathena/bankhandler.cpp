/*
 *  The ManaPlus Client
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

#include "net/eathena/bankhandler.h"

#include "notifymanager.h"

#include "enums/resources/notifytypes.h"

#include "listeners/banklistener.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::BankHandler *bankHandler;

namespace EAthena
{

BankHandler::BankHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_BANK_STATUS,
        SMSG_BANK_DEPOSIT,
        SMSG_BANK_WITHDRAW,
        0
    };
    handledMessages = _messages;
    bankHandler = this;
}

void BankHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_BANK_STATUS:
            processBankStatus(msg);
            break;

        case SMSG_BANK_DEPOSIT:
            processBankDeposit(msg);
            break;

        case SMSG_BANK_WITHDRAW:
            processBankWithdraw(msg);
            break;

        default:
            break;
    }
}

void BankHandler::deposit(const int money) const
{
    createOutPacket(CMSG_BANK_DEPOSIT);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::withdraw(const int money) const
{
    createOutPacket(CMSG_BANK_WITHDRAW);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::check() const
{
    createOutPacket(CMSG_BANK_CHECK);
    outMsg.writeInt32(0, "account id");
}

void BankHandler::processBankStatus(Net::MessageIn &msg)
{
    const int money = static_cast<int>(msg.readInt64("money"));
    msg.readInt16("reason");
    BankListener::distributeEvent(money);
}

void BankHandler::processBankDeposit(Net::MessageIn &msg)
{
    const int reason = msg.readInt16("reason");
    const int money = static_cast<int>(msg.readInt64("money"));
    msg.readInt32("balance");
    BankListener::distributeEvent(money);
    if (reason)
        NotifyManager::notify(NotifyTypes::BANK_DEPOSIT_FAILED);
}

void BankHandler::processBankWithdraw(Net::MessageIn &msg)
{
    const int reason = msg.readInt16("reason");
    const int money = static_cast<int>(msg.readInt64("money"));
    msg.readInt32("balance");
    BankListener::distributeEvent(money);
    if (reason)
        NotifyManager::notify(NotifyTypes::BANK_WITHDRAW_FAILED);
}

void BankHandler::open() const
{
    createOutPacket(CMSG_BANK_OPEN);
    outMsg.writeInt32(0, "unused");
}

void BankHandler::close() const
{
    createOutPacket(CMSG_BANK_CLOSE);
    outMsg.writeInt32(0, "unused");
}

}  // namespace EAthena
