/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/adminhandler.h"

#include "being/being.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "utils/gmfunctions.h"
#include "utils/stringutils.h"

#include "debug.h"

extern int packetVersion;
extern int packetVersionZero;

namespace EAthena
{

std::string AdminHandler::mStatsName;

AdminHandler::AdminHandler() :
    Ea::AdminHandler()
{
    adminHandler = this;
}

AdminHandler::~AdminHandler()
{
    adminHandler = nullptr;
}

void AdminHandler::announce(const std::string &text) const
{
    createOutPacket(CMSG_ADMIN_ANNOUNCE);
    outMsg.writeInt16(CAST_S16(text.length() + 4), "len");
    outMsg.writeString(text, CAST_S32(text.length()), "message");
}

void AdminHandler::localAnnounce(const std::string &text) const
{
    createOutPacket(CMSG_ADMIN_LOCAL_ANNOUNCE);
    outMsg.writeInt16(CAST_S16(text.length() + 4), "len");
    outMsg.writeString(text, CAST_S32(text.length()), "message");
}

void AdminHandler::hide(const bool h A_UNUSED) const
{
    createOutPacket(CMSG_ADMIN_HIDE);
    outMsg.writeInt32(0, "unused");
}

void AdminHandler::kick(const BeingId playerId) const
{
    createOutPacket(CMSG_ADMIN_KICK);
    outMsg.writeBeingId(playerId, "account id");
}

void AdminHandler::kickAll() const
{
    createOutPacket(CMSG_ADMIN_KICK_ALL);
}

void AdminHandler::warp(const std::string &map, const int x, const int y) const
{
    createOutPacket(CMSG_PLAYER_MAPMOVE);
    outMsg.writeString(map, 16, "map");
    outMsg.writeInt16(CAST_S16(x), "x");
    outMsg.writeInt16(CAST_S16(y), "y");
}

void AdminHandler::resetStats() const
{
    createOutPacket(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(0, "flag");
}

void AdminHandler::resetSkills() const
{
    createOutPacket(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(1, "flag");
}

void AdminHandler::gotoName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_GOTO);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::recallName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_RECALL);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::mute(const Being *const being,
                        const int type,
                        const int limit) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_ADMIN_MUTE);
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt8(CAST_S8(type), "type");
    outMsg.writeInt16(CAST_S16(limit), "value");
}

void AdminHandler::muteName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_MUTE_NAME);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::requestLogin(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_ADMIN_ID_TO_LOGIN);
    outMsg.writeBeingId(being->getId(), "account id");
}

void AdminHandler::setTileType(const int x, const int y,
                               const int type) const
{
    createOutPacket(CMSG_ADMIN_SET_TILE_TYPE);
    outMsg.writeInt16(CAST_S16(x), "x");
    outMsg.writeInt16(CAST_S16(y), "y");
    outMsg.writeInt16(CAST_S16(type), "type");
}

void AdminHandler::unequipAll(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_ADMIN_UNEQUIP_ALL);
    outMsg.writeBeingId(being->getId(), "account id");
}

