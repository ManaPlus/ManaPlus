/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_TMWA_BANKHANDLER_H
#define NET_TMWA_BANKHANDLER_H

#include "net/bankhandler.h"

namespace TmwAthena
{

class BankHandler final : public Net::BankHandler
{
    public:
        BankHandler();

        A_DELETE_COPY(BankHandler)

        ~BankHandler() override final;

        void deposit(const int money) const override final;

        void withdraw(const int money) const override final;

        void check() const override final;

        void open() const override final;

        void close() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_BANKHANDLER_H
