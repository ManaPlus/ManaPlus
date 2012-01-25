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

#include "utils/paths.h"

#include <string.h>
#include <cstdarg>
#include <cstdio>

#include <stdlib.h>

#ifdef WIN32
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
