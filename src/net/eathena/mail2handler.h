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

#ifndef NET_EATHENA_MAIL2HANDLER_H
#define NET_EATHENA_MAIL2HANDLER_H

#include "net/mail2handler.h"

namespace EAthena
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

        void removeItem(const Item *const item,
                        const int amount) const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_MAIL2HANDLER_H
