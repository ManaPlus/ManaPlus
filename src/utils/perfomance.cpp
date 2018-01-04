/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#include "localconsts.h"

#ifdef USE_PROFILER

#include "utils/perfomance.h"

#include "configuration.h"
#include "game.h"

#include "utils/timer.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <iostream>

#include "debug.h"

static const clockid_t clockType = CLOCK_MONOTONIC;

#define timeData ((static_cast<long long int>(time.tv_sec) * 1000000000LL \
    + static_cast<long long int>(time.tv_nsec)) / 1)

namespace Perfomance
{
    std::ofstream file;
    std::string temp;
    long long unsigned int startTime;

    void init(const std::string &path)
    {
        file.open(path.c_str(), std::ios_base::trunc);
        timespec time;
        clock_gettime(clockType, &time);
        temp.reserve(10000000U);
        startTime = timeData;
    }

    void clear()
    {
        if (file.is_open())
            file.close();
    }

    void start()
    {
        timespec time;
        clock_gettime(clockType, &time);
        temp.append(toString(timeData - startTime)).append(
            " __init__\n");
    }

    void blockStart(const std::string &name)
    {
        timespec time;
        clock_gettime(clockType, &time);
        temp.append(toString(timeData - startTime)).append(
            " start: ").append(name).append("\n");
    }

    void blockEnd(const std::string &name)
    {
        timespec time;
        clock_gettime(clockType, &time);
        temp.append(toString(timeData - startTime)).append(
            " end: ").append(name).append("\n");
    }

    void flush()
    {
        if (fps < 40)
            file << temp;
        temp.clear();
//        file.flush();
    }
}  // namespace Perfomance

#endif  // USE_PROFILER
