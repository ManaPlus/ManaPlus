/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef RESOURCES_QUESTITEMTEXT_H
#define RESOURCES_QUESTITEMTEXT_H

#include "enums/resources/questtype.h"

#include <string>

#include "localconsts.h"

struct QuestItemText final
{
    QuestItemText(const std::string &text0,
                  const QuestTypeT type0,
                  const std::string &str1,
                  const std::string &str2) :
        text(text0),
        type(type0),
        data1(str1),
        data2(str2)
    {
    }

    std::string text;
    QuestTypeT type;
    std::string data1;
    std::string data2;
};

#endif  // RESOURCES_QUESTITEMTEXT_H
