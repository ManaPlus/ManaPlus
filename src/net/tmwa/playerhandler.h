/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_TMWA_PLAYERHANDLER_H
#define NET_TMWA_PLAYERHANDLER_H

#include "net/ea/playerhandler.h"

namespace TmwAthena
{

class PlayerHandler final : public Ea::PlayerHandler
{
    public:
        PlayerHandler();

        A_DELETE_COPY(PlayerHandler)

        void attack(const BeingId id,
                    const Keep keep) const final;
        void stopAttack() const final;
        void emote(const uint8_t emoteId) const final;

        void increaseAttribute(const AttributesT attr,
                               const int amount) const final;
        void increaseSkill(const uint16_t skillId) const final;

        void pickUp(const FloorItem *const floorItem) const final;
        void setDirection(const unsigned char direction) const final;
        void setDestination(const int x, const int y,
                            const int direction) const final;
        void changeAction(const BeingActionT &action)
                          const final;
        void requestOnlineList() const final A_CONST;
        void updateStatus(const uint8_t status) const final A_CONST;

        void respawn() const final;

        void setShortcut(const int idx,
                         const uint8_t type,
                         const int id,
                         const int level) const final A_CONST;

        void shortcutShiftRow(const int row) const final;

        void removeOption() const final A_CONST;

        void changeCart(const int type) const final A_CONST;

        void setMemo() const final A_CONST;

        void doriDori() const final A_CONST;

        void explosionSpirits() const final A_CONST;

        void requestPvpInfo() const final A_CONST;

        void revive() const final A_CONST;

        void setViewEquipment(const bool allow) const final A_CONST;

        void setStat(Net::MessageIn &msg,
                     const int type,
                     const int base,
                     const int mod,
                     const Notify notify) const final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_PLAYERHANDLER_H
