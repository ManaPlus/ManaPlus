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

#ifndef NET_TA_PLAYERHANDLER_H
#define NET_TA_PLAYERHANDLER_H

#include "net/net.h"
#include "net/playerhandler.h"

#include "net/ea/playerhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class PlayerHandler : public MessageHandler, public Ea::PlayerHandler
{
    public:
        PlayerHandler();

        void handleMessage(Net::MessageIn &msg);

        void attack(int id, bool keep = false);
        void stopAttack();
        void emote(Uint8 emoteId);

        void increaseAttribute(int attr);
        void increaseSkill(unsigned short skillId);

        void pickUp(FloorItem *floorItem);
        void setDirection(char direction);
        void setDestination(int x, int y, int direction = -1);
        void changeAction(Being::Action action);
        void processOnlineList(Net::MessageIn &msg);
        void requestOnlineList();

        void respawn();
};

} // namespace TmwAthena

#endif // NET_TA_PLAYERHANDLER_H
