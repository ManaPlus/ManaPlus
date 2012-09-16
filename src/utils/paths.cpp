/*
 *  The ManaPlus Client
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

#ifdef _MSC_VER
#  include "msvc/config.h"
#elif defined(HAVE_CONFIG_H)
#  include "config.h"
#endif

#include "utils/paths.h"

#include "utils/stringutils.h"

#include "resources/resourcemanager.h"

#include <string.h>
#include <cstdarg>
#include <cstdio>
#include <physfs.h>
#include <stdlib.h>

#ifdef WIN32
#include "utils/specialfolder.h"
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#elif defined __OpenBSD__
#include <limits.h>
#endif

#include "debug.h"

std::string getRealPath(const std::string &str)
{
    std::string path;
#if defined __OpenBSD__
    char *realPath = (char*)calloc(PATH_MAX, sizeof(char));
    realpath(str.c_str(), realPath);
#else
    char *realPath = realpath(str.c_str(), nullptr);
#endif
    if (!realPath)
        return "";
    path = realPath;
    free(realPath);
    return path;
}

bool isRealPath(const std::string &str)
{
    std::string path = getRealPath(str);
    return str == path;
}

bool checkPath(std::string path)
{
    if (path.empty())
        return true;
    return path.find("../") == std::string::npos
        && path.find("..\\") == std::string::npos
        && path.find("/..") == std::string::npos
        && path.find("\\..") == std::string::npos;
}

std::string &fixDirSeparators(std::string &str)
{
    if (*PHYSFS_getDirSeparator() == '/')
        return str;

    return replaceAll(str, "/", "\\");
}

std::string removeLast(std::string str)
{
    size_t pos2 = str.rfind("/");
    const size_t pos3 = str.rfind("\\");
    if (pos3 != std::string::npos)
    {
        if (pos2 == std::string::npos || pos3 > pos2)
            pos2 = pos3;
    }
    if (pos2 != std::string::npos)
        return str.substr(0, pos2);
    else
        return str;
}

#ifdef WIN32
std::string getSelfName()
{
    return "manaplus.exe";
}

#elif defined(__APPLE__)
std::string getSelfName()
{
    return "manaplus.exe";
}

#elif defined __linux__ || defined __linux
#include <unistd.h>

std::string getSelfName()
{
    char buf[257];
    const ssize_t sz = readlink("/proc/self/exe", buf, 256);
    if (sz > 0 && sz < 256)
    {
        buf[sz] = 0;
        return buf;
    }
    else
    {
        return "";
    }
}

#else
std::string getSelfName()
{
    return "";
}

#endif

std::string getDesktopDir()
{
#ifdef WIN32
    std::string dir = getSpecialFolderLocation(CSIDL_MYPICTURES);
    if (dir.empty())
        dir = getSpecialFolderLocation(CSIDL_DESKTOP);
    return dir;
#elif defined USE_X11
    char *xdg = getenv("XDG_CONFIG_HOME");
    std::string file;
    if (!xdg)
        file = std::string(PHYSFS_getUserDir()) + "/.config/user-dirs.dirs";
    else
        file = std::string(xdg) + "/user-dirs.dirs";

    StringVect arr = ResourceManager::loadTextFileLocal(file);
    for (StringVectCIter it = arr.begin(), it_end = arr.end();
        it != it_end; ++ it)
    {
        std::string str = *it;
        if (findCutFirst(str, "XDG_DESKTOP_DIR=\""))
        {
            str = str.substr(0, str.size() - 1);
            // use hack to replace $HOME var.
            // if in string other vars, fallback to default path
            replaceAll(str, "$HOME/", PHYSFS_getUserDir());
            str = getRealPath(str);
            if (str.empty())
                str = std::string(PHYSFS_getUserDir()) + "Desktop";
            return str;
        }
    }

    return std::string(PHYSFS_getUserDir()) + "Desktop";
#else
    return std::string(PHYSFS_getUserDir()) + "Desktop";
#endif
}
