/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef NET_EATHENA_PLAYERHANDLER_H
#define NET_EATHENA_PLAYERHANDLER_H

#include "net/net.h"
#include "net/playerhandler.h"

#include "net/ea/playerhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

class PlayerHandler final : public MessageHandler, public Ea::PlayerHandler
{
    public:
        PlayerHandler();

        A_DELETE_COPY(PlayerHandler)

        void handleMessage(Net::MessageIn &msg);

        void attack(int id, bool keep = false);
        void stopAttack();
        void emote(uint8_t emoteId);

        void increaseAttribute(int attr);
        void increaseSkill(unsigned short skillId);

        void pickUp(const FloorItem *floorItem);
        void setDirection(unsigned char direction);
        void setDestination(int x, int y, int direction = -1);
        void changeAction(Being::Action action);
        void updateStatus(uint8_t status);

        void processPlayerShortcuts(Net::MessageIn &msg);
        void processPlayerShowEquip(Net::MessageIn &msg);

        void requestOnlineList();
        void respawn();
};

} // namespace EAthena

#endif // NET_EATHENA_PLAYERHANDLER_H
