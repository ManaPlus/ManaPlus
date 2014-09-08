/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/tmwa/playerhandler.h"

#include "configuration.h"
#include "game.h"
#include "soundmanager.h"

#include "being/attributes.h"
#include "being/beingflag.h"
#include "being/localplayer.h"

#include "gui/windows/statuswindow.h"

#include "net/net.h"

#include "net/tmwa/attrs.h"
#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "gui/windows/whoisonline.h"

#include "gui/onlineplayer.h"
#include "gui/viewport.h"

#include "resources/map/map.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;
extern int serverVersion;

namespace TmwAthena
{

PlayerHandler::PlayerHandler() :
    MessageHandler(),
    Ea::PlayerHandler()
{
    static const uint16_t _messages[] =
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
        SMSG_MAP_MASK,
        SMSG_MAP_MUSIC,
        0
    };
    handledMessages = _messages;
    playerHandler = this;
}

void PlayerHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::handleMessage")
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

        case SMSG_PLAYER_STAT_UPDATE_3:  // Update a base attribute
            processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4:  // Attribute increase ack
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
            break;

        case SMSG_MAP_MASK:
            processMapMask(msg);
            break;

        case SMSG_MAP_MUSIC:
            processMapMusic(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PlayerHandler::handleMessage")
}

void PlayerHandler::attack(const int id, const bool keep) const
{
    MessageOut outMsg(CMSG_PLAYER_ATTACK);
    outMsg.writeInt32(id);
    if (keep)
        outMsg.writeInt8(7);
    else
        outMsg.writeInt8(0);
}

void PlayerHandler::stopAttack() const
{
    MessageOut outMsg(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(const uint8_t emoteId) const
{
    MessageOut outMsg(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId);
}

void PlayerHandler::increaseAttribute(const int attr) const
{
    if (attr >= STR && attr <= LUK)
    {
        MessageOut outMsg(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<int16_t>(attr));
        outMsg.writeInt8(1);
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::SKILL_POINTS) <= 0)
        return;

    MessageOut outMsg(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId);
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (!floorItem)
        return;

    MessageOut outMsg(CMSG_ITEM_PICKUP);
    outMsg.writeInt32(floorItem->getId());
    TmwAthena::InventoryHandler *const handler =
        static_cast<TmwAthena::InventoryHandler*>(Net::getInventoryHandler());
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0);
    outMsg.writeInt8(direction);
}

void PlayerHandler::setDestination(const int x, const int y,
                                   const int direction) const
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        static_cast<unsigned char>(direction));
}

void PlayerHandler::changeAction(const BeingAction::Action &action) const
{
    char type;
    switch (action)
    {
        case BeingAction::SIT:
            type = 2;
            break;
        case BeingAction::STAND:
            type = 3;
            break;
        default:
        case BeingAction::MOVE:
        case BeingAction::ATTACK:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
            return;
    }

    MessageOut outMsg(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0);
    outMsg.writeInt8(type);
}

void PlayerHandler::respawn() const
{
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0);
}

void PlayerHandler::requestOnlineList() const
{
    MessageOut outMsg(CMSG_ONLINE_LIST);
}

