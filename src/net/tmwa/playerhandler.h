/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef NET_TMWA_PLAYERHANDLER_H
#define NET_TMWA_PLAYERHANDLER_H

#include "net/net.h"
#include "net/playerhandler.h"

#include "net/ea/playerhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class PlayerHandler final : public MessageHandler, public Ea::PlayerHandler
{
    public:
        PlayerHandler();

        A_DELETE_COPY(PlayerHandler)

        void handleMessage(Net::MessageIn &msg) override;

        void attack(const int id, const bool keep) const override;
        void stopAttack() const override;
        void emote(const uint8_t emoteId) const override;

        void increaseAttribute(const int attr) const override;
        void increaseSkill(const uint16_t skillId) const override;

        void pickUp(const FloorItem *const floorItem) const override;
        void setDirection(const unsigned char direction) const override;
        void setDestination(const int x, const int y,
                            const int direction) const override;
        void changeAction(const Being::Action &action) const override;
        void processOnlineList(Net::MessageIn &msg) const;
        void requestOnlineList() const override;
        void updateStatus(const uint8_t status) const override;

        void respawn() const override;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_PLAYERHANDLER_H
