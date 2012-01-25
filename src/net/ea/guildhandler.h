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

#ifndef NET_EA_GUILDHANDLER_H
#define NET_EA_GUILDHANDLER_H

#include "net/guildhandler.h"
#include "net/messagein.h"

#include "net/ea/gui/guildtab.h"

namespace Ea
{

class GuildHandler : public Net::GuildHandler
{
    public:
        GuildHandler();

        ~GuildHandler();

        void requestAlliance(int guildId, int otherGuildId);

        void requestAllianceResponse(int guildId, int otherGuildId,
                                     bool response);

        void endAlliance(int guildId, int otherGuildId);

        bool isSupported();

        virtual void processGuildCreateResponse(Net::MessageIn &msg);

        virtual void processGuildPositionInfo(Net::MessageIn &msg);

        virtual void processGuildMemberLogin(Net::MessageIn &msg);

        virtual void processGuildMasterOrMember(Net::MessageIn &msg);

        virtual void processGuildBasicInfo(Net::MessageIn &msg);

        virtual void processGuildAlianceInfo(Net::MessageIn &msg);

        virtual void processGuildMemberList(Net::MessageIn &msg);

        virtual void processGuildPosNameList(Net::MessageIn &msg);

        virtual void processGuildPosInfoList(Net::MessageIn &msg);

        virtual void processGuildPositionChanged(Net::MessageIn &msg);

        virtual void processGuildMemberPosChange(Net::MessageIn &msg);

        virtual void processGuildEmblem(Net::MessageIn &msg);

        virtual void processGuildSkillInfo(Net::MessageIn &msg);

        virtual void processGuildNotice(Net::MessageIn &msg);

        virtual void processGuildInvite(Net::MessageIn &msg);

        virtual void processGuildInviteAck(Net::MessageIn &msg);

        virtual void processGuildLeave(Net::MessageIn &msg);

        virtual void processGuildExpulsion(Net::MessageIn &msg);

        virtual void processGuildExpulsionList(Net::MessageIn &msg);

        virtual void processGuildMessage(Net::MessageIn &msg);

        virtual void processGuildSkillUp(Net::MessageIn &msg);

        virtual void processGuildReqAlliance(Net::MessageIn &msg);

        virtual void processGuildReqAllianceAck(Net::MessageIn &msg);

        virtual void processGuildDelAlliance(Net::MessageIn &msg);

        virtual void processGuildOppositionAck(Net::MessageIn &msg);

        virtual void processGuildBroken(Net::MessageIn &msg);

        void clear();

    protected:
        bool showBasicInfo;
};

extern Guild *taGuild;
extern GuildTab *guildTab;
}

#endif // NET_EA_GUILDHANDLER_H
