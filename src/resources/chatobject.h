/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef RESOURCES_CHATOBJECT_H
#define RESOURCES_CHATOBJECT_H

#include "enums/simpletypes/beingid.h"

#include <map>
#include <string>

#include "localconsts.h"

struct ChatObject final
{
    ChatObject();

    ~ChatObject();

    A_DELETE_COPY(ChatObject)

    void update();

    static ChatObject *findByName(const std::string &name);

    static ChatObject *findById(const int id);

    BeingId ownerId;
    int chatId;
    uint16_t maxUsers;
    uint16_t currentUsers;
    uint8_t type;
    std::string title;

    static std::map<std::string, ChatObject*> chatNameMap;
    static std::map<int, ChatObject*> chatIdMap;
};

#endif  // RESOURCES_CHATOBJECT_H
