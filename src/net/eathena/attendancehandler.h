/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_EATHENA_ATTENDANCEHANDLER_H
#define NET_EATHENA_ATTENDANCEHANDLER_H

#include "net/attendancehandler.h"

namespace EAthena
{
class AttendanceHandler final : public Net::AttendanceHandler
{
    public:
        AttendanceHandler();

        A_DELETE_COPY(AttendanceHandler)

        ~AttendanceHandler() override final;

        void openAttendanceWindow() const override final;

        void requestReward() const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_ATTENDANCEHANDLER_H
