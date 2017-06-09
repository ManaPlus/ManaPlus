/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/adminhandler.h"

#include "game.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "utils/gmfunctions.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace TmwAthena
{

AdminHandler::AdminHandler() :
    Ea::AdminHandler()
{
    adminHandler = this;
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
}

void AdminHandler::warp(const std::string &map, const int x, const int y) const
{
    Gm::runCommand("warp",
        strprintf("%s %d %d", map.c_str(), x, y));
}

void AdminHandler::resetStats() const
{
}

void AdminHandler::resetSkills() const
{
}

void AdminHandler::gotoName(const std::string &name) const
{
    Gm::runCommand("goto", name);
}

void AdminHandler::recallName(const std::string &name) const
{
    Gm::runCommand("recall", name);
}

void AdminHandler::mute(const Being *const being A_UNUSED,
                        const int type A_UNUSED,
                        const int limit A_UNUSED) const
{
}

void AdminHandler::muteName(const std::string &name A_UNUSED) const
{
}

void AdminHandler::requestLogin(const Being *const being A_UNUSED) const
{
}

void AdminHandler::setTileType(const int x A_UNUSED, const int y A_UNUSED,
                               const int type A_UNUSED) const
{
}

void AdminHandler::unequipAll(const Being *const being A_UNUSED) const
{
}

void AdminHandler::requestStats(const std::string &name A_UNUSED) const
{
}

void AdminHandler::monsterInfo(const std::string &name A_UNUSED) const
{
}

void AdminHandler::itemInfo(const std::string &name A_UNUSED) const
{
}

void AdminHandler::whoDrops(const std::string &name A_UNUSED) const
{
}

void AdminHandler::mobSearch(const std::string &name A_UNUSED) const
{
}

void AdminHandler::mobSpawnSearch(const std::string &name A_UNUSED) const
{
}

void AdminHandler::playerGmCommands(const std::string &name A_UNUSED) const
{
}

void AdminHandler::playerCharGmCommands(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showLevel(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showStats(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showStorageList(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showCartList(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showInventoryList(const std::string &name A_UNUSED) const
{
}

void AdminHandler::locatePlayer(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showAccountInfo(const std::string &name A_UNUSED) const
{
}

void AdminHandler::spawnSlave(const std::string &name A_UNUSED) const
{
}

void AdminHandler::spawnClone(const std::string &name A_UNUSED) const
{
}

void AdminHandler::spawnSlaveClone(const std::string &name A_UNUSED) const
{
}

void AdminHandler::spawnEvilClone(const std::string &name A_UNUSED) const
{
}

void AdminHandler::savePosition(const std::string &name A_UNUSED) const
{
}

void AdminHandler::loadPosition(const std::string &name A_UNUSED) const
{
}

void AdminHandler::randomWarp(const std::string &name A_UNUSED) const
{
}

void AdminHandler::gotoNpc(const std::string &name A_UNUSED) const
{
}

void AdminHandler::killer(const std::string &name A_UNUSED) const
{
}

void AdminHandler::killable(const std::string &name A_UNUSED) const
{
}

void AdminHandler::heal(const std::string &name A_UNUSED) const
{
}

void AdminHandler::alive(const std::string &name) const
{
    Gm::runCommand("revive", name);
}

void AdminHandler::disguise(const std::string &name A_UNUSED) const
{
}

void AdminHandler::immortal(const std::string &name A_UNUSED) const
{
}

void AdminHandler::hide(const std::string &name A_UNUSED) const
{
    Gm::runCommand("hide");
}

void AdminHandler::nuke(const std::string &name A_UNUSED) const
{
}

void AdminHandler::kill(const std::string &name A_UNUSED) const
{
}

void AdminHandler::jail(const std::string &name A_UNUSED) const
{
}

void AdminHandler::unjail(const std::string &name A_UNUSED) const
{
}

void AdminHandler::npcMove(const std::string &name A_UNUSED,
                           const int x A_UNUSED,
                           const int y A_UNUSED) const
{
}

void AdminHandler::hideNpc(const std::string &name A_UNUSED) const
{
}

void AdminHandler::showNpc(const std::string &name A_UNUSED) const
{
}

void AdminHandler::changePartyLeader(const std::string &name A_UNUSED) const
{
}

void AdminHandler::partyRecall(const std::string &name A_UNUSED) const
{
}

void AdminHandler::breakGuild(const std::string &name A_UNUSED) const
{
}

void AdminHandler::guildRecall(const std::string &name A_UNUSED) const
{
}

void AdminHandler::slide(const int x, const int y) const
{
    warp(Game::instance()->getCurrentMapName(), x, y);
}

}  // namespace TmwAthena
