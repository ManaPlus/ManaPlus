/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_DB_UNITSDB_H
#define RESOURCES_DB_UNITSDB_H

#include "enums/simpletypes/skiperror.h"

#include <string>

#include "localconsts.h"

namespace UnitsDb
{
    /**
    * Loads and parses the units.xml file (if found).
    */
    void load();

    void unload();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    /**
    * Formats the given number in the correct currency format.
    */
    std::string formatCurrency(const int value) A_WARN_UNUSED;

    std::string formatCurrency64(const int64_t value) A_WARN_UNUSED;

    /**
    * Formats the given number in the correct currency format.
    */
    std::string formatCurrency(std::string name,
                               const int value) A_WARN_UNUSED;

    std::string formatCurrency64(std::string name,
                                 const int64_t value) A_WARN_UNUSED;

    /**
    * Formats the given number in the correct weight/mass format.
    */
    std::string formatWeight(const int value) A_WARN_UNUSED;

    bool existsCurrency(const std::string &name) A_WARN_UNUSED;
}  // namespace UnitsDb

#endif  // RESOURCES_DB_UNITSDB_H
