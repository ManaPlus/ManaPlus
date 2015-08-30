/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/eathena/playerhandler.h"

#include "configuration.h"
#include "game.h"
#include "notifymanager.h"

#include "being/beingflag.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/onlineplayer.h"

#include "gui/windows/statuswindow.h"
#include "gui/windows/whoisonline.h"

#include "input/inputmanager.h"

#include "net/ea/playerrecv.h"

#include "net/eathena/messageout.h"
#include "net/eathena/playerrecv.h"
#include "net/eathena/protocol.h"
#include "net/eathena/inventoryhandler.h"

#include "resources/map/map.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;

namespace EAthena
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
        SMSG_PLAYER_SHORTCUTS,
        SMSG_PLAYER_SHOW_EQUIP,
        SMSG_PLAYER_GET_EXP,
        SMSG_PVP_INFO,
        SMSG_PLAYER_HEAL,
        SMSG_PLAYER_SKILL_MESSAGE,
        SMSG_MAP_MASK,
        SMSG_MAP_MUSIC,
        SMSG_ONLINE_LIST,
        SMSG_PLAYER_NOTIFY_MAPINFO,
        SMSG_PLAYER_FAME_BLACKSMITH,
        SMSG_PLAYER_FAME_ALCHEMIST,
        SMSG_PLAYER_UPGRADE_MESSAGE,
        SMSG_PLAYER_FAME_TAEKWON,
        SMSG_PLAYER_READ_BOOK,
        SMSG_PLAYER_EQUIP_TICK_ACK,
        SMSG_AUTOSHADOW_SPELL_LIST,
        SMSG_PLAYER_RANK_POINTS,
        SMSG_PLAYER_CLIENT_COMMAND,
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
            PlayerRecv::processWalkResponse(msg);
            break;

        case SMSG_PLAYER_WARP:
            Ea::PlayerRecv::processPlayerWarp(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_1:
            Ea::PlayerRecv::processPlayerStatUpdate1(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_2:
            Ea::PlayerRecv::processPlayerStatUpdate2(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_3:  // Update a base attribute
            Ea::PlayerRecv::processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4:  // Attribute increase ack
            Ea::PlayerRecv::processPlayerStatUpdate4(msg);
            break;

        // Updates stats and status points
        case SMSG_PLAYER_STAT_UPDATE_5:
            PlayerRecv::processPlayerStatUpdate5(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_6:
            Ea::PlayerRecv::processPlayerStatUpdate6(msg);
            break;

        case SMSG_PLAYER_ARROW_MESSAGE:
            Ea::PlayerRecv::processPlayerArrowMessage(msg);
            break;

        case SMSG_PLAYER_SHORTCUTS:
            PlayerRecv::processPlayerShortcuts(msg);
            break;

        case SMSG_PLAYER_SHOW_EQUIP:
            PlayerRecv::processPlayerShowEquip(msg);
            break;

        case SMSG_PLAYER_GET_EXP:
            PlayerRecv::processPlayerGetExp(msg);
            break;

        case SMSG_PVP_INFO:
            PlayerRecv::processPvpInfo(msg);
            break;

        case SMSG_PLAYER_HEAL:
            PlayerRecv::processPlayerHeal(msg);
            break;

        case SMSG_PLAYER_SKILL_MESSAGE:
            PlayerRecv::processPlayerSkillMessage(msg);
            break;

        case SMSG_MAP_MASK:
            PlayerRecv::processMapMask(msg);
            break;

        case SMSG_MAP_MUSIC:
            Ea::PlayerRecv::processMapMusic(msg);
            break;

        case SMSG_ONLINE_LIST:
            PlayerRecv::processOnlineList(msg);
            break;

        case SMSG_PLAYER_NOTIFY_MAPINFO:
            PlayerRecv::processNotifyMapInfo(msg);
            break;

        case SMSG_PLAYER_FAME_BLACKSMITH:
            PlayerRecv::processPlayerFameBlacksmith(msg);
            break;

        case SMSG_PLAYER_FAME_ALCHEMIST:
            PlayerRecv::processPlayerFameAlchemist(msg);
            break;

        case SMSG_PLAYER_UPGRADE_MESSAGE:
            PlayerRecv::processPlayerUpgradeMessage(msg);
            break;

        case SMSG_PLAYER_FAME_TAEKWON:
            PlayerRecv::processPlayerFameTaekwon(msg);
            break;

        case SMSG_PLAYER_READ_BOOK:
            PlayerRecv::processPlayerReadBook(msg);
            break;

        case SMSG_PLAYER_EQUIP_TICK_ACK:
            PlayerRecv::processPlayerEquipTickAck(msg);
            break;

        case SMSG_AUTOSHADOW_SPELL_LIST:
            PlayerRecv::processPlayerAutoShadowSpellList(msg);
            break;

        case SMSG_PLAYER_RANK_POINTS:
            PlayerRecv::processPlayerRankPoints(msg);
            break;

        case SMSG_PLAYER_CLIENT_COMMAND:
            PlayerRecv::processPlayerClientCommand(msg);
            break;

        default:
            break;
    }
}

void PlayerHandler::attack(const BeingId id,
                           const Keep keep) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeBeingId(id, "target id");
    if (keep == Keep_true)
        outMsg.writeInt8(7, "action");
    else
        outMsg.writeInt8(0, "action");
}

void PlayerHandler::stopAttack() const
{
    createOutPacket(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(const uint8_t emoteId) const
{
    createOutPacket(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void PlayerHandler::increaseAttribute(const AttributesT attr) const
{
    if (attr >= Attributes::STR && attr <= Attributes::LUK)
    {
        createOutPacket(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<int16_t>(attr), "attribute id");
        outMsg.writeInt8(1, "increase");
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::SKILL_POINTS) <= 0)
        return;

    createOutPacket(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId, "skill id");
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (!floorItem)
        return;

    createOutPacket(CMSG_ITEM_PICKUP);
    outMsg.writeBeingId(floorItem->getId(), "object id");
    EAthena::InventoryHandler *const handler =
        static_cast<EAthena::InventoryHandler*>(inventoryHandler);
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt8(0, "head direction");
    outMsg.writeInt8(0, "unused");
    outMsg.writeInt8(MessageOut::toServerDirection(direction),
        "player direction");
}

void PlayerHandler::setDestination(const int x, const int y,
                                   const int direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        static_cast<unsigned char>(direction), "destination");
}

void PlayerHandler::changeAction(const BeingActionT &action) const
{
    unsigned char type;
    switch (action)
    {
        case BeingAction::SIT:
            type = 2;
            break;
        case BeingAction::STAND:
        case BeingAction::PRESTAND:
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

    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt8(type, "action");
}

void PlayerHandler::respawn() const
{
    createOutPacket(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0, "action");
}

void PlayerHandler::requestOnlineList() const
{
    createOutPacket(CMSG_ONLINE_LIST);
}

void PlayerHandler::updateStatus(const uint8_t status) const
{
    createOutPacket(CMSG_SET_STATUS);
    outMsg.writeInt8(status, "status");
    outMsg.writeInt8(0, "unused");
}

void PlayerHandler::setShortcut(const int idx,
                                const uint8_t type,
                                const int id,
                                const int level) const
{
    createOutPacket(CMSG_SET_SHORTCUTS);
    outMsg.writeInt16(static_cast<int16_t>(idx), "index");
    outMsg.writeInt8(static_cast<int8_t>(type), "type");
    outMsg.writeInt32(id, "id");
    outMsg.writeInt16(static_cast<int16_t>(level), "level");
}

void PlayerHandler::removeOption() const
{
    createOutPacket(CMSG_REMOVE_OPTION);
}

void PlayerHandler::changeCart(const int type) const
{
    createOutPacket(CMSG_CHANGE_CART);
    outMsg.writeInt16(static_cast<int16_t>(type), "type");
}

void PlayerHandler::setMemo() const
{
    createOutPacket(CMSG_PLAYER_SET_MEMO);
}

void PlayerHandler::doriDori() const
{
    createOutPacket(CMSG_DORI_DORI);
}

void PlayerHandler::explosionSpirits() const
{
    createOutPacket(CMSG_EXPLOSION_SPIRITS);
}

void PlayerHandler::requestPvpInfo() const
{
    createOutPacket(CMSG_PVP_INFO);
    outMsg.writeInt32(0, "char id");
    outMsg.writeInt32(0, "account id");
}

void PlayerHandler::revive() const
{
    createOutPacket(CMSG_PLAYER_AUTO_REVIVE);
}

void PlayerHandler::setViewEquipment(const bool allow) const
{
    createOutPacket(CMSG_PLAYER_SET_EQUIPMENT_VISIBLE);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(allow ? 1 : 0, "allow");
}

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int base,
                            const int mod,
                            const Notify notify) const
{
    Ea::PlayerHandler::setStat(msg, type, base, mod, notify);
}

}  // namespace EAthena
