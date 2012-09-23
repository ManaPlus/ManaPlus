/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef NET_EATHENA_GUILDHANDLER_H
#define NET_EATHENA_GUILDHANDLER_H

#include "net/guildhandler.h"

#include "net/ea/guildhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

class GuildHandler final : public Ea::GuildHandler, public MessageHandler
{
    public:
        GuildHandler();

        ~GuildHandler();

        void handleMessage(Net::MessageIn &msg);

        void create(const std::string &name);

        void invite(int guildId, const std::string &name);

        void invite(int guildId, const Being *const being);

        void inviteResponse(int guildId, bool response);

        void leave(int guildId);

        void kick(GuildMember *member, std::string reason = "");

        void chat(int guildId, const std::string &text);

        void memberList(int guildId);

        void info(int guildId);

        void changeMemberPostion(GuildMember *member, int level);

        void changeNotice(int guildId, std::string msg1, std::string msg2);
};

    extern Ea::GuildTab *guildTab;
    extern Guild *taGuild;
}

#endif // NET_EATHENA_GUILDHANDLER_H
