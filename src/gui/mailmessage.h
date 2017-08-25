/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2017  The ManaPlus Developers
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

#ifndef GUI_MAILMESSAGE_H
#define GUI_MAILMESSAGE_H

#include "const/resources/item/cards.h"

#include <string>

#include "localconsts.h"

struct MailMessage final
{
    MailMessage() :
        title(),
        sender(),
        strTime(),
        text(),
        id(0),
        time(0),
        expireTime(0),
        money(0),
        itemAmount(0),
        itemId(0),
        itemType(0),
        itemAttribute(0U),
        itemRefine(0U),
        type(0U),
        read(false),
        itemIdentify(false)
    {
        for (int f = 0; f < 4; f ++)
            card[f] = 0;
    }

    A_DELETE_COPY(MailMessage)

    std::string title;
    std::string sender;
    std::string strTime;
    std::string text;
    uint16_t card[maxMailCards];
    int id;
    int time;
    int expireTime;
    int money;
    int itemAmount;
    int itemId;
    int itemType;
    uint8_t itemAttribute;
    uint8_t itemRefine;
    uint8_t type;
    bool read;
    bool itemIdentify;
};
#endif  // GUI_MAILMESSAGE_H
