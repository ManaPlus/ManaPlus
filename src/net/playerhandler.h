/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "being.h"
#include "flooritem.h"
#include "playerinfo.h"

namespace Net
{

class PlayerHandler
{
    public:
        virtual ~PlayerHandler()
        { }

        virtual void attack(int id, bool keep = false) = 0;

        virtual void stopAttack() = 0;

        virtual void emote(uint8_t emoteId) = 0;

        virtual void increaseAttribute(int attr) = 0;

        virtual void decreaseAttribute(int attr) = 0;

        virtual void increaseSkill(unsigned short skillId) = 0;

        virtual void pickUp(const FloorItem *floorItem) = 0;

        virtual void setDirection(unsigned char direction) = 0;

        virtual void setDestination(int x, int y, int direction = -1) = 0;

        virtual void changeAction(Being::Action action) = 0;

        virtual void respawn() = 0;

        virtual void ignorePlayer(const std::string &player, bool ignore) = 0;

        virtual void ignoreAll(bool ignore) = 0;

        virtual bool canUseMagic() const = 0;

        virtual bool canCorrectAttributes() const = 0;

        virtual int getJobLocation() const A_WARN_UNUSED = 0;

        virtual int getAttackLocation() const A_WARN_UNUSED = 0;

        virtual Vector getDefaultWalkSpeed() const A_WARN_UNUSED = 0;

        virtual void requestOnlineList() = 0;

        virtual void updateStatus(uint8_t status) = 0;
};

} // namespace Net

#endif // PLAYERHANDLER_H
