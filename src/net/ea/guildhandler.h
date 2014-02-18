/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

namespace Ea
{
class GuildTab;

class GuildHandler : public Net::GuildHandler
{
    public:
        GuildHandler();

        A_DELETE_COPY(GuildHandler)

        virtual ~GuildHandler();

        void requestAlliance(const int guildId,
                             const int otherGuildId) const override final;

        void requestAllianceResponse(const int guildId, const int otherGuildId,
                                     const bool response) const override final;

        void endAlliance(const int guildId,
                         const int otherGuildId) const override final;

        virtual void processGuildCreateResponse(Net::MessageIn &msg) const;

        virtual void processGuildPositionInfo(Net::MessageIn &msg) const;

        virtual void processGuildMemberLogin(Net::MessageIn &msg) const;

        virtual void processGuildMasterOrMember(Net::MessageIn &msg) const;

        virtual void processGuildBasicInfo(Net::MessageIn &msg);

        virtual void processGuildAlianceInfo(Net::MessageIn &msg) const;

        virtual void processGuildMemberList(Net::MessageIn &msg) const;

        virtual void processGuildPosNameList(Net::MessageIn &msg) const;

        virtual void processGuildPosInfoList(Net::MessageIn &msg) const;

        virtual void processGuildPositionChanged(Net::MessageIn &msg) const;

        virtual void processGuildMemberPosChange(Net::MessageIn &msg) const;

        virtual void processGuildEmblem(Net::MessageIn &msg) const;

        virtual void processGuildSkillInfo(Net::MessageIn &msg) const;

        virtual void processGuildNotice(Net::MessageIn &msg) const;

        virtual void processGuildInvite(Net::MessageIn &msg) const;

        virtual void processGuildInviteAck(Net::MessageIn &msg) const;

        virtual void processGuildLeave(Net::MessageIn &msg) const;

        virtual void processGuildExpulsion(Net::MessageIn &msg) const;

        virtual void processGuildExpulsionList(Net::MessageIn &msg) const;

        virtual void processGuildMessage(Net::MessageIn &msg) const;

        virtual void processGuildSkillUp(Net::MessageIn &msg) const;

        virtual void processGuildReqAlliance(Net::MessageIn &msg) const;

        virtual void processGuildReqAllianceAck(Net::MessageIn &msg) const;

        virtual void processGuildDelAlliance(Net::MessageIn &msg) const;

        virtual void processGuildOppositionAck(Net::MessageIn &msg) const;

        virtual void processGuildBroken(Net::MessageIn &msg) const;

        void clear() const override final;

        ChatTab *getTab() const override final;

    protected:
        bool showBasicInfo;
};

extern Guild *taGuild;
extern GuildTab *guildTab;
}  // namespace Ea

#endif  // NET_EA_GUILDHANDLER_H
