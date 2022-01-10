/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef NET_EATHENA_ACHIEVEMENTRECV_H
#define NET_EATHENA_ACHIEVEMENTRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace AchievementRecv
    {
        void processAchievementList(Net::MessageIn &msg);
        void processAchievementUpdate(Net::MessageIn &msg);
        void processAchievementGetReward(Net::MessageIn &msg);
    }  // namespace AchievementRecv
}  // namespace EAthena

#endif  // NET_EATHENA_ACHIEVEMENTRECV_H
