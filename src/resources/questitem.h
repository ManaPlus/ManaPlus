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

#ifndef RESOURCES_QUESTITEM_H
#define RESOURCES_QUESTITEM_H

#include "resources/questitemtext.h"

#include "utils/vector.h"

#include <set>

#include "localconsts.h"

struct QuestItem final
{
    QuestItem() :
        var(0),
        name(),
        group(),
        incomplete(),
        complete(),
        texts(),
        completeFlag(-1),
        broken(false)
    {
    }

    A_DELETE_COPY(QuestItem)

    int var;
    std::string name;
    std::string group;
    std::set<int> incomplete;
    std::set<int> complete;
    STD_VECTOR<QuestItemText> texts;
    int completeFlag;
    bool broken;
};

#endif  // RESOURCES_QUESTITEM_H
