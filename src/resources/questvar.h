/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#ifndef RESOURCES_QUESTVAR_H
#define RESOURCES_QUESTVAR_H

#include <map>

#include "localconsts.h"

struct QuestVar final
{
    QuestVar() :
        var1(0),
        var2(0),
        var3(0),
        time1(0)
    {
    }

    QuestVar(const int v1,
             const int v2,
             const int v3,
             const int t) :
        var1(v1),
        var2(v2),
        var3(v3),
        time1(t)
    {
    }

    A_DEFAULT_COPY(QuestVar)

    int var1;
    int var2;
    int var3;
    int time1;
};

typedef std::map<int, QuestVar> NpcQuestVarMap;
typedef NpcQuestVarMap::const_iterator NpcQuestVarMapCIter;

#endif  // RESOURCES_QUESTVAR_H
