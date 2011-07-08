/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_MANASERV_PLAYERHANDLER_H
#define NET_MANASERV_PLAYERHANDLER_H

#include "net/playerhandler.h"

#include "net/manaserv/messagehandler.h"

#include <guichan/actionlistener.hpp>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

namespace ManaServ
{

struct RespawnRequestListener : public gcn::ActionListener
{
    void action(const gcn::ActionEvent &event);
};

static RespawnRequestListener respawnListener;

class PlayerHandler : public MessageHandler, public Net::PlayerHandler
{
    public:
        PlayerHandler();

        void handleMessage(Net::MessageIn &msg);

        void attack(int id, bool keep = false);
        void stopAttack();
        void emote(Uint8 emoteId);

        void increaseAttribute(int attr);
        void decreaseAttribute(int attr);
        void increaseSkill(unsigned short skillId);

        void pickUp(FloorItem *floorItem);
        void setDirection(char direction);
        void setDestination(int x, int y, int direction = -1);
        void changeAction(Being::Action action);

        void respawn();

        void ignorePlayer(const std::string &player, bool ignore);
        void ignoreAll(bool ignore);

        bool canUseMagic() const;
        bool canCorrectAttributes() const;

        int getJobLocation() const;

        Vector getDefaultWalkSpeed() const;

    private:
        void handleMapChangeMessage(Net::MessageIn &msg);
};

} // namespace ManaServ

#endif // NET_MANASERV_PLAYERHANDLER_H
