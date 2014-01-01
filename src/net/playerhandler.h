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

#ifndef NET_PLAYERHANDLER_H
#define NET_PLAYERHANDLER_H

#include "flooritem.h"

#include "being/being.h"
#include "being/playerinfo.h"

namespace Net
{

class PlayerHandler
{
    public:
        virtual ~PlayerHandler()
        { }

        virtual void attack(const int id, const bool keep) const = 0;

        virtual void stopAttack() const = 0;

        virtual void emote(const uint8_t emoteId) const = 0;

        virtual void increaseAttribute(const int attr) const = 0;

        virtual void decreaseAttribute(const int attr) const = 0;

        virtual void increaseSkill(const uint16_t skillId) const = 0;

        virtual void pickUp(const FloorItem *const floorItem) const = 0;

        virtual void setDirection(const unsigned char direction) const = 0;

        virtual void setDestination(const int x, const int y,
                                    const int direction) const = 0;

        virtual void changeAction(const Being::Action &action) const = 0;

        virtual void respawn() const = 0;

        virtual void ignorePlayer(const std::string &player,
                                  const bool ignore) const = 0;

        virtual void ignoreAll(const bool ignore) const = 0;

        virtual bool canUseMagic() const = 0;

        virtual bool canCorrectAttributes() const = 0;

        virtual int getJobLocation() const A_WARN_UNUSED = 0;

        virtual int getAttackLocation() const A_WARN_UNUSED = 0;

        virtual Vector getDefaultWalkSpeed() const A_WARN_UNUSED = 0;

        virtual void requestOnlineList() const = 0;

        virtual void updateStatus(const uint8_t status) const = 0;
};

}  // namespace Net

#endif  // NET_PLAYERHANDLER_H
