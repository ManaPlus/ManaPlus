/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_EMOTEINFO_H
#define RESOURCES_EMOTEINFO_H

#include "utils/stringvector.h"

#include <list>

#include "localconsts.h"

struct EmoteSprite;

struct EmoteInfo final
{
    EmoteInfo() :
        sprites(),
        particles(),
        time(400),
        effectId(-1)
    { }

    A_DELETE_COPY(EmoteInfo)

    std::list<EmoteSprite*> sprites;
    StringVect particles;
    int time;
    int effectId;
};

#endif  // RESOURCES_EMOTEINFO_H
