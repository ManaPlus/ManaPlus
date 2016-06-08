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

#include "net/eathena/beinghandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"
#include "net/eathena/sprite.h"

#include "debug.h"

extern Net::BeingHandler *beingHandler;
extern int serverVersion;

namespace EAthena
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
        IsWeapon_false);
    being->setSprite(SPRITE_HEAD_BOTTOM,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_HEAD_TOP,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_HEAD_MID,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_CLOTHES_COLOR,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_SHIELD,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_FLOOR,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_ROBE,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_EVOL2,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_EVOL3,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_EVOL4,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_EVOL5,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_EVOL6,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_HAIR,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
    being->setSprite(SPRITE_SHOES,
        0,
        std::string(),
        ItemColor_one,
        IsWeapon_false);
}

void BeingHandler::requestRanks(const RankT rank) const
{
    createOutPacket(CMSG_REQUEST_RANKS);
    outMsg.writeInt16(CAST_S16(rank), "type");
}

void BeingHandler::viewPlayerEquipment(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_PLAYER_VIEW_EQUIPMENT);
    outMsg.writeBeingId(being->getId(), "account id");
}

void BeingHandler::requestNameByCharId(const int id) const
{
    createOutPacket(CMSG_SOLVE_CHAR_NAME);
    outMsg.writeInt32(id, "character id");
}

}  // namespace EAthena
