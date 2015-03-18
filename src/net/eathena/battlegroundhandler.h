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

#ifndef NET_EATHENA_BATTLEGROUNDHANDLER_H
#define NET_EATHENA_BATTLEGROUNDHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/battlegroundhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class BattleGroundHandler final : public MessageHandler,
                                  public Net::BattleGroundHandler
{
    public:
        BattleGroundHandler();

        A_DELETE_COPY(BattleGroundHandler)

        void handleMessage(Net::MessageIn &msg) override final;

    protected:
        void processBattleEmblem(Net::MessageIn &msg);

        void processBattleUpdateScore(Net::MessageIn &msg);

        void processBattleUpdateCoords(Net::MessageIn &msg);

        void processBattlePlay(Net::MessageIn &msg);

        void processBattleQueueAck(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_BATTLEGROUNDHANDLER_H
