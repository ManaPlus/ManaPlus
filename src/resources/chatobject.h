/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "resources/image.h"

#include <string>

struct ChatObject final
{
    ChatObject() :
        ownerId(0),
        charId(0),
        maxUsers(0),
        currentUsers(0),
        type(0),
        title()
    {
    }

    A_DELETE_COPY(ChatObject)

    int ownerId;
    int charId;
    uint16_t maxUsers;
    uint16_t currentUsers;
    uint8_t type;
    std::string title;
};

#endif  // RESOURCES_CHATOBJECT_H