void AdminHandler::requestStats(const std::string &name) const
{
    mStatsName = name;
    createOutPacket(CMSG_ADMIN_REQUEST_STATS);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::monsterInfo(const std::string &name) const
{
    Gm::runCommand("monsterinfo", name);
}

void AdminHandler::itemInfo(const std::string &name) const
{
    Gm::runCommand("iteminfo", name);
}

void AdminHandler::whoDrops(const std::string &name) const
{
    Gm::runCommand("whodrops", name);
}

void AdminHandler::mobSearch(const std::string &name) const
{
    Gm::runCommand("mobsearch", name);
}

void AdminHandler::mobSpawnSearch(const std::string &name) const
{
    Gm::runCommand("whereis", name);
}

void AdminHandler::playerGmCommands(const std::string &name) const
{
    Gm::runCharCommand("commands", name);
}

void AdminHandler::playerCharGmCommands(const std::string &name) const
{
    Gm::runCharCommand("charcommands", name);
}

void AdminHandler::showLevel(const std::string &name) const
{
    Gm::runCharCommand("exp", name);
}

void AdminHandler::showStats(const std::string &name) const
{
    Gm::runCharCommand("stats", name);
}

void AdminHandler::showStorageList(const std::string &name) const
{
    Gm::runCharCommand("storagelist", name);
}

void AdminHandler::showCartList(const std::string &name) const
{
    Gm::runCharCommand("cartlist", name);
}

void AdminHandler::showInventoryList(const std::string &name) const
{
    Gm::runCharCommand("itemlist", name);
}

void AdminHandler::locatePlayer(const std::string &name) const
{
    Gm::runCommand("where", name);
}

void AdminHandler::showAccountInfo(const std::string &name) const
{
    Gm::runCommand("accinfo", name);
}

void AdminHandler::spawnSlave(const std::string &name) const
{
    Gm::runCommand("summon", name);
}

void AdminHandler::spawnClone(const std::string &name) const
{
    Gm::runCommand("clone", name);
}

void AdminHandler::spawnSlaveClone(const std::string &name) const
{
    Gm::runCommand("slaveclone", name);
}

void AdminHandler::spawnEvilClone(const std::string &name) const
{
    Gm::runCommand("evilclone", name);
}

void AdminHandler::savePosition(const std::string &name) const
{
    Gm::runCharCommand("save", name);
}

void AdminHandler::loadPosition(const std::string &name) const
{
    Gm::runCharCommand("load", name);
}

void AdminHandler::randomWarp(const std::string &name) const
{
    Gm::runCharCommand("jump", name);
}

void AdminHandler::gotoNpc(const std::string &name) const
{
    Gm::runCommand("tonpc", name);
}

void AdminHandler::killer(const std::string &name) const
{
    Gm::runCharCommand("killer", name);
}

void AdminHandler::killable(const std::string &name) const
{
    Gm::runCharCommand("killable", name);
}

void AdminHandler::heal(const std::string &name) const
{
    Gm::runCharCommand("heal", name);
}

void AdminHandler::alive(const std::string &name) const
{
    Gm::runCharCommand("alive", name);
}

void AdminHandler::disguise(const std::string &name) const
{
    Gm::runCommand("disguise", name);
}

void AdminHandler::immortal(const std::string &name) const
{
    Gm::runCharCommand("monsterignore", name);
}

void AdminHandler::hide(const std::string &name) const
{
    Gm::runCharCommand("hide", name);
}

void AdminHandler::nuke(const std::string &name) const
{
    Gm::runCommand("nuke", name);
}

void AdminHandler::kill(const std::string &name) const
{
    Gm::runCharCommand("kill", name);
}

void AdminHandler::jail(const std::string &name) const
{
    Gm::runCommand("jail", name);
}

void AdminHandler::unjail(const std::string &name) const
{
    Gm::runCommand("unjail", name);
}

void AdminHandler::npcMove(const std::string &name,
                           const int x,
                           const int y) const
{
    Gm::runCommand("npcmove",
        strprintf("%d %d %s",
        x,
        y,
        name.c_str()));
}

void AdminHandler::hideNpc(const std::string &name) const
{
    Gm::runCommand("hidenpc", name);
}

void AdminHandler::showNpc(const std::string &name) const
{
    Gm::runCommand("shownpc", name);
}

void AdminHandler::changePartyLeader(const std::string &name) const
{
    Gm::runCommand("changeleader", name);
}

void AdminHandler::partyRecall(const std::string &name) const
{
    Gm::runCommand("partyrecall", name);
}

void AdminHandler::breakGuild(const std::string &name) const
{
    Gm::runCharCommand("breakguild", name);
}

void AdminHandler::guildRecall(const std::string &name) const
{
    Gm::runCommand("guildrecall", name);
}

void AdminHandler::slide(const int x, const int y) const
{
    Gm::runCommand("slide",
        strprintf("%d %d", x, y));
}

void AdminHandler::changeDress() const
{
    if (packetVersionZero < 20171214 &&
        packetVersion < 20171220)
    {
        return;
    }
    createOutPacket(CMSG_CHANGE_DRESS);
}

void AdminHandler::resetCooldowns() const
{
    if (packetVersion < 20160622)
        return;
    createOutPacket(CMSG_ADMIN_RESET_COOLDOWNS);
}

}  // namespace EAthena
