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

#include "resources/wallpaper.h"

#include "configuration.h"

#include "fs/virtfs/fs.h"
#include "fs/virtfs/list.h"

#include "resources/wallpaperdata.h"

#include "utils/cast.h"
#include "utils/foreach.h"

#include <algorithm>

#ifdef WIN32
#include <sys/time.h>
#else  // WIN32
#include <ctime>
#endif  // WIN32

#include "debug.h"

static bool wallpaperCompare(const WallpaperData &a, const WallpaperData &b);

static STD_VECTOR<WallpaperData> wallpaperData;
static bool haveBackup;  // Is the backup (no size given) version available?

static std::string wallpaperPath;
static std::string wallpaperFile;

// Search for the wallpaper path values sequentially..
static void initDefaultWallpaperPaths()
{
    // Init the path
    wallpaperPath = branding.getStringValue("wallpapersPath");

    if (wallpaperPath.empty() || !VirtFs::isDirectory(wallpaperPath))
        wallpaperPath = paths.getValue("wallpapers", "");

    if (wallpaperPath.empty() || !VirtFs::isDirectory(wallpaperPath))
        wallpaperPath = "graphics/images/";

    // Init the default file
    wallpaperFile = branding.getStringValue("wallpaperFile");

    if (!wallpaperFile.empty() && !VirtFs::isDirectory(wallpaperFile))
        return;
    wallpaperFile = paths.getValue("wallpaperFile", "");

    if (wallpaperFile.empty() || VirtFs::isDirectory(wallpaperFile))
        wallpaperFile = "login_wallpaper.png";
}

static bool wallpaperCompare(const WallpaperData &a, const WallpaperData &b)
{
    const int aa = a.width * a.height;
    const int ab = b.width * b.height;

    return aa > ab ||
        (aa == ab && a.width > b.width);
}

void Wallpaper::loadWallpapers()
{
    wallpaperData.clear();
    initDefaultWallpaperPaths();
    VirtFs::List *const imgs = VirtFs::enumerateFiles(wallpaperPath);

    FOR_EACH (StringVectCIter, i, imgs->names)
    {
        // First, get the base filename of the image:
        std::string filename = *i;
        const std::string name = filename;
        // If the backup file is found, we tell it.
        if (filename.find(wallpaperFile) != std::string::npos)
            haveBackup = true;

        // If the image format is terminated by: "_<width>x<height>.png"
        // It is taken as a potential wallpaper.

        size_t separator = filename.rfind('_');
        filename = filename.substr(0, separator);

        // Check that the base filename doesn't have any '%' markers.
        separator = filename.find('%');
        if (separator == std::string::npos)
        {
            // Then, append the width and height search mask.
            filename.append("_%10dx%10d.png");

            int width;
            int height;
            if (sscanf(name.c_str(),
                filename.c_str(),
                &width,
                &height) == 2)
            {
                WallpaperData wp;
                wp.filename = wallpaperPath;
                wp.filename.append(*i);
                wp.width = width;
                wp.height = height;
                wallpaperData.push_back(wp);
            }
        }
    }

    VirtFs::freeList(imgs);
    std::sort(wallpaperData.begin(), wallpaperData.end(), &wallpaperCompare);
}

std::string Wallpaper::getWallpaper(const int width, const int height)
{
    WallpaperData wp;

    // Wallpaper filename container
    StringVect wallPaperVector;

    FOR_EACH (STD_VECTOR<WallpaperData>::const_iterator, iter, wallpaperData)
    {
        wp = *iter;
        if (wp.width <= width && wp.height <= height)
            wallPaperVector.push_back(wp.filename);
    }

    // If we've got more than one occurence of a valid wallpaper...
    if (!wallPaperVector.empty())
    {
        // Return randomly a wallpaper between vector[0] and
        // vector[vector.size() - 1]
        srand(CAST_U32(time(nullptr)));
        return wallPaperVector[CAST_S32(static_cast<double>(
            wallPaperVector.size()) * rand() / (RAND_MAX + 1.0))];
    }

    // Return the backup file if everything else failed...
    if (haveBackup)
        return pathJoin(wallpaperPath, wallpaperFile);

    // Return an empty string if everything else failed
    return std::string();
}
