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

void Mail2Recv::processCheckNameResult(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt16("class");
    msg.readInt16("level");
    if (msg.getVersion() >= 20160316)
        msg.readString(24, "name");
}

void Mail2Recv::processSendResult(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("result");
}

void Mail2Recv::processMailListPage(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("len");
    msg.readUInt8("open type");
    const int cnt = msg.readUInt8("cnt");
    msg.readUInt8("isEnd");
    for (int f = 0; f < cnt; f ++)
    {
        msg.readInt64("mail id");
        msg.readUInt8("is read");
        msg.readUInt8("type");
        msg.readString(24, "sender name");
        msg.readInt32("reg time");
        msg.readInt32("expire time");
        msg.readString(-1, "title");
    }
}

void Mail2Recv::processReadMail(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("len");
    msg.readUInt8("open type");
    msg.readInt64("mail id");
    const int textLen = msg.readInt16("text len");
    msg.readInt64("money");
    const int itemsCount = msg.readUInt8("item count");
    msg.readString(textLen, "text message");
    for (int f = 0; f < itemsCount; f ++)
    {
        msg.readInt16("amount");
        msg.readInt16("item id");
        msg.readUInt8("identify");
        msg.readUInt8("damaged");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readUInt16("card");
        msg.readInt32("unknown");
        msg.readUInt8("type");
        msg.readInt32("unknown");
        for (int d = 0; d < 5; d ++)
        {
            msg.readInt16("option index");
            msg.readInt16("option value");
            msg.readUInt8("option param");
        }
    }
}

void Mail2Recv::processMailDelete(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("open type");
    msg.readInt64("mail id");
}

void Mail2Recv::processRequestMoney(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt64("mail id");
    msg.readUInt8("open type");
    msg.readUInt8("result");
}

void Mail2Recv::processRequestItems(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt64("mail id");
    msg.readUInt8("open type");
    msg.readUInt8("result");
}

}  // namespace EAthena
