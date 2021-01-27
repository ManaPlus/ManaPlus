/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef RESOURCES_ITEMMENUITEM_H
#define RESOURCES_ITEMMENUITEM_H

#include <string>

#include "localconsts.h"

struct ItemMenuItem final
{
    ItemMenuItem(const std::string &name01,
                 const std::string &name02,
                 const std::string &command01,
                 const std::string &command02) :
        name1(name01),
        name2(name02),
        command1(command01),
        command2(command02)
    {
    }

    A_DEFAULT_COPY(ItemMenuItem)

    std::string name1;
    std::string name2;
    std::string command1;
    std::string command2;
};

#endif  // RESOURCES_ITEMMENUITEM_H
