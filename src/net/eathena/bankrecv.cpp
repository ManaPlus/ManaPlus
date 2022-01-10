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

#include "net/eathena/bankrecv.h"

#include "notifymanager.h"

#include "enums/resources/notifytypes.h"

#include "listeners/banklistener.h"

#include "net/messagein.h"

#include "utils/cast.h"

#include "debug.h"

namespace EAthena
{

void BankRecv::processBankStatus(Net::MessageIn &msg)
{
    const int money = CAST_S32(msg.readInt64("money"));
    const int reason = msg.readInt16("reason");
    BankListener::distributeEvent(money);
    if (reason != 0)
        NotifyManager::notify(NotifyTypes::BANK_CHECK_FAILED);
}

void BankRecv::processBankDeposit(Net::MessageIn &msg)
{
    const int reason = msg.readInt16("reason");
    const int money = CAST_S32(msg.readInt64("money"));
    msg.readInt32("balance");
    BankListener::distributeEvent(money);
    if (reason != 0)
        NotifyManager::notify(NotifyTypes::BANK_DEPOSIT_FAILED);
}

void BankRecv::processBankWithdraw(Net::MessageIn &msg)
{
    const int reason = msg.readInt16("reason");
    const int money = CAST_S32(msg.readInt64("money"));
    msg.readInt32("balance");
    BankListener::distributeEvent(money);
    if (reason != 0)
        NotifyManager::notify(NotifyTypes::BANK_WITHDRAW_FAILED);
}

}  // namespace EAthena
