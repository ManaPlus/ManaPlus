/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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
# include "msvc/config.h"
#elif defined(HAVE_CONFIG_H)
#include "config.h"
#endif  // _MSC_VER

#include "fs/paths.h"
#include "fs/virtfs.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#ifdef USE_X11
#include "fs/files.h"
#endif  // USE_X11

#ifdef __native_client__
#include <limits.h>
#define realpath(N, R) strcpy(R, N)
#endif  // __native_client__

#ifdef WIN32
#include "fs/specialfolder.h"
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#elif defined __OpenBSD__
#include <limits>
#elif defined __native_client__
#include <limits.h>
#endif  // WIN32

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif  // WIN32

#ifdef ANDROID
#ifdef USE_SDL2
#include <SDL_system.h>
#endif  // USE_SDL2
#endif  // ANDROID

#include "debug.h"

namespace
{
    std::string mPackageDir;
}  // namespace

std::string getRealPath(const std::string &str)
{
#if defined(__OpenBSD__) || defined(__ANDROID__) || defined(__native_client__)
    char *realPath = reinterpret_cast<char*>(calloc(PATH_MAX, sizeof(char)));
    if (!realPath)
        return "";
    realpath(str.c_str(), realPath);
#else  // defined(__OpenBSD__) || defined(__ANDROID__) ||
       // defined(__native_client__)

    char *realPath = realpath(str.c_str(), nullptr);
    if (!realPath)
        return "";
#endif  // defined(__OpenBSD__) || defined(__ANDROID__) ||
        // defined(__native_client__)

    std::string path = realPath;
    free(realPath);
    return path;
}

bool isRealPath(const std::string &str)
{
    return str == getRealPath(str);
}

bool checkPath(const std::string &path)
{
    if (path.empty())
        return true;
    return path.find("../") == std::string::npos
        && path.find("..\\") == std::string::npos
        && path.find("/..") == std::string::npos
        && path.find("\\..") == std::string::npos;
}

void prepareFsPath(std::string &path)
{
//    std::string path2 = path;
    sanitizePath(path);
// can be enabled for debugging
//    if (path != path2)
//    {
//        reportAlways("Path can be improved: '%s' -> '%s'",
//            path2.c_str(),
//            path0.c_str());
//    }
}

std::string &fixDirSeparators(std::string &str)
{
    if (dirSeparator[0] == '/')
        return str;

    return replaceAll(str, "/", "\\");
}

std::string removeLast(const std::string &str)
{
    size_t pos2 = str.rfind('/');
    const size_t pos3 = str.rfind('\\');
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

#else  // WIN32

std::string getSelfName()
{
    return "";
}

#endif  // WIN32

std::string getPicturesDir()
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
    {
        file = std::string(VirtFs::getUserDir()).append(
            "/.config/user-dirs.dirs");
    }
    else
    {
        file = std::string(xdg).append("/user-dirs.dirs");
    }

    StringVect arr;
    Files::loadTextFileLocal(file, arr);
    FOR_EACH (StringVectCIter, it, arr)
    {
        std::string str = *it;
        if (findCutFirst(str, "XDG_PICTURES_DIR=\""))
        {
            str = str.substr(0, str.size() - 1);
            // use hack to replace $HOME var.
            // if in string other vars, fallback to default path
            replaceAll(str, "$HOME/", VirtFs::getUserDir());
            str = getRealPath(str);
            if (str.empty())
                str = std::string(VirtFs::getUserDir()).append("Desktop");
            return str;
        }
    }

    return std::string(VirtFs::getUserDir()).append("Desktop");
#else  // WIN32

    return std::string(VirtFs::getUserDir()).append("Desktop");
#endif  // WIN32
}

std::string getHomePath()
{
#ifdef WIN32
    return getSpecialFolderLocation(CSIDL_LOCAL_APPDATA);
#else
    const char *path = getenv("HOME");
    if (path == nullptr)
    {
        const uid_t uid = getuid();
        const struct passwd *const pw = getpwuid(uid);
        if (pw != nullptr &&
            pw->pw_dir != nullptr)
        {
            path = pw->pw_dir;
        }
        if (path == nullptr)
            return "/";
    }
    std::string dir = path;
    if (findLast(dir, "/") == false)
        dir += "/";
    return dir;
#endif  // WIN32
}

#ifdef ANDROID
std::string getSdStoragePath()
{
    return getenv("DATADIR2");
}
#endif  // ANDROID

std::string getPackageDir()
{
    return mPackageDir;
}

void setPackageDir(const std::string &dir)
{
    mPackageDir = dir;
}
