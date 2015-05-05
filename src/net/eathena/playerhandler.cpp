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

#include "being/localplayer.h"

#include "enums/being/attributes.h"

#include "gui/windows/statuswindow.h"

#include "input/inputmanager.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/inventoryhandler.h"

#include "utils/stringutils.h"

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

        case SMSG_PLAYER_SHORTCUTS:
            processPlayerShortcuts(msg);
            break;

        case SMSG_PLAYER_SHOW_EQUIP:
            processPlayerShowEquip(msg);
            break;

        case SMSG_PLAYER_GET_EXP:
            processPlayerGetExp(msg);
            break;

        case SMSG_PVP_INFO:
            processPvpInfo(msg);
            break;

        case SMSG_PLAYER_HEAL:
            processPlayerHeal(msg);
            break;

        case SMSG_PLAYER_SKILL_MESSAGE:
            processPlayerSkillMessage(msg);
            break;

        case SMSG_MAP_MASK:
            processMapMask(msg);
            break;

        case SMSG_MAP_MUSIC:
            processMapMusic(msg);
            break;

        case SMSG_ONLINE_LIST:
            processOnlineList(msg);
            break;

        case SMSG_PLAYER_NOTIFY_MAPINFO:
            processNotifyMapInfo(msg);
            break;

        case SMSG_PLAYER_FAME_BLACKSMITH:
            processPlayerFameBlacksmith(msg);
            break;

        case SMSG_PLAYER_FAME_ALCHEMIST:
            processPlayerFameAlchemist(msg);
            break;

        case SMSG_PLAYER_UPGRADE_MESSAGE:
            processPlayerUpgradeMessage(msg);
            break;

        case SMSG_PLAYER_FAME_TAEKWON:
            processPlayerFameTaekwon(msg);
            break;

        case SMSG_PLAYER_READ_BOOK:
            processPlayerReadBook(msg);
            break;

        case SMSG_PLAYER_EQUIP_TICK_ACK:
            processPlayerEquipTickAck(msg);
            break;

        case SMSG_AUTOSHADOW_SPELL_LIST:
            processPlayerAutoShadowSpellList(msg);
            break;

        case SMSG_PLAYER_RANK_POINTS:
            processPlayerRankPoints(msg);
            break;

        case SMSG_PLAYER_CLIENT_COMMAND:
            processPlayerClientCommand(msg);
            break;

        default:
            break;
    }
}

void PlayerHandler::attack(const int id, const Keep keep) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(id, "target id");
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

void PlayerHandler::increaseAttribute(const int attr) const
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
    outMsg.writeInt32(floorItem->getId(), "object id");
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

void PlayerHandler::changeAction(const BeingAction::Action &action) const
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

void PlayerHandler::processPlayerShortcuts(Net::MessageIn &msg)
{
    // +++ player shortcuts ignored. It also disabled on server side.
    // may be in future better use it?
    msg.readUInt8("unused?");
    for (int f = 0; f < 27; f ++)
    {
        msg.readUInt8("type 0: item, 1: skill");
        msg.readInt32("item or skill id");
        msg.readInt16("skill level");
    }
    msg.skip(77, "unused");
}

