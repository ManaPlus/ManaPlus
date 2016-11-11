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

#include "net/eathena/playerrecv.h"

#include "actormanager.h"
#include "configuration.h"
#include "notifymanager.h"
#include "party.h"

#include "being/beingflag.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/onlineplayer.h"

#include "gui/windows/statuswindow.h"
#include "gui/windows/whoisonline.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "net/eathena/sp.h"
#include "net/messagein.h"

#include "utils/gettext.h"

#include "debug.h"

namespace EAthena
{

void PlayerRecv::processPlayerShortcuts(Net::MessageIn &msg)
{
    // +++ player shortcuts ignored. It also disabled on server side.
    // may be in future better use it?
    if (msg.getVersion() >= 20141022)
        msg.readUInt8("rotate");
    for (int f = 0; f < 27; f ++)
    {
        msg.readUInt8("type 0: item, 1: skill");
        msg.readInt32("item or skill id");
        msg.readInt16("skill level");
    }
    msg.skip(77, "unused");
}

void PlayerRecv::processPlayerShowEquip(Net::MessageIn &msg)
{
    // +++ for now server allow only switch this option but not using it.
    msg.readUInt8("show equip");  // 1 mean need open "equipment" window
}

void PlayerRecv::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::PLAYER_CHAR_POINTS,
        msg.readInt16("char points"));

    unsigned int val = msg.readUInt8("str");
    PlayerInfo::setStatBase(Attributes::PLAYER_STR, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_STR,
            msg.readUInt8("str cost"));
    }
    else
    {
        msg.readUInt8("str need");
    }

    val = msg.readUInt8("agi");
    PlayerInfo::setStatBase(Attributes::PLAYER_AGI, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_AGI,
            msg.readUInt8("agi cost"));
    }
    else
    {
        msg.readUInt8("agi cost");
    }

    val = msg.readUInt8("vit");
    PlayerInfo::setStatBase(Attributes::PLAYER_VIT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_VIT,
            msg.readUInt8("vit cost"));
    }
    else
    {
        msg.readUInt8("vit cost");
    }

    val = msg.readUInt8("int");
    PlayerInfo::setStatBase(Attributes::PLAYER_INT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_INT,
            msg.readUInt8("int cost"));
    }
    else
    {
        msg.readUInt8("int cost");
    }

    val = msg.readUInt8("dex");
    PlayerInfo::setStatBase(Attributes::PLAYER_DEX, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_DEX,
            msg.readUInt8("dex cost"));
    }
    else
    {
        msg.readUInt8("dex cost");
    }

    val = msg.readUInt8("luk");
    PlayerInfo::setStatBase(Attributes::PLAYER_LUK, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_LUK,
            msg.readUInt8("luk cost"));
    }
    else
    {
        msg.readUInt8("luk cost");
    }

    PlayerInfo::setStatBase(Attributes::PLAYER_ATK,
        msg.readInt16("left atk"), Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_ATK,
        msg.readInt16("right atk"));
    PlayerInfo::updateAttrs();

    val = msg.readInt16("right matk");
    PlayerInfo::setStatBase(Attributes::PLAYER_MATK, val, Notify_false);

    val = msg.readInt16("left matk");
    PlayerInfo::setStatMod(Attributes::PLAYER_MATK, val);

    PlayerInfo::setStatBase(Attributes::PLAYER_DEF,
        msg.readInt16("left def"), Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_DEF,
        msg.readInt16("right def"));

    PlayerInfo::setStatBase(Attributes::PLAYER_MDEF,
        msg.readInt16("left mdef"), Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_MDEF,
        msg.readInt16("right mdef"));

    PlayerInfo::setStatBase(Attributes::PLAYER_HIT,
        msg.readInt16("hit"));

    PlayerInfo::setStatBase(Attributes::PLAYER_FLEE,
        msg.readInt16("flee"), Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_FLEE,
        msg.readInt16("flee2/10"));

    PlayerInfo::setStatBase(Attributes::PLAYER_CRIT,
        msg.readInt16("crit/10"));

    PlayerInfo::setAttribute(Attributes::PLAYER_ATTACK_DELAY,
        msg.readInt16("attack speed"));
    msg.readInt16("plus speed = 0");

    BLOCK_END("PlayerRecv::processPlayerStatUpdate5")
}

void PlayerRecv::processPlayerGetExp(Net::MessageIn &msg)
{
    if (!localPlayer)
        return;
    const BeingId id = msg.readBeingId("player id");
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
            UNIMPLEMENTEDPACKETFIELD(stat);
    }
    // need show particle depend on isQuest flag, for now ignored
}

void PlayerRecv::processWalkResponse(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processWalkResponse")
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
    BLOCK_END("PlayerRecv::processWalkResponse")
}

void PlayerRecv::processWalkError(Net::MessageIn &msg)
{
    msg.readInt32("tick");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    if (localPlayer)
        localPlayer->failMove(x, y);
}

void PlayerRecv::processPvpInfo(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("char id");
    msg.readBeingId("account id");
    msg.readInt32("pvp won");
    msg.readInt32("pvp lost");
    msg.readInt32("pvp point");
}

