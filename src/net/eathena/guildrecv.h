/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef NET_EATHENA_GUILDRECV_H
#define NET_EATHENA_GUILDRECV_H

#include <string>

namespace Net
{
    class MessageIn;
}  // namespace Net

class Guild;
class GuildTab;

namespace EAthena
{
    namespace GuildRecv
    {
        extern bool showBasicInfo;

        void processGuildCreateResponse(Net::MessageIn &msg);
        void processGuildMasterOrMember(Net::MessageIn &msg);
        void processGuildBasicInfo(Net::MessageIn &msg);
        void processGuildAlianceInfo(Net::MessageIn &msg);
        void processGuildMemberList(Net::MessageIn &msg);
        void processGuildPosNameList(Net::MessageIn &msg);
        void processGuildPosInfoList(Net::MessageIn &msg);
        void processGuildPositionChanged(Net::MessageIn &msg);
        void processGuildMemberPosChange(Net::MessageIn &msg);
        void processGuildEmblemData(Net::MessageIn &msg);
        void processGuildSkillInfo(Net::MessageIn &msg);
        void processGuildNotice(Net::MessageIn &msg);
        void processGuildInvite(Net::MessageIn &msg);
        void processGuildInviteAck(Net::MessageIn &msg);
        void processGuildLeave(Net::MessageIn &msg);
        void processGuildLeave2(Net::MessageIn &msg);
        void processGuildMessage(Net::MessageIn &msg);
        void processGuildReqAlliance(Net::MessageIn &msg);
        void processGuildReqAllianceAck(Net::MessageIn &msg);
        void processGuildDelAlliance(Net::MessageIn &msg);
        void processGuildOppositionAck(Net::MessageIn &msg);
        void processGuildBroken(Net::MessageIn &msg);
        void processGuildExpulsionContinue(const std::string &nick);
        void processGuildUpdateCoords(Net::MessageIn &msg);
        void processGuildPositionInfo(Net::MessageIn &msg);
        void processGuildMemberLogin(Net::MessageIn &msg);
        void processGuildExpulsion(Net::MessageIn &msg);
        void processGuildExpulsion2(Net::MessageIn &msg);
        void processGuildExpulsionList1(Net::MessageIn &msg);
        void processGuildExpulsionList2(Net::MessageIn &msg);
        void processGuildExpulsionList3(Net::MessageIn &msg);
        void processGuildEmblem(Net::MessageIn &msg);
        void processOnlineInfo(Net::MessageIn &msg);
        void processGuildSetPosition(Net::MessageIn &msg);
    }  // namespace GuildRecv
    extern Guild *taGuild;
}  // namespace EAthena

extern GuildTab *guildTab;

#endif  // NET_EATHENA_GUILDRECV_H
