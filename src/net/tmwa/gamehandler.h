/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_TMWA_GAMEHANDLER_H
#define NET_TMWA_GAMEHANDLER_H

#include "net/ea/gamehandler.h"

namespace TmwAthena
{

class GameHandler final : public Ea::GameHandler
{
    public:
        GameHandler();

        A_DELETE_COPY(GameHandler)

        ~GameHandler() override final;

        void connect() const override final;

        bool isConnected() const override final A_WARN_UNUSED;

        void disconnect() const override final;

        void quit() const override final;

        void ping(const int tick) const override final;

        void disconnect2() const override final;

        void mapLoadedEvent() const override final;

        void reqRemainTime() const override final;

        bool mustPing() const override final A_WARN_UNUSED
        { return false; }

        void ping2() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_GAMEHANDLER_H
