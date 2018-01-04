/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#include "resources/chatobject.h"

#include "debug.h"

std::map<std::string, ChatObject*> ChatObject::chatNameMap;
std::map<int, ChatObject*> ChatObject::chatIdMap;

ChatObject::ChatObject() :
    ownerId(BeingId_zero),
    chatId(0),
    maxUsers(0),
    currentUsers(0),
    type(0),
    title()
{
}

ChatObject::~ChatObject()
{
    std::map<std::string, ChatObject*>::iterator it = chatNameMap.find(title);
    if (it != chatNameMap.end() && (*it).second == this)
        chatNameMap.erase(it);
    std::map<int, ChatObject*>::iterator it2 = chatIdMap.find(chatId);
    if (it2 != chatIdMap.end() && (*it2).second == this)
        chatIdMap.erase(it2);
}

void ChatObject::update()
{
    chatNameMap[title] = this;
    chatIdMap[chatId] = this;
}

ChatObject *ChatObject::findByName(const std::string &name)
{
    const std::map<std::string, ChatObject*>::iterator it =
        chatNameMap.find(name);
    if (it == chatNameMap.end())
        return nullptr;
    return (*it).second;
}

ChatObject *ChatObject::findById(const int id)
{
    const std::map<int, ChatObject*>::iterator it = chatIdMap.find(id);
    if (it == chatIdMap.end())
        return nullptr;
    return (*it).second;
}
