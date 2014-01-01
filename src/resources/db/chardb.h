/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef RESOURCES_DB_CHARDB_H
#define RESOURCES_DB_CHARDB_H

#include "utils/xml.h"

#include <vector>

/**
 * Char information database.
 */
namespace CharDB
{
    /**
     * Loads the chars data.
     */
    void load();

    /**
     * Clear the chars data
     */
    void unload();

    void loadMinMax(const XmlNodePtr node,
                    unsigned *restrict const min,
                    unsigned *restrict const max);

    unsigned getMinHairColor() A_WARN_UNUSED;

    unsigned getMaxHairColor() A_WARN_UNUSED;

    unsigned getMinHairStyle() A_WARN_UNUSED;

    unsigned getMaxHairStyle() A_WARN_UNUSED;

    unsigned getMinStat() A_WARN_UNUSED;

    unsigned getMaxStat() A_WARN_UNUSED;

    unsigned getSumStat() A_WARN_UNUSED;

    unsigned getMinLook() A_WARN_UNUSED;

    unsigned getMaxLook() A_WARN_UNUSED;

    const std::vector<int> &getDefaultItems() A_WARN_UNUSED;
}  // namespace CharDB

#endif  // RESOURCES_DB_CHARDB_H
