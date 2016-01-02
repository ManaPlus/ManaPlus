/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/eathena/elementalrecv.h"

#include "logger.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

void ElementalRecv::processElementalUpdateStatus(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("type");
    msg.readInt32("value");
}

void ElementalRecv::processElementalInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("elemental id");
    msg.readInt32("hp");
    msg.readInt32("max hp");
    msg.readInt32("sp");
    msg.readInt32("max sp");
}

}  // namespace EAthena
