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

#ifndef NET_TMWA_REFINEHANDLER_H
#define NET_TMWA_REFINEHANDLER_H

#include "net/refinehandler.h"

namespace TmwAthena
{
class RefineHandler final : public Net::RefineHandler
{
    public:
        RefineHandler();

        A_DELETE_COPY(RefineHandler)

        ~RefineHandler() override final;

        void addItem(const Item *const item) const override final;

        void refineItem(const Item *const item1,
                        const Item *const item2,
                        const int blessing) const override final;

        void close() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_REFINEHANDLER_H
