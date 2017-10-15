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

#ifndef NET_EATHENA_PLAYERRECV_H
#define NET_EATHENA_PLAYERRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace PlayerRecv
    {
        void processPlayerShortcuts(Net::MessageIn &msg);
        void processPlayerShowEquip(Net::MessageIn &msg);
        void processPlayerStatUpdate5(Net::MessageIn &msg);
        void processPlayerGetExp(Net::MessageIn &msg);
        void processPvpInfo(Net::MessageIn &msg);
        void processPlayerHeal(Net::MessageIn &msg);
        void processPlayerSkillMessage(Net::MessageIn &msg);
        void processWalkResponse(Net::MessageIn &msg);
        void processWalkError(Net::MessageIn &msg);
        void processNotifyMapInfo(Net::MessageIn &msg);
        void processPlayerFameBlacksmith(Net::MessageIn &msg);
        void processPlayerFameAlchemist(Net::MessageIn &msg);
        void processPlayerUpgradeMessage(Net::MessageIn &msg);
        void processPlayerFameTaekwon(Net::MessageIn &msg);
        void processPlayerReadBook(Net::MessageIn &msg);
        void processPlayerEquipTickAck(Net::MessageIn &msg);
        void processPlayerAutoShadowSpellList(Net::MessageIn &msg);
        void processPlayerRankPoints(Net::MessageIn &msg);
        void processOnlineList(Net::MessageIn &msg);
        void processDressRoomOpen(Net::MessageIn &msg);
        void processKilledBy(Net::MessageIn &msg);
        void processPlayerAttrs(Net::MessageIn &msg);
        void processPlayerStatUpdate7(Net::MessageIn &msg);
    }  // namespace PlayerRecv
}  // namespace EAthena

#endif  // NET_EATHENA_PLAYERRECV_H
