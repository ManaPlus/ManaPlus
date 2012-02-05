/*
 *  Color database
 *  Copyright (C) 2008  Aethyra Development Team
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

#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include <map>
#include <string>

/**
 * Color information database.
 */
namespace ColorDB
{
    class ItemColor
    {
        public:
            ItemColor():
                id(0),
                name(""),
                color("")
            { }

            ItemColor(int id0, std::string name0, std::string color0)
            {
                this->id = id0;
                this->name = name0;
                this->color = color0;
            }

            int id;
            std::string name;
            std::string color;
    };

    /**
     * Loads the color data from <code>colors.xml</code>.
     */
    void load();

    /**
     * Loads the color data from <code>colors.xml</code>.
     */
    void loadHair();

    void loadColorLists();

    /**
     * Clear the color data
     */
    void unload();

    std::string &getHairColor(int id);

    std::string &getHairColorName(int id);

    int getHairSize();

    std::map <int, ItemColor> *getColorsList(std::string name);

    // Color DB
    typedef std::map<int, ItemColor> Colors;
    typedef Colors::iterator ColorIterator;
    typedef std::map <std::string, std::map <int, ItemColor> > ColorLists;
    typedef ColorLists::iterator ColorListsIterator;
}

#endif
