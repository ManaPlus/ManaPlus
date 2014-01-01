/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef NET_TMWA_PETHANDLER_H
#define NET_TMWA_PETHANDLER_H

#include "net/tmwa/messagehandler.h"

#include "net/pethandler.h"

namespace TmwAthena
{

class PetHandler final :  public MessageHandler, public Net::PetHandler
{
    public:
        PetHandler();

        A_DELETE_COPY(PetHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void move(const Being *const being,
                  const int x1, const int y1,
                  const int x2, const int y2) const override final;

        void spawn(const Being *const being,
                   const int x, const int y) const override final;

        void emote(const uint8_t emoteId) const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_PETHANDLER_H
