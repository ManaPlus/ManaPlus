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

#include "net/tmwa/beinghandler.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"
#include "net/tmwa/sprite.h"

#include "debug.h"

extern Net::BeingHandler *beingHandler;

namespace TmwAthena
{

BeingHandler::BeingHandler(const bool enableSync) :
    Ea::BeingHandler(enableSync)
{
    beingHandler = this;
}

void BeingHandler::requestNameById(const BeingId id) const
{
    createOutPacket(CMSG_NAME_REQUEST);
    outMsg.writeBeingId(id, "being id");
}

void BeingHandler::undress(Being *const being) const
{
    if (!being)
        return;
    being->setSprite(SPRITE_WEAPON,
        0,
        std::string(),
        ItemColor_one,
        zeroCards);
    being->setSprite(SPRITE_HEAD_BOTTOM,
        0,
        std::string(),
        ItemColor_one,
        zeroCards);
    being->setSprite(SPRITE_CLOTHES_COLOR,
        0,
        std::string(),
        ItemColor_one,
        zeroCards);
    being->setSprite(SPRITE_HAIR,
        0,
        std::string(),
        ItemColor_one,
        zeroCards);
    being->setSprite(SPRITE_SHOES,
        0,
        std::string(),
        ItemColor_one,
        zeroCards);
}

#ifdef EATHENA_SUPPORT
void BeingHandler::requestRanks(const RankT rank A_UNUSED) const
{
}

void BeingHandler::viewPlayerEquipment(const Being *const being A_UNUSED) const
{
}
#endif

void BeingHandler::requestNameByCharId(const int id A_UNUSED) const
{
}

}  // namespace TmwAthena
