/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Fate <fate.tmw@googlemail.com>
 *  Copyright (C) 2008  Chuck Miller <shadowmil@gmail.com>
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

#ifndef RESOURCES_EFFECTDESCRIPTION_H
#define RESOURCES_EFFECTDESCRIPTION_H

#include <string>

#include "localconsts.h"

struct EffectDescription final
{
    EffectDescription(const int id0,
                      const std::string &gfx0,
                      const std::string &sfx0,
                      const std::string &sprite0) :
        id(id0),
        gfx(gfx0),
        sfx(sfx0),
        sprite(sprite0)
    { }

    A_DEFAULT_COPY(EffectDescription)

    int id;
    std::string gfx;
    std::string sfx;
    std::string sprite;
};
#endif  // RESOURCES_EFFECTDESCRIPTION_H