void PlayerHandler::processOnlineList(Net::MessageIn &msg)
{
    if (!whoIsOnline)
        return;

    BLOCK_START("PlayerHandler::processOnlineList")
    const int size = msg.readInt16() - 4;
    std::vector<OnlinePlayer*> arr;

    if (!size)
    {
        if (whoIsOnline)
            whoIsOnline->loadList(arr);
        BLOCK_END("PlayerHandler::processOnlineList")
        return;
    }

    char *const start = reinterpret_cast<char*>(msg.readBytes(size));
    if (!start)
    {
        BLOCK_END("PlayerHandler::processOnlineList")
        return;
    }

    const char *buf = start;

    int addVal = 1;
    if (serverVersion >= 4)
        addVal = 3;

    while (buf - start + 1 < size
           && *(buf + static_cast<size_t>(addVal)))
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

        unsigned char gender = Gender::UNSPECIFIED;
        if (serverVersion >= 4)
        {
            if (config.getBoolValue("showgender"))
            {
                if (status & BeingFlag::GENDER_MALE)
                    gender = Gender::MALE;
                else if (status & BeingFlag::GENDER_OTHER)
                    gender = Gender::OTHER;
                else
                    gender = Gender::FEMALE;
            }
        }
        arr.push_back(new OnlinePlayer(static_cast<const char*>(buf),
            status, level, gender, ver));
        buf += strlen(buf) + 1;
    }

    if (whoIsOnline)
        whoIsOnline->loadList(arr);
    delete [] start;
    BLOCK_END("PlayerHandler::processOnlineList")
}

void PlayerHandler::updateStatus(const uint8_t status) const
{
    MessageOut outMsg(CMSG_SET_STATUS);
    outMsg.writeInt8(status);
    outMsg.writeInt8(0);
}

void PlayerHandler::processMapMask(Net::MessageIn &msg)
{
    const int mask = msg.readInt32();
    msg.readInt32();  // unused
    Map *const map = Game::instance()->getCurrentMap();
    if (map)
        map->setMask(mask);
}

void PlayerHandler::processMapMusic(Net::MessageIn &msg)
{
    const int size = msg.readInt16() - 5;
    const std::string music = msg.readString(size);
    soundManager.playMusic(music);

    Map *const map = viewport->getMap();
    if (map)
        map->setMusicFile(music);
}

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::CHAR_POINTS, msg.readInt16());

    unsigned int val = msg.readUInt8();
    PlayerInfo::setStatBase(STR, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(STR, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(AGI, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(AGI, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(VIT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(VIT, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(INT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(INT, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(DEX, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(DEX, msg.readUInt8());
    else
        msg.readUInt8();

    val = msg.readUInt8();
    PlayerInfo::setStatBase(LUK, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(LUK, msg.readUInt8());
    else
        msg.readUInt8();

    PlayerInfo::setStatBase(ATK, msg.readInt16(), false);
    PlayerInfo::setStatMod(ATK, msg.readInt16());
    PlayerInfo::updateAttrs();

    val = msg.readInt16();
    PlayerInfo::setStatBase(MATK, val, false);

    val = msg.readInt16();
    PlayerInfo::setStatMod(MATK, val);

    PlayerInfo::setStatBase(DEF, msg.readInt16(), false);
    PlayerInfo::setStatMod(DEF, msg.readInt16());

    PlayerInfo::setStatBase(MDEF, msg.readInt16(), false);
    PlayerInfo::setStatMod(MDEF, msg.readInt16());

    PlayerInfo::setStatBase(HIT, msg.readInt16());

    PlayerInfo::setStatBase(FLEE, msg.readInt16(), false);
    PlayerInfo::setStatMod(FLEE, msg.readInt16());

    PlayerInfo::setStatBase(CRIT, msg.readInt16());

    msg.readInt16();  // manner
    BLOCK_END("PlayerHandler::processPlayerStatUpdate5")
}

void PlayerHandler::processWalkResponse(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processWalkResponse")
    /*
      * This client assumes that all walk messages succeed,
      * and that the server will send a correction notice
      * otherwise.
      */
    uint16_t srcX, srcY, dstX, dstY;
    msg.readInt32();  // tick
    msg.readCoordinatePair(srcX, srcY, dstX, dstY);
    if (localPlayer)
        localPlayer->setRealPos(dstX, dstY);
    BLOCK_END("PlayerHandler::processWalkResponse")
}

void PlayerHandler::setShortcut(const int idx A_UNUSED,
                                const uint8_t type A_UNUSED,
                                const int id A_UNUSED,
                                const int level A_UNUSED) const
{
}

}  // namespace TmwAthena
