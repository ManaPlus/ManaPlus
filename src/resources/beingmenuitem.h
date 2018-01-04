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

#ifndef RESOURCES_BEINGMENUITEM_H
#define RESOURCES_BEINGMENUITEM_H

#include <string>

#include "localconsts.h"

struct BeingMenuItem final
{
    BeingMenuItem(const std::string &name0,
                  const std::string &command0) :
        name(name0),
        command(command0)
    {
    }

    A_DEFAULT_COPY(BeingMenuItem)

    std::string name;
    std::string command;
};

#endif  // RESOURCES_BEINGMENUITEM_H
