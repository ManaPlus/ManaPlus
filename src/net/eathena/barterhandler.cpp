/*
 *  The ManaPlus Client
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

#include "net/eathena/barterhandler.h"

#include "net/eathena/barterrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersionMain;
extern int packetVersionRe;
extern int packetVersionZero;

namespace EAthena
{

BarterHandler::BarterHandler() :
    Net::BarterHandler()
{
    barterHandler = this;
    BarterRecv::mBuyDialog = nullptr;
}

BarterHandler::~BarterHandler()
{
    barterHandler = nullptr;
}

void BarterHandler::close() const
{
    if (packetVersionMain < 20190116 &&
        packetVersionRe < 20190116 &&
        packetVersionZero < 20181226)
    {
        return;
    }

    createOutPacket(CMSG_NPC_BARTER_CLOSE);
}

}  // namespace EAthena
