/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "fs/mkdir.h"

#include "utils/cast.h"

#if defined WIN32
#include <limits.h>
#include <windows.h>
#endif  // defined WIN32

#include <sys/stat.h>

#include "debug.h"

#if defined WIN32
int mkdir_r(const char *const pathname)
{
    if (!pathname)
        return -1;

    char tmp[PATH_MAX];
    char tmp2[PATH_MAX];
    char *p;

    if (strlen(pathname) >= PATH_MAX - 2)
        return -1;

    strncpy(tmp, pathname, sizeof(tmp) - 1);
    tmp[PATH_MAX - 1] = '\0';

    const int len = CAST_S32(strlen(tmp));

    if (len < 1 || len >= INT_MAX)
        return -1;

    // terminate the pathname with '/'
    if (tmp[len - 1] != '/')
    {
        tmp[len] = '/';
        tmp[len + 1] = '\0';
    }

    for (p = tmp; *p; p++)
    {
        if (*p == '/' || *p == '\\')
        {
            *p = '\0';
            // ignore a slash at the beginning of a path
            if (tmp[0] == 0)
            {
                *p = '/';
                continue;
            }

            strcpy(tmp2, tmp);
            char *p2 = tmp2 + strlen(tmp2) - 1;
            if (*p2 == '/' || *p2 == '\\')
                *p2 = 0;
            // check if the name already exists, but not as directory
            struct stat statbuf;
            if (!stat(tmp2, &statbuf))
            {
                if (S_ISDIR(statbuf.st_mode))
                {
                    *p = '/';
                    continue;
                }
                else
                    return -1;
            }

            if (!CreateDirectory(tmp2, nullptr))
            {
                // hack, hack. just assume that x: might be a drive
                // letter, and try again
                if (!(strlen(tmp2) == 2 && !strcmp(tmp2 + 1, ":")))
                    return -1;
            }

            *p = '/';
        }
    }
    return 0;
}
#else  // WIN32

/// Create a directory, making leading components first if necessary
int mkdir_r(const char *const pathname)
{
    if (pathname == nullptr)
        return -1;

    const size_t len = CAST_SIZE(strlen(pathname));
    char *tmp = new char[len + 2];
    char *p = nullptr;

    strcpy(tmp, pathname);

    // terminate the pathname with '/'
    if (tmp[len - 1] != '/')
    {
        tmp[len] = '/';
        tmp[len + 1] = '\0';
    }

    for (p = tmp; *p != 0; p++)
    {
        if (*p == '/')
        {
            *p = '\0';
            // ignore a slash at the beginning of a path
            if (tmp[0] == 0)
            {
                *p = '/';
                continue;
            }

            // check if the name already exists, but not as directory
            struct stat statbuf;
            if (stat(tmp, &statbuf) == 0)
            {
                if (S_ISDIR(statbuf.st_mode))
                {
                    *p = '/';
                    continue;
                }
                else
                {
                    delete []tmp;
                    return -1;
                }
            }

            if (mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
            {
                delete []tmp;
                return -1;
            }

            *p = '/';
        }
    }
    delete []tmp;
    return 0;
}
#endif  // WIN32
