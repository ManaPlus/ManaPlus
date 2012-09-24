/*
 *  Emote database
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef EMOTE_DB_H
#define EMOTE_DB_H

#include "utils/stringvector.h"

#include <list>
#include <map>

#include "localconsts.h"

class AnimatedSprite;

struct EmoteSprite final
{
    const AnimatedSprite *sprite;
    std::string name;
};

struct EmoteInfo final
{
    std::list<EmoteSprite*> sprites;
    StringVect particles;
};

typedef std::map<int, EmoteInfo*> EmoteInfos;
typedef std::map<int, int> EmoteToEmote;

/**
 * Emote information database.
 */
namespace EmoteDB
{
    void load();

    void unload();

    const EmoteInfo *get(const int id, const bool allowNull = false);

    const AnimatedSprite *getAnimation(const int id,
                                       const bool allowNull = false);

    const AnimatedSprite *getAnimation2(int id, const bool allowNull = false);

    const EmoteSprite *getSprite(const int id, const bool allowNull = false);

    const int &getLast();

    typedef EmoteInfos::iterator EmoteInfosIterator;
}

#endif
