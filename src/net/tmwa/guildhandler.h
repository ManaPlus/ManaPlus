/*
 *  The ManaPlus Client
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

#ifndef NET_TMWA_GUILDHANDLER_H
#define NET_TMWA_GUILDHANDLER_H

#include "net/ea/guildhandler.h"

namespace TmwAthena
{

class GuildHandler final : public Ea::GuildHandler
{
    public:
        GuildHandler();

        A_DELETE_COPY(GuildHandler)

        ~GuildHandler();

        void create(const std::string &name) const final;

        void invite(const std::string &name) const final;

        void invite(const Being *const being) const final;

        void inviteResponse(const int guildId,
                            const bool response) const final;

        void leave(const int guildId) const final;

        void kick(const GuildMember *restrict const member,
                  const std::string &restrict reason) const final;

        void chat(const std::string &text) const final;

        void memberList() const final;

        void info() const final;

        void changeMemberPostion(const GuildMember *const member,
                                 const int level) const final;

        void changeNotice(const int guildId,
                          const std::string &restrict msg1,
                          const std::string &restrict msg2)
                          const final;

        void checkMaster() const final;

        void requestAlliance(const Being *const being) const final
                             A_CONST;

        void requestAllianceResponse(const int beingId,
                                     const bool accept) const final
                                     A_CONST;

        void endAlliance(const int guildId,
                         const int flag) const final A_CONST;

        void changePostionInfo(const int posId,
                               const int mode,
                               const int ranking,
                               const int payRate,
                               const std::string &name) const final
                               A_CONST;

        void requestOpposition(const Being *const being) const final
                               A_CONST;

        void breakGuild(const std::string &name) const final A_CONST;

        void changeEmblem(std::string emblem) const final A_CONST;

        void requestEmblem(const int guildId) const final A_CONST;
};

}  // namespace TmwAthena

extern GuildTab *guildTab;

namespace Ea
{
    extern Guild *taGuild;
}

#endif  // NET_TMWA_GUILDHANDLER_H
