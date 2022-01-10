/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_BASICSTAT_H
#define RESOURCES_BASICSTAT_H

#include "enums/being/attributes.h"

#include <string>

#include "localconsts.h"

struct BasicStat final
{
    BasicStat(const AttributesT attr0,
              const std::string &tag0,
              const std::string &name0) :
        tag(tag0),
        name(name0),
        attr(attr0)
    {}

    A_DEFAULT_COPY(BasicStat)

    std::string tag;
    std::string name;
    AttributesT attr;
};

#endif  // RESOURCES_BASICSTAT_H
