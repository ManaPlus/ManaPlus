/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_WALLPAPER_H
#define RESOURCES_WALLPAPER_H

#include <string>

#include "localconsts.h"

/**
 * Handles organizing and choosing of wallpapers.
 */
class Wallpaper final
{
    public:
        A_DELETE_COPY(Wallpaper)

        /**
         * Reads the folder that contains wallpapers and organizes the
         * wallpapers found by area, width, and height.
         */
        static void loadWallpapers();

        /**
         * Returns the largest wallpaper for the given resolution, or the
         * default wallpaper if none are found.
         *
         * @param width the desired width
         * @param height the desired height
         * @return the file to use, or empty if no wallpapers are useable
         */
        static std::string getWallpaper(const int width,
                                        const int height) A_WARN_UNUSED;
};

#endif  // RESOURCES_WALLPAPER_H
