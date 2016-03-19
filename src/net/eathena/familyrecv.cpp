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

#include "net/eathena/familyrecv.h"

#include "logger.h"
#include "notifymanager.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

namespace FamilyRecv
{
    int mParent1 = 0;
    int mParent2 = 0;
}  // namespace FamilyRecv

void FamilyRecv::processAskForChild(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("account id who ask");
    msg.readInt32("acoount id for other parent");
    msg.readString(24, "name who ask");
}

void FamilyRecv::processCallPartner(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    if (localPlayer && name == localPlayer->getName())
    {
        NotifyManager::notify(NotifyTypes::CALLED_PARTNER);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::CALLING_PARTNER, name.c_str());
    }
}

void FamilyRecv::processDivorced(Net::MessageIn &msg)
{
    const std::string name = msg.readString(24, "name");
    NotifyManager::notify(NotifyTypes::DIVORCED, name.c_str());
}

void FamilyRecv::processAskForChildReply(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("type");
}

}  // namespace EAthena
