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

#include "net/tmwa/playerhandler.h"

#include "configuration.h"
#include "client.h"
#include "logger.h"

#include "net/messagein.h"

#include "net/tmwa/protocol.h"
#include "net/tmwa/npchandler.h"
#include "net/tmwa/inventoryhandler.h"

#include "gui/whoisonline.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;

namespace TmwAthena
{

PlayerHandler::PlayerHandler()
{
    static const Uint16 _messages[] =
    {
        SMSG_WALK_RESPONSE,
        SMSG_PLAYER_WARP,
        SMSG_PLAYER_STAT_UPDATE_1,
        SMSG_PLAYER_STAT_UPDATE_2,
        SMSG_PLAYER_STAT_UPDATE_3,
        SMSG_PLAYER_STAT_UPDATE_4,
        SMSG_PLAYER_STAT_UPDATE_5,
        SMSG_PLAYER_STAT_UPDATE_6,
        SMSG_PLAYER_ARROW_MESSAGE,
        SMSG_ONLINE_LIST,
        0
    };
    handledMessages = _messages;
    playerHandler = this;
}

void PlayerHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_WALK_RESPONSE:
            processWalkResponse(msg);
            break;

        case SMSG_PLAYER_WARP:
            processPlayerWarp(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_1:
            processPlayerStatUpdate1(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_2:
            processPlayerStatUpdate2(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_3: // Update a base attribute
            processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4: // Attribute increase ack
            processPlayerStatUpdate4(msg);
            break;

        // Updates stats and status points
        case SMSG_PLAYER_STAT_UPDATE_5:
            processPlayerStatUpdate5(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_6:
            processPlayerStatUpdate6(msg);
            break;

        case SMSG_PLAYER_ARROW_MESSAGE:
            processPlayerArrowMessage(msg);
            break;

        case SMSG_ONLINE_LIST:
            processOnlineList(msg);

        default:
            break;
    }
}

void PlayerHandler::attack(int id, bool keep)
{
    MessageOut outMsg(CMSG_PLAYER_ATTACK);
    outMsg.writeInt32(id);
    if (keep)
        outMsg.writeInt8(7);
    else
        outMsg.writeInt8(0);
}

void PlayerHandler::stopAttack()
{
    MessageOut outMsg(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(Uint8 emoteId)
{
    MessageOut outMsg(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId);
}

void PlayerHandler::increaseAttribute(int attr)
{
    if (attr >= STR && attr <= LUK)
    {
        MessageOut outMsg(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<Sint16>(attr));
        outMsg.writeInt8(1);
    }
}

void PlayerHandler::increaseSkill(unsigned short skillId)
{
    if (PlayerInfo::getAttribute(SKILL_POINTS) <= 0)
        return;

    MessageOut outMsg(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId);
}

void PlayerHandler::pickUp(FloorItem *floorItem)
{
    if (!floorItem)
        return;

    MessageOut outMsg(CMSG_ITEM_PICKUP);
    outMsg.writeInt32(floorItem->getId());
    TmwAthena::InventoryHandler *handler =
        static_cast<TmwAthena::InventoryHandler*>(Net::getInventoryHandler());
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(char direction)
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0);
    outMsg.writeInt8(direction);
}

void PlayerHandler::setDestination(int x, int y, int direction)
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<short unsigned int>(x),
        static_cast<short unsigned int>(y),
        static_cast<unsigned char>(direction));
}

void PlayerHandler::changeAction(Being::Action action)
{
    char type;
    switch (action)
    {
        case Being::SIT:
            type = 2;
            break;
        case Being::STAND:
            type = 3;
            break;
        default:
        case Being::MOVE:
        case Being::ATTACK:
        case Being::DEAD:
        case Being::HURT:
        case Being::SPAWN:
            return;
    }

    MessageOut outMsg(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0);
    outMsg.writeInt8(type);
}

void PlayerHandler::respawn()
{
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0);
}

void PlayerHandler::requestOnlineList()
{
    MessageOut outMsg(CMSG_ONLINE_LIST);
}

void PlayerHandler::processOnlineList(Net::MessageIn &msg)
{
    if (!whoIsOnline)
        return;

    int size = msg.readInt16() - 4;
    std::vector<OnlinePlayer*> arr;

    if (!size)
    {
        if (whoIsOnline)
            whoIsOnline->loadList(arr);
        return;
    }

    char *start = reinterpret_cast<char*>(msg.readBytes(size));
    if (!start)
        return;

    char *buf = start;

    int addVal = 1;
    if (serverVersion >= 4)
        addVal = 3;

    while (buf - start + 1 < size && *(buf + addVal))
    {
        unsigned char status = 255;
        unsigned char ver = 0;
        unsigned char level = 0;
        if (serverVersion >= 4)
        {
            status = *buf;
            buf ++;
            level = *buf;
            buf ++;
            ver = *buf;
        }
        buf ++;

        int gender = GENDER_UNSPECIFIED;
        if (serverVersion >= 4)
        {
            if (config.getBoolValue("showgender"))
            {
                if (status & Being::FLAG_GENDER_MALE)
                    gender = GENDER_MALE;
                else if (status & Being::FLAG_GENDER_OTHER)
                    gender = GENDER_OTHER;
                else
                    gender = GENDER_FEMALE;
            }
        }
        arr.push_back(new OnlinePlayer(static_cast<char*>(buf),
            status, level, gender, ver));
        buf += strlen(buf) + 1;
    }

    if (whoIsOnline)
        whoIsOnline->loadList(arr);
    delete [] start;
}

void PlayerHandler::updateStatus(Uint8 status)
{
    MessageOut outMsg(CMSG_SET_STATUS);
    outMsg.writeInt8(status);
    outMsg.writeInt8(0);
}

} // namespace TmwAthena
