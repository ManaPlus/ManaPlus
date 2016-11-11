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

#include "net/eathena/npcrecv.h"

#include "actormanager.h"

#include "being/being.h"

#include "gui/windows/cutinwindow.h"
#include "gui/windows/npcdialog.h"

#include "net/messagein.h"
#include "net/npchandler.h"

#include "net/ea/npcrecv.h"

#include "debug.h"

namespace EAthena
{


void NpcRecv::processNpcCutin(Net::MessageIn &msg)
{
    Ea::NpcRecv::mRequestLang = false;
    const std::string image = msg.readString(64, "image name");
    const CutInT cutin = static_cast<CutInT>(msg.readUInt8("type"));
    if (cutInWindow)
        cutInWindow->show(image, cutin);
}

void NpcRecv::processNpcViewPoint(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    Ea::NpcRecv::mRequestLang = false;
    // +++ probably need add nav point and start moving to it
    msg.readInt32("npc id");
    msg.readInt32("type");  // 0 display for 15 sec,
                            // 1 display until teleport,
                            // 2 remove
    msg.readInt32("x");
    msg.readInt32("y");
    msg.readUInt8("number");  // can be used for scripts
    msg.readInt32("color");
}

void NpcRecv::processNpcShowProgressBar(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    Ea::NpcRecv::mRequestLang = false;
    // +++ probably need show progress bar in npc dialog
    msg.readInt32("color");
    msg.readInt32("seconds");
}

void NpcRecv::processNpcCloseTimeout(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    Ea::NpcRecv::mRequestLang = false;
    // this packet send after npc closed by timeout.
    msg.readInt32("npc id");
}

void NpcRecv::processArea(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len");
    if (len < 12)
        return;
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("npc id"));
    const int area = msg.readInt32("area size");
    if (dstBeing)
        dstBeing->setAreaSize(area);
}

void NpcRecv::processShowDigit(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readUInt8("type");
    msg.readInt32("value");
}

void NpcRecv::processProgressBarAbort(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
}

void NpcRecv::processNpcSkin(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len");
    npcHandler->getNpc(msg, NpcAction::Other);
    const std::string skin = msg.readString(len - 8, "skin");
    if (Ea::NpcRecv::mDialog)
        Ea::NpcRecv::mDialog->setSkin(skin);
}

}  // namespace EAthena
