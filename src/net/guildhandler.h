/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_GUILDHANDLER_H
#define NET_GUILDHANDLER_H

#include "guild.h"

#include "gui/widgets/tabs/chat/chattab.h"

class Being;

namespace Net
{

class GuildHandler notfinal
{
    public:
        GuildHandler()
        { }

        A_DELETE_COPY(GuildHandler)

        virtual ~GuildHandler()
        { }

        virtual void create(const std::string &name) const = 0;

        virtual void invite(const std::string &name) const = 0;

        virtual void invite(const Being *const being) const = 0;

        virtual void inviteResponse(const int guildId,
                                    const bool response) const = 0;

        virtual void leave(const int guildId) const = 0;

        virtual void kick(const GuildMember *restrict const member,
                          const std::string &restrict reason) const = 0;

        virtual void chat(const std::string &text) const = 0;

        virtual void memberList() const = 0;

        virtual void info() const = 0;

        virtual void changeMemberPostion(const GuildMember *const member,
                                         const int level) const = 0;

        virtual void changePostionInfo(const int posId,
                                       const int mode,
                                       const int ranking,
                                       const int payRate,
                                       const std::string &name) const = 0;

        virtual void requestAlliance(const Being *const being) const = 0;

        virtual void requestOpposition(const Being *const being) const = 0;

        virtual void requestAllianceResponse(const int beingId,
                                             const bool accept) const = 0;

        virtual void endAlliance(const int guildId,
                                 const int flag) const = 0;

        virtual void changeNotice(const int guildId,
                                  const std::string &restrict msg1,
                                  const std::string &restrict msg2) const = 0;

        virtual void checkMaster() const = 0;

        virtual void breakGuild(const std::string &name) const = 0;

        virtual void changeEmblem(std::string emblem) const = 0;

        virtual void requestEmblem(const int guildId) const = 0;

        virtual void clear() const = 0;

        virtual ChatTab *getTab() const = 0;
};

}  // namespace Net

extern Net::GuildHandler *guildHandler;

#endif  // NET_GUILDHANDLER_H
