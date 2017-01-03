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

#ifndef NET_TMWA_ADMINHANDLER_H
#define NET_TMWA_ADMINHANDLER_H

#include "net/ea/adminhandler.h"

namespace TmwAthena
{

class AdminHandler final : public Ea::AdminHandler
{
    public:
        AdminHandler();

        A_DELETE_COPY(AdminHandler)

        void announce(const std::string &text) const override final;

        void localAnnounce(const std::string &text) const override final;

        void hide(const bool h) const override final;

        void kick(const BeingId playerId) const override final;

        void kickAll() const override final A_CONST;

        void warp(const std::string &map,
                  const int x, const int y) const override final;

        void resetStats() const override final A_CONST;

        void resetSkills() const override final A_CONST;

        void gotoName(const std::string &name) const override final;

        void recallName(const std::string &name) const override final;

        void mute(const Being *const being,
                  const int type,
                  const int limit) const override final A_CONST;

        void muteName(const std::string &name) const override final A_CONST;

        void requestLogin(const Being *const being) const override final
                          A_CONST;

        void setTileType(const int x, const int y,
                         const int type) const override final A_CONST;

        void unequipAll(const Being *const being) const override final A_CONST;

        void requestStats(const std::string &name) const override final
                          A_CONST;

        void monsterInfo(const std::string &name) const override final A_CONST;

        void itemInfo(const std::string &name) const override final A_CONST;

        void whoDrops(const std::string &name) const override final A_CONST;

        void mobSearch(const std::string &name) const override final A_CONST;

        void mobSpawnSearch(const std::string &name) const override final
                            A_CONST;

        void playerGmCommands(const std::string &name) const override final
                              A_CONST;

        void playerCharGmCommands(const std::string &name) const override final
                                  A_CONST;

        void showLevel(const std::string &name) const override final A_CONST;

        void showStats(const std::string &name) const override final A_CONST;

        void showStorageList(const std::string &name) const override final
                             A_CONST;

        void showCartList(const std::string &name) const override final
                          A_CONST;

        void showInventoryList(const std::string &name) const override final
                               A_CONST;

        void locatePlayer(const std::string &name) const override final
                          A_CONST;

        void showAccountInfo(const std::string &name) const override final
                             A_CONST;

        void spawnSlave(const std::string &name) const override final A_CONST;

        void spawnClone(const std::string &name) const override final A_CONST;

        void spawnSlaveClone(const std::string &name) const override final
                             A_CONST;

        void spawnEvilClone(const std::string &name) const override final
                            A_CONST;

        void savePosition(const std::string &name) const override final
                          A_CONST;

        void loadPosition(const std::string &name) const override final
                          A_CONST;

        void randomWarp(const std::string &name) const override final A_CONST;

        void gotoNpc(const std::string &name) const override final A_CONST;

        void killer(const std::string &name) const override final A_CONST;

        void killable(const std::string &name) const override final A_CONST;

        void heal(const std::string &name) const override final A_CONST;

        void alive(const std::string &name) const override final;

        void disguise(const std::string &name) const override final A_CONST;

        void immortal(const std::string &name) const override final A_CONST;

        void hide(const std::string &name) const override final;

        void nuke(const std::string &name) const override final A_CONST;

        void kill(const std::string &name) const override final A_CONST;

        void jail(const std::string &name) const override final A_CONST;

        void unjail(const std::string &name) const override final A_CONST;

        void npcMove(const std::string &name,
                     const int x,
                     const int y) const override final A_CONST;

        void hideNpc(const std::string &name) const override final A_CONST;

        void showNpc(const std::string &name) const override final A_CONST;

        void changePartyLeader(const std::string &name) const override final
                               A_CONST;

        void partyRecall(const std::string &name) const override final A_CONST;

        void breakGuild(const std::string &name) const override final A_CONST;

        void guildRecall(const std::string &name) const override final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_ADMINHANDLER_H
