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

#include "net/eathena/attendancehandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;
extern int packetVersionRe;
extern int packetVersionMain;
extern int packetVersionZero;

namespace EAthena
{

AttendanceHandler::AttendanceHandler()
{
    attendanceHandler = this;
}

AttendanceHandler::~AttendanceHandler()
{
    attendanceHandler = nullptr;
}

void AttendanceHandler::openAttendanceWindow() const
{
    if (packetVersion < 20160323)
        return;
    createOutPacket(CMSG_WINDOW_OPEN_REQUEST2);
    outMsg.writeInt8(5, "window type");
}

void AttendanceHandler::requestReward() const
{
    if (packetVersionRe < 20180307 &&
        packetVersionMain < 20180404 &&
        packetVersionZero < 20180411)
    {
        return;
    }

    createOutPacket(CMSG_ATTENDANCE_REWARD);
}

}  // namespace EAthena
