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

#ifndef NET_EATHENA_PLAYERHANDLER_H
#define NET_EATHENA_PLAYERHANDLER_H

#include "net/ea/playerhandler.h"

namespace EAthena
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
        void updateStatus(const uint8_t status) const final;

        void requestOnlineList() const final;
        void respawn() const final;
        void setShortcut(const int idx,
                         const uint8_t type,
                         const int id,
                         const int level) const final;
        void shortcutShiftRow(const int row) const final;
        void removeOption() const final;
        void changeCart(const int type) const final;
        void setMemo() const final;
        void doriDori() const final;
        void explosionSpirits() const final;
        void requestPvpInfo() const final;
        void revive() const final;
        void setViewEquipment(const bool allow) const final;

        void setStat(Net::MessageIn &msg,
                     const int type,
                     const int base,
                     const int mod,
                     const Notify notify) const final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_PLAYERHANDLER_H
