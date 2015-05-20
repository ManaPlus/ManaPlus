/*
 *  The ManaPlus Client
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

#ifndef NET_EA_GUILDHANDLER_H
#define NET_EA_GUILDHANDLER_H

#include "net/guildhandler.h"

namespace Net
{
    class MessageIn;
}

class GuildTab;

namespace Ea
{
class GuildHandler notfinal : public Net::GuildHandler
{
    public:
        GuildHandler();

        A_DELETE_COPY(GuildHandler)

        virtual ~GuildHandler();

        void endAlliance(const int guildId,
                         const int otherGuildId) const override final;

        static void processGuildCreateResponse(Net::MessageIn &msg);

        static void processGuildMasterOrMember(Net::MessageIn &msg);

        static void processGuildBasicInfo(Net::MessageIn &msg);

        static void processGuildAlianceInfo(Net::MessageIn &msg);

        static void processGuildMemberList(Net::MessageIn &msg);

        static void processGuildPosNameList(Net::MessageIn &msg);

        static void processGuildPosInfoList(Net::MessageIn &msg);

        static void processGuildPositionChanged(Net::MessageIn &msg);

        static void processGuildMemberPosChange(Net::MessageIn &msg);

        static void processGuildEmblemData(Net::MessageIn &msg);

        static void processGuildSkillInfo(Net::MessageIn &msg);

        static void processGuildNotice(Net::MessageIn &msg);

        static void processGuildInvite(Net::MessageIn &msg);

        static void processGuildInviteAck(Net::MessageIn &msg);

        static void processGuildLeave(Net::MessageIn &msg);

        static void processGuildMessage(Net::MessageIn &msg);

        static void processGuildSkillUp(Net::MessageIn &msg);

        static void processGuildReqAlliance(Net::MessageIn &msg);

        static void processGuildReqAllianceAck(Net::MessageIn &msg);

        static void processGuildDelAlliance(Net::MessageIn &msg);

        static void processGuildOppositionAck(Net::MessageIn &msg);

        static void processGuildBroken(Net::MessageIn &msg);

        static void processGuildExpulsionContinue(const std::string &nick);

        void clear() const override final;

        ChatTab *getTab() const override final;

    protected:
        static bool showBasicInfo;
};

extern Guild *taGuild;
}  // namespace Ea

extern GuildTab *guildTab;

#endif  // NET_EA_GUILDHANDLER_H
