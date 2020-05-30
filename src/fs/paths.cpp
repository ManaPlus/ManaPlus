/*
 *  The ManaPlus Client
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

#ifdef _MSC_VER
# include "msvc/config.h"
#elif defined(HAVE_CONFIG_H)
#include "config.h"
#endif  // _MSC_VER

#include "fs/paths.h"

#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#ifdef USE_X11
#include "fs/files.h"

#include "utils/foreach.h"
#endif  // USE_X11

#if defined(__native_client__) || defined(__SWITCH__)
#define realpath(N, R) strcpy(R, N)
#endif  // __native_client__

#ifdef WIN32
#include "fs/specialfolder.h"
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#endif

#if defined __OpenBSD__
#include <limits>
#else
#include <climits>
#endif  // WIN32

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif  // WIN32

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef ANDROID
#ifdef USE_SDL2
#include <SDL_system.h>
#endif  // USE_SDL2
#endif  // ANDROID
PRAGMA48(GCC diagnostic pop)

#if defined(__native_client__) || defined(__SWITCH__)
#ifndef SSIZE_MAX
#define SSIZE_MAX INT_MAX
#endif
#endif  // __native_client__

#include "debug.h"

namespace
{
    std::string mPackageDir;
}  // namespace

std::string getRealPath(const std::string &str)
{
#if defined(__GLIBC__)
    if (str.find("(unreachable)") != std::string::npos)
        return "";
#endif  // defined(__GLIBC__)

    // possible fix for realpath overflow
    if (str.size() > SSIZE_MAX / 10)
    {
        return std::string();
    }
#if defined(__OpenBSD__) || defined(__ANDROID__) || \
    defined(__native_client__) || defined(__SWITCH__)
    char *realPath = reinterpret_cast<char*>(calloc(PATH_MAX, sizeof(char)));
    if (!realPath)
        return "";
    realpath(str.c_str(), realPath);
#else  // defined(__OpenBSD__) || defined(__ANDROID__) ||
       // defined(__native_client__)

    char *realPath = realpath(str.c_str(), nullptr);
    if (realPath == nullptr)
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
        && path.find("\\..") == std::string::npos
        && path.find("(unreachable)") == std::string::npos;
}

void prepareFsPath(std::string &path)
{
#ifdef DEBUGFS
    std::string path2 = path;
#endif
    sanitizePath(path);
// can be enabled for debugging
#ifdef DEBUGFS
    if (path != path2)
    {
        reportAlways("Path can be improved: '%s' -> '%s'",
            path2.c_str(),
            path.c_str())
    }
#endif
}

std::string &fixDirSeparators(std::string &str)
{
#ifdef WIN32
    return replaceAll(str, "/", "\\");
#else
    return str;
#endif
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

std::string getSelfName()
{
    char buf[257];
    const ssize_t sz = readlink("/proc/self/exe", buf, 256);
    if (sz > 0 && sz < 256)
    {
        buf[sz] = 0;
        return buf;
    }
    return "";
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
        file = pathJoin(VirtFs::getUserDir(),
            ".config/user-dirs.dirs");
    }
    else
    {
        file = pathJoin(xdg, "user-dirs.dirs");
    }

    if (Files::existsLocal(file))
    {
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
                    str = pathJoin(VirtFs::getUserDir(), "Desktop");
                return str;
            }
        }
    }
#endif  // WIN32

    return pathJoin(VirtFs::getUserDir(), "Desktop");
}

std::string getHomePath()
{
#if defined(UNITTESTS) && defined(UNITESTSDIR)
    std::string dir = UNITESTSDIR;
    if (findLast(dir, std::string(dirSeparator)) == false)
        dir += dirSeparator;
    return dir;
#else  // defined(UNITTESTS) && defined(UNITESTSDIR)
#ifdef WIN32
    return getSpecialFolderLocation(CSIDL_LOCAL_APPDATA);
#elif defined(__SWITCH__)
    return VirtFs::getBaseDir();
#else
    const char *path = getenv("HOME");
    if (path == nullptr)
    {
        const uid_t uid = getuid();
        const passwd *const pw = getpwuid(uid);
        if (pw != nullptr &&
            pw->pw_dir != nullptr)
        {
            path = pw->pw_dir;
        }
        if (path == nullptr)
            return dirSeparator;
    }
    std::string dir = path;
    if (findLast(dir, std::string(dirSeparator)) == false)
        dir += dirSeparator;
    return dir;
#endif  // WIN32
#endif  // defined(UNITTESTS) && defined(UNITESTSDIR)
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
