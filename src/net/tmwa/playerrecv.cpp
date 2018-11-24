/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/tmwa/playerrecv.h"

#include "actormanager.h"
#include "configuration.h"

#include "being/beingflag.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/onlineplayer.h"

#include "gui/windows/statuswindow.h"
#include "gui/windows/whoisonline.h"

#include "net/messagein.h"

#include "resources/db/groupdb.h"

#include "debug.h"

namespace TmwAthena
{

void PlayerRecv::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::PLAYER_CHAR_POINTS,
        msg.readInt16("char points"),
        Notify_true);

    unsigned int val = msg.readUInt8("str");
    PlayerInfo::setStatBase(Attributes::PLAYER_STR, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_STR,
            msg.readUInt8("str cost"));
    }
    else
    {
        msg.readUInt8("str cost");
    }

    val = msg.readUInt8("agi");
    PlayerInfo::setStatBase(Attributes::PLAYER_AGI, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_AGI,
            msg.readUInt8("agi cost"));
    }
    else
    {
        msg.readUInt8("agi cost");
    }

    val = msg.readUInt8("vit");
    PlayerInfo::setStatBase(Attributes::PLAYER_VIT, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_VIT,
            msg.readUInt8("vit cost"));
    }
    else
    {
        msg.readUInt8("vit cost");
    }

    val = msg.readUInt8("int");
    PlayerInfo::setStatBase(Attributes::PLAYER_INT, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_INT,
            msg.readUInt8("int cost"));
    }
    else
    {
        msg.readUInt8("int cost");
    }

    val = msg.readUInt8("dex");
    PlayerInfo::setStatBase(Attributes::PLAYER_DEX, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_DEX,
            msg.readUInt8("dex cost"));
    }
    else
    {
        msg.readUInt8("dex cost");
    }

    val = msg.readUInt8("luk");
    PlayerInfo::setStatBase(Attributes::PLAYER_LUK, val, Notify_true);
    if (statusWindow != nullptr)
    {
        statusWindow->setPointsNeeded(Attributes::PLAYER_LUK,
            msg.readUInt8("luk cost"));
    }
    else
    {
        msg.readUInt8("luk cost");
    }

    PlayerInfo::setStatBase(Attributes::PLAYER_ATK,
        msg.readInt16("atk"),
        Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_ATK,
        msg.readInt16("atk+"),
        Notify_true);
    PlayerInfo::updateAttrs();

    val = msg.readInt16("matk");
    PlayerInfo::setStatBase(Attributes::PLAYER_MATK, val, Notify_false);

    val = msg.readInt16("matk+");
    PlayerInfo::setStatMod(Attributes::PLAYER_MATK,
        val,
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_DEF,
        msg.readInt16("def"),
        Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_DEF,
        msg.readInt16("def+"),
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_MDEF,
        msg.readInt16("mdef"),
        Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_MDEF,
        msg.readInt16("mdef+"),
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_HIT,
        msg.readInt16("hit"),
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_FLEE,
        msg.readInt16("flee"),
        Notify_false);
    PlayerInfo::setStatMod(Attributes::PLAYER_FLEE,
        msg.readInt16("flee+"),
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_CRIT,
        msg.readInt16("crit"),
        Notify_true);

    PlayerInfo::setStatBase(Attributes::PLAYER_MANNER,
        msg.readInt16("manner"),
        Notify_true);
    msg.readInt16("unused?");
    BLOCK_END("PlayerRecv::processPlayerStatUpdate5")
}

void PlayerRecv::processWalkResponse(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processWalkResponse")
    /*
      * This client assumes that all walk messages succeed,
      * and that the server will send a correction notice
      * otherwise.
      */
    uint16_t srcX;
    uint16_t srcY;
    uint16_t dstX;
    uint16_t dstY;
    msg.readInt32("tick");
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("unused");
    if (localPlayer != nullptr)
        localPlayer->setRealPos(dstX, dstY);
    BLOCK_END("PlayerRecv::processWalkResponse")
}

void PlayerRecv::processOnlineList(Net::MessageIn &msg)
{
    if (whoIsOnline == nullptr)
        return;

    BLOCK_START("PlayerRecv::processOnlineList")
    const int count = (msg.readInt16("len") - 4) / 31;
    STD_VECTOR<OnlinePlayer*> arr;

    if (count == 0)
    {
        if (whoIsOnline != nullptr)
            whoIsOnline->loadList(arr);
        BLOCK_END("PlayerRecv::processOnlineList")
        return;
    }

    for (int f = 0; f < count; f ++)
    {
        const BeingId beingId = msg.readBeingId("account id");
        const std::string name = msg.readString(24, "name");
        const unsigned char level = msg.readUInt8("level");
        const unsigned char group = msg.readUInt8("group");
        GenderT gender = static_cast<GenderT>(msg.readUInt8("gender"));
        unsigned char status = 0;
        switch (gender)
        {
            case Gender::MALE:
                status |= BeingFlag::GENDER_MALE;
                break;
            case Gender::HIDDEN:
                status |= BeingFlag::GENDER_HIDDEN;
                break;
            case Gender::FEMALE:
            case Gender::UNSPECIFIED:
            default:
                break;
        }

        if (!config.getBoolValue("showgender"))
            gender = Gender::UNSPECIFIED;
        arr.push_back(new OnlinePlayer(name,
            status, level, gender, 0, group));
        if (actorManager)
            actorManager->updateNameId(name, beingId);
    }

    if (whoIsOnline != nullptr)
        whoIsOnline->loadList(arr);
    BLOCK_END("PlayerRecv::processOnlineList")
}

}  // namespace TmwAthena
