/*
 *  The ManaPlus Client
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

#ifndef NET_TMWA_GUILDHANDLER_H
#define NET_TMWA_GUILDHANDLER_H

#include "net/guildhandler.h"

namespace TmwAthena
{

class GuildHandler final : public Net::GuildHandler
{
    public:
        GuildHandler();

        A_DELETE_COPY(GuildHandler)

        ~GuildHandler() override final;

        void clear() const override final;

        ChatTab *getTab() const override final;

        void create(const std::string &name) const override final;

        void invite(const std::string &name) const override final;

        void invite(const Being *const being) const override final;

        void inviteResponse(const int guildId,
                            const bool response) const override final;

        void leave(const int guildId) const override final;

        void kick(const GuildMember *restrict const member,
                  const std::string &restrict reason) const override final;

        void chat(const std::string &text) const override final;

        void memberList() const override final;

        void info() const override final;

        void changeMemberPostion(const GuildMember *const member,
                                 const int level) const override final;

        void changeNotice(const int guildId,
                          const std::string &restrict msg1,
                          const std::string &restrict msg2)
                          const override final;

        void checkMaster() const override final;

        void requestAlliance(const Being *const being) const override final
                             A_CONST;

        void requestAllianceResponse(const int beingId,
                                     const bool accept) const override final
                                     A_CONST;

        void endAlliance(const int guildId,
                         const int flag) const override final A_CONST;

        void changePostionInfo(const int posId,
                               const int mode,
                               const int ranking,
                               const int payRate,
                               const std::string &name) const override final
                               A_CONST;

        void requestOpposition(const Being *const being) const override final
                               A_CONST;

        void breakGuild(const std::string &name) const override final A_CONST;

        void changeEmblem(std::string emblem) const override final A_CONST;

        void requestEmblem(const int guildId) const override final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_GUILDHANDLER_H