void PlayerHandler::processPlayerShowEquip(Net::MessageIn &msg)
{
    // +++ for now server allow only switch this option but not using it.
    msg.readUInt8("show equip");  // 1 mean need open "equipment" window
}

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::CHAR_POINTS,
        msg.readInt16("char points"));

    unsigned int val = msg.readUInt8("str");
    PlayerInfo::setStatBase(Attributes::STR, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::STR,
            msg.readUInt8("str cost"));
    }
    else
    {
        msg.readUInt8("str need");
    }

    val = msg.readUInt8("agi");
    PlayerInfo::setStatBase(Attributes::AGI, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::AGI,
            msg.readUInt8("agi cost"));
    }
    else
    {
        msg.readUInt8("agi cost");
    }

    val = msg.readUInt8("vit");
    PlayerInfo::setStatBase(Attributes::VIT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::VIT,
            msg.readUInt8("vit cost"));
    }
    else
    {
        msg.readUInt8("vit cost");
    }

    val = msg.readUInt8("int");
    PlayerInfo::setStatBase(Attributes::INT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::INT,
            msg.readUInt8("int cost"));
    }
    else
    {
        msg.readUInt8("int cost");
    }

    val = msg.readUInt8("dex");
    PlayerInfo::setStatBase(Attributes::DEX, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::DEX,
            msg.readUInt8("dex cost"));
    }
    else
    {
        msg.readUInt8("dex cost");
    }

    val = msg.readUInt8("luk");
    PlayerInfo::setStatBase(Attributes::LUK, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::LUK,
            msg.readUInt8("luk cost"));
    }
    else
    {
        msg.readUInt8("luk cost");
    }

    PlayerInfo::setStatBase(Attributes::ATK,
        msg.readInt16("left atk"), Notify_false);
    PlayerInfo::setStatMod(Attributes::ATK, msg.readInt16("right atk"));
    PlayerInfo::updateAttrs();

    val = msg.readInt16("right matk");
    PlayerInfo::setStatBase(Attributes::MATK, val, Notify_false);

    val = msg.readInt16("left matk");
    PlayerInfo::setStatMod(Attributes::MATK, val);

    PlayerInfo::setStatBase(Attributes::DEF,
        msg.readInt16("left def"), Notify_false);
    PlayerInfo::setStatMod(Attributes::DEF, msg.readInt16("right def"));

    PlayerInfo::setStatBase(Attributes::MDEF,
        msg.readInt16("left mdef"), Notify_false);
    PlayerInfo::setStatMod(Attributes::MDEF, msg.readInt16("right mdef"));

    PlayerInfo::setStatBase(Attributes::HIT, msg.readInt16("hit"));

    PlayerInfo::setStatBase(Attributes::FLEE,
        msg.readInt16("flee"), Notify_false);
    PlayerInfo::setStatMod(Attributes::FLEE, msg.readInt16("flee2/10"));

    PlayerInfo::setStatBase(Attributes::CRIT, msg.readInt16("crit/10"));

    PlayerInfo::setAttribute(Attributes::ATTACK_DELAY,
        msg.readInt16("attack speed"));
    msg.readInt16("plus speed = 0");

    BLOCK_END("PlayerHandler::processPlayerStatUpdate5")
}

void PlayerHandler::processPlayerGetExp(Net::MessageIn &msg)
{
    if (!localPlayer)
        return;
    const int id = msg.readInt32("player id");
    const int exp = msg.readInt32("exp amount");
    const int stat = msg.readInt16("exp type");
    const bool fromQuest = msg.readInt16("is from quest");
    if (!fromQuest && id == localPlayer->getId())
    {
        if (stat == 1)
            localPlayer->addXpMessage(exp);
        else if (stat == 2)
            localPlayer->addJobMessage(exp);
        else
            UNIMPLIMENTEDPACKET;
    }
    // need show particle depend on isQuest flag, for now ignored
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
    msg.readInt32("tick");
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("(sx<<4) | (sy&0x0f)");
    if (localPlayer)
        localPlayer->setRealPos(dstX, dstY);
    BLOCK_END("PlayerHandler::processWalkResponse")
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

void PlayerHandler::processPvpInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("account id");
    msg.readInt32("pvp won");
    msg.readInt32("pvp lost");
    msg.readInt32("pvp point");
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

void PlayerHandler::processPlayerHeal(Net::MessageIn &msg)
{
    if (!localPlayer)
        return;

    const int type = msg.readInt16("var id");
    const int amount = msg.readInt16("value");
    if (type == 5)
        localPlayer->addHpMessage(amount);
    else if (type == 7)
        localPlayer->addSpMessage(amount);
}

void PlayerHandler::processPlayerSkillMessage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ need show this message
    msg.readInt32("type");
}

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int base,
                            const int mod,
                            const Notify notify) const
{
    Ea::PlayerHandler::setStat(msg, type, base, mod, notify);
}

void PlayerHandler::processNotifyMapInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("type");
}

void PlayerHandler::processPlayerFameBlacksmith(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerHandler::processPlayerFameAlchemist(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerHandler::processPlayerUpgradeMessage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("result");
    msg.readInt16("item id");
}

void PlayerHandler::processPlayerFameTaekwon(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerHandler::processPlayerReadBook(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("book id");
    msg.readInt32("page");
}

void PlayerHandler::processPlayerEquipTickAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("unused");
    msg.readInt32("flag");
}

void PlayerHandler::processPlayerAutoShadowSpellList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 8) / 2;
    for (int f = 0; f < count; f ++)
        msg.readInt16("skill id");
}

void PlayerHandler::processPlayerRankPoints(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("type");
    msg.readInt32("points");
    msg.readInt32("fame");
}

void PlayerHandler::processPlayerClientCommand(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 4;
    std::string command = msg.readString(sz, "command");
    std::string cmd;
    std::string args;

    if (!parse2Str(command, cmd, args))
    {
        cmd = command;
        args.clear();
    }
    inputManager.executeChatCommand(cmd, args, nullptr);
}

}  // namespace EAthena
