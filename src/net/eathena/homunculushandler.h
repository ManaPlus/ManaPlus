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

#ifndef NET_EATHENA_HOMUNCULUSHANDLER_H
#define NET_EATHENA_HOMUNCULUSHANDLER_H

#include "net/homunculushandler.h"

namespace EAthena
{
class HomunculusHandler final : public Net::HomunculusHandler
{
    public:
        HomunculusHandler();

        A_DELETE_COPY(HomunculusHandler)

        ~HomunculusHandler() override final;

        void setName(const std::string &name) const override final;

        void moveToMaster() const override final;

        void move(const int x, const int y) const override final;

        void attack(const BeingId targetId,
                    const Keep keep) const override final;

        void feed() const override final;

        void fire() const override final;

        void talk(const std::string &restrict text) const override final;

        void emote(const uint8_t emoteId) const override final;

        void setDirection(const unsigned char type) const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_HOMUNCULUSHANDLER_H