void PlayerRecv::processPlayerHeal(Net::MessageIn &msg)
{
    if (!localPlayer)
        return;

    const int type = msg.readInt16("var id");
    int amount;
    if (msg.getVersion() >= 20150513)
        amount = msg.readInt32("value");
    else
        amount = msg.readInt16("value");
    if (type == Sp::HP)
    {
        const int base = PlayerInfo::getAttribute(Attributes::PLAYER_HP) +
            amount;
        PlayerInfo::setAttribute(Attributes::PLAYER_HP, base);
        if (localPlayer->isInParty() && Party::getParty(1))
        {
            PartyMember *const m = Party::getParty(1)
                ->getMember(localPlayer->getId());
            if (m)
            {
                m->setHp(base);
                m->setMaxHp(PlayerInfo::getAttribute(
                    Attributes::PLAYER_MAX_HP));
            }
        }
        localPlayer->addHpMessage(amount);
    }
    else if (type == Sp::SP)
    {
        localPlayer->addSpMessage(amount);
    }
}

void PlayerRecv::processPlayerSkillMessage(Net::MessageIn &msg)
{
    const int message = msg.readInt32("type");
    switch (message)
    {
        case 0x15:
            NotifyManager::notify(NotifyTypes::SKILL_END_ALL_NEGATIVE_STATUS);
            break;
        case 0x16:
            NotifyManager::notify(NotifyTypes::SKILL_IMMUNITY_TO_ALL_STATUSES);
            break;
        case 0x17:
            NotifyManager::notify(NotifyTypes::SKILL_MAX_HP);
            break;
        case 0x18:
            NotifyManager::notify(NotifyTypes::SKILL_MAX_SP);
            break;
        case 0x19:
            NotifyManager::notify(NotifyTypes::SKILL_ALL_STATUS_PLUS_20);
            break;
        case 0x1c:
            NotifyManager::notify(NotifyTypes::SKILL_ENCHANT_WEAPON_HOLY);
            break;
        case 0x1d:
            NotifyManager::notify(NotifyTypes::SKILL_ENCHANT_ARMOR_HOLY);
            break;
        case 0x1e:
            NotifyManager::notify(NotifyTypes::SKILL_DEF_PLUS_25);
            break;
        case 0x1f:
            NotifyManager::notify(NotifyTypes::SKILL_ATTACK_PLUS_100);
            break;
        case 0x20:
            NotifyManager::notify(NotifyTypes::SKILL_FLEE_PLUS_50);
            break;
        case 0x28:
            NotifyManager::notify(NotifyTypes::SKILL_FULL_STRIP_FAILED);
            break;
        default:
            NotifyManager::notify(NotifyTypes::SKILL_MESSAGE_UNKNOWN);
            break;
    }
}

void PlayerRecv::processNotifyMapInfo(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("type");
}

void PlayerRecv::processPlayerFameBlacksmith(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerRecv::processPlayerFameAlchemist(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerRecv::processPlayerUpgradeMessage(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("result");
    msg.readInt16("item id");
}

void PlayerRecv::processPlayerFameTaekwon(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("points");
    msg.readInt32("total points");
}

void PlayerRecv::processPlayerReadBook(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("book id");
    msg.readInt32("page");
}

void PlayerRecv::processPlayerEquipTickAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("unused");
    msg.readInt32("flag");
}

void PlayerRecv::processPlayerAutoShadowSpellList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int count = (msg.readInt16("len") - 8) / 2;
    for (int f = 0; f < count; f ++)
        msg.readInt16("skill id");
}

void PlayerRecv::processPlayerRankPoints(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("type");
    msg.readInt32("points");
    msg.readInt32("fame");
}

void PlayerRecv::processOnlineList(Net::MessageIn &msg)
{
    if (!whoIsOnline)
        return;

    BLOCK_START("PlayerRecv::processOnlineList")
    const int size = msg.readInt16("len") - 4;
    std::vector<OnlinePlayer*> arr;

    if (!size)
    {
        if (whoIsOnline)
            whoIsOnline->loadList(arr);
        BLOCK_END("PlayerRecv::processOnlineList")
        return;
    }

    char *const start = reinterpret_cast<char*>(msg.readBytes(size, "nicks"));
    if (!start)
    {
        BLOCK_END("PlayerRecv::processOnlineList")
        return;
    }

    const char *buf = start;

    int addVal = 3;

    while (buf - start + 1 < size
           && *(buf + CAST_SIZE(addVal)))
    {
        const unsigned char status = *buf;
        buf ++;
        const unsigned char level = *buf;
        buf ++;
        const unsigned char ver = *buf;
        buf ++;

        GenderT gender = Gender::UNSPECIFIED;
        if (config.getBoolValue("showgender"))
        {
            if (status & BeingFlag::GENDER_MALE)
                gender = Gender::MALE;
            else if (status & BeingFlag::GENDER_OTHER)
                gender = Gender::OTHER;
            else
                gender = Gender::FEMALE;
        }
        arr.push_back(new OnlinePlayer(static_cast<const char*>(buf),
            status, level, gender, ver));
        buf += strlen(buf) + 1;
    }

    if (whoIsOnline)
        whoIsOnline->loadList(arr);
    delete [] start;
    BLOCK_END("PlayerRecv::processOnlineList")
}

void PlayerRecv::processDressRoomOpen(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("view");
}

void PlayerRecv::processKilledBy(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("killer id");
    const Being *const dstBeing = actorManager->findBeing(id);
    if (id == BeingId_zero)
    {
        debugMsg(
            // TRANSLATORS: player killed message
            _("You were killed by unknown source."));
    }
    else
    {
        std::string name;
        if (dstBeing)
            name = dstBeing->getName();
        else
            name = strprintf("?%u", CAST_U32(id));
        debugMsg(strprintf(
            // TRANSLATORS: player killed message
            _("You were killed by %s."),
            name.c_str()));
    }
}

}  // namespace EAthena
