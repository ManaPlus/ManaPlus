/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#ifndef RESOURCES_DB_WEAPONSDB_H
#define RESOURCES_DB_WEAPONSDB_H

#include <vector>

#include "localconsts.h"

typedef std::vector<int> WeaponsInfos;
typedef WeaponsInfos::const_iterator WeaponsInfosIter;

namespace WeaponsDB
{
    void load();

    void unload();

    const WeaponsInfos &getBows();

    const WeaponsInfos &getSwords();

    const WeaponsInfos &getShields();
}  // namespace WeaponsDB

#endif  // RESOURCES_DB_WEAPONSDB_H
