/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_DB_COLORDB_H
#define RESOURCES_DB_COLORDB_H

#include "enums/simpletypes/skiperror.h"

#include "resources/itemcolordata.h"

#include <map>

#include "localconsts.h"

/**
 * Color information database.
 */
namespace ColorDB
{
    /**
     * Loads the color data from <code>colors.xml</code>.
     */
    void load();

    /**
     * Loads the color data from <code>colors.xml</code>.
     */
    void loadHair(const std::string &fileName,
                  std::map<ItemColor, ItemColorData> &colors,
                  const SkipError skipError);

    void loadColorLists(const std::string &fileName,
                        const SkipError skipError);

    /**
     * Clear the color data
     */
    void unload();

    std::string &getHairColorName(const ItemColor id) A_WARN_UNUSED;

    int getHairSize() A_WARN_UNUSED;

    const std::map <ItemColor, ItemColorData> *getColorsList(const std::string
                                                             &name)
                                                             A_WARN_UNUSED;

    // Color DB
    typedef std::map<ItemColor, ItemColorData> Colors;
    typedef Colors::iterator ColorIterator;
    typedef std::map <std::string, std::map <ItemColor, ItemColorData> >
        ColorLists;
    typedef ColorLists::iterator ColorListsIterator;
}  // namespace ColorDB

#endif  // RESOURCES_DB_COLORDB_H
