/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "resources/db/palettedb.h"

#include "logger.h"

#include "resources/dyecolor.h"
#include "resources/resourcemanager.h"

#include <map>

#include "debug.h"

namespace
{
    bool mLoaded = false;
    std::map<std::string, DyeColor> mColors;
    DyeColor mEmpty(0, 0, 0, 0);
}

void PaletteDB::load()
{
    if (mLoaded)
        unload();

    loadPalette();
}

void PaletteDB::loadPalette()
{
    mLoaded = true;
    StringVect lines;
    ResourceManager::loadTextFile("palette.gpl", lines);
    StringVectCIter it = lines.begin();
    if (it == lines.end())
    {
        logger->log("missing GIMP palette file");
        return;
    }
    if (*it != "GIMP Palette")
    {
        logger->log("wrong GIMP palette file");
        return;
    }
    ++ it;
    // skip header
    while (it != lines.end())
    {
        const std::string line = *it;
        if (!line.empty() && line[0] == '#')
            break;
        ++ it;
    }

    char name[101];

    // process colors and ignore commets
    while (it != lines.end())
    {
        const std::string line = *it;
        ++ it;

        if (line.empty() || line[0] == '#')
            continue;

        unsigned int r;
        unsigned int g;
        unsigned int b;

        if (sscanf(line.c_str(), "%10u %10u %10u\t%100s",
            &r, &g, &b, name) == 4)
        {
            mColors[name] = DyeColor(r, g, b);
        }
    }
}

void PaletteDB::unload()
{
    mColors.clear();
}

const DyeColor &PaletteDB::getColor(const std::string &name)
{
    std::map<std::string, DyeColor>::const_iterator it = mColors.find(name);
    if (it != mColors.end())
        return (*it).second;
    else
        return mEmpty;
}
