/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef NET_PLAYERHANDLER_H
#define NET_PLAYERHANDLER_H

#include "being/flooritem.h"

#include "enums/being/attributes.h"
#include "enums/being/beingaction.h"

#include "enums/simpletypes/keep.h"
#include "enums/simpletypes/notify.h"

#include "net/messagein.h"

namespace Net
{

class PlayerHandler notfinal
{
    public:
        PlayerHandler()
        { }

        A_DELETE_COPY(PlayerHandler)

        virtual ~PlayerHandler()
        { }

        virtual void attack(const BeingId id,
                            const Keep keep) const = 0;

        virtual void stopAttack() const = 0;

        virtual void emote(const uint8_t emoteId) const = 0;

        virtual void increaseAttribute(const AttributesT attr,
                                       const int amount) const = 0;

        virtual void increaseSkill(const uint16_t skillId) const = 0;

        virtual void pickUp(const FloorItem *const floorItem) const = 0;

        virtual void setDirection(const unsigned char direction) const = 0;

        virtual void setDestination(const int x, const int y,
                                    const int direction) const = 0;

        virtual void changeAction(const BeingActionT &action) const = 0;

        virtual void respawn() const = 0;

        virtual void ignorePlayer(const std::string &player,
                                  const bool ignore) const = 0;

        virtual void ignoreAll(const bool ignore) const = 0;

        virtual bool canUseMagic() const = 0;

        virtual int getDefaultWalkSpeed() const A_WARN_UNUSED = 0;

        virtual void requestOnlineList() const = 0;

        virtual void updateStatus(const uint8_t status) const = 0;

        virtual void setShortcut(const int idx,
                                 const uint8_t type,
                                 const int id,
                                 const int level) const = 0;

        virtual void shortcutShiftRow(const int row) const = 0;

        virtual void removeOption() const = 0;

        virtual void changeCart(const int type) const = 0;

        virtual void setMemo() const = 0;

        virtual void doriDori() const = 0;

        virtual void explosionSpirits() const = 0;

        virtual void requestPvpInfo() const = 0;

        virtual void revive() const = 0;

        virtual void setViewEquipment(const bool allow) const = 0;

        virtual void setStat(Net::MessageIn &msg,
                             const int type,
                             const int64_t base,
                             const int mod,
                             const Notify notify) const = 0;
        virtual void selectStyle(const int headColor,
                                 const int headStyle,
                                 const int bodyColor,
                                 const int topStyle,
                                 const int middleStyle,
                                 const int bottomStyle) const = 0;
};

}  // namespace Net

extern Net::PlayerHandler *playerHandler;

#endif  // NET_PLAYERHANDLER_H
