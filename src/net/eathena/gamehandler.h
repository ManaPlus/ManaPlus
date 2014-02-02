/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef NET_EATHENA_GAMEHANDLER_H
#define NET_EATHENA_GAMEHANDLER_H

#include "net/eathena/messagehandler.h"

#include "net/ea/gamehandler.h"

namespace EAthena
{

class GameHandler final : public MessageHandler, public Ea::GameHandler
{
    public:
        GameHandler();

        A_DELETE_COPY(GameHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void connect() override final;

        bool isConnected() const override final A_WARN_UNUSED;

        void disconnect() override final;

        void quit() const override final;

        void ping(const int tick) const override final;

        void disconnect2() const override final;

        void mapLoadedEvent() const override final;

        static void processMapCharId(Net::MessageIn &msg);

        bool mustPing() const override final A_WARN_UNUSED
        { return true; }
};

}  // namespace EAthena

#endif  // NET_EATHENA_GAMEHANDLER_H
