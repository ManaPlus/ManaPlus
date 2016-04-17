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

#ifndef NET_TMWA_HOMUNCULUSHANDLER_H
#define NET_TMWA_HOMUNCULUSHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/homunculushandler.h"

namespace TmwAthena
{

class HomunculusHandler final : public Net::HomunculusHandler
{
    public:
        HomunculusHandler();

        A_DELETE_COPY(HomunculusHandler)

        void setName(const std::string &name) const final A_CONST;

        void moveToMaster() const final A_CONST;

        void move(const int x, const int y) const final A_CONST;

        void attack(const BeingId targetId,
                    const Keep keep) const final A_CONST;

        void feed() const final A_CONST;

        void fire() const final A_CONST;

        void talk(const std::string &restrict text) const final
                  A_CONST;

        void emote(const uint8_t emoteId) const final A_CONST;

        void setDirection(const unsigned char type) const final
                          A_CONST;
};

}  // namespace TmwAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_TMWA_HOMUNCULUSHANDLER_H
