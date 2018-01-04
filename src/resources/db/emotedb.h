/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RESOURCES_DB_EMOTEDB_H
#define RESOURCES_DB_EMOTEDB_H

#include "enums/simpletypes/skiperror.h"

#include <map>
#include <string>

#include "localconsts.h"

struct EmoteInfo;
struct EmoteSprite;

typedef std::map<int, EmoteInfo*> EmoteInfos;
typedef std::map<int, int> EmoteToEmote;

/**
 * Emote information database.
 */
namespace EmoteDB
{
    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    void loadSpecialXmlFile(const std::string &fileName,
                            const SkipError skipError);

    void unload();

    const EmoteInfo *get(const int id,
                         const bool allowNull) A_WARN_UNUSED;

    const EmoteInfo *get2(int id,
                          const bool allowNull) A_WARN_UNUSED;

    const EmoteSprite *getSprite(const int id,
                                 const bool allowNull) A_WARN_UNUSED;

    const int &getLast() A_CONST A_WARN_UNUSED;

    int getIdByIndex(const int index);

    int size();

    typedef EmoteInfos::iterator EmoteInfosIterator;
}  // namespace EmoteDB

#endif  // RESOURCES_DB_EMOTEDB_H
