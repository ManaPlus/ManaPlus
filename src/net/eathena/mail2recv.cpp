/*
 *  The ManaPlus Client
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

#include "net/eathena/mail2recv.h"

#include "logger.h"

#include "net/messagein.h"

#include "const/resources/item/cards.h"

#include "debug.h"

namespace EAthena
{

void Mail2Recv::processMailIcon(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("show icon");
}

void Mail2Recv::processOpenNewMailWindow(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "receiver");
    msg.readUInt8("result");
}

void Mail2Recv::processAddItemResult(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("result");
    msg.readInt16("index");
    msg.readInt16("count");
    msg.readInt16("itid");
    msg.readUInt8("type");
    msg.readUInt8("identify");
    msg.readUInt8("damaged");
    msg.readUInt8("refine");
    for (int f = 0; f < maxCards; f++)
        msg.readUInt16("card");
    for (int f = 0; f < 5; f ++)
    {
        msg.readInt16("option index");
        msg.readInt16("option value");
        msg.readUInt8("option param");
    }
    msg.readInt16("weight");
    msg.readUInt8("unknown 1");
    msg.readUInt8("unknown 2");
    msg.readUInt8("unknown 3");
    msg.readUInt8("unknown 4");
    msg.readUInt8("unknown 5");
}

void Mail2Recv::processRemoveItemResult(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("result");
    msg.readInt16("index");
    msg.readInt16("count");
    msg.readInt16("weight");
}

}  // namespace EAthena
