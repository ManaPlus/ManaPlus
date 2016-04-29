/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#ifdef ENABLE_ASSERTS

#include "utils/checkutils.h"

#include "logger.h"

#if defined __linux__ || defined __linux
#include <execinfo.h>
#endif  // defined __linux__ || defined __linux

#include "debug.h"

bool reportFalseReal(const bool val, const char *const file,
                     const unsigned line)
{
    if (!val)
        logger->log("Debug: false value at %s:%u", file, line);
    return val;
}

bool reportTrueReal(const bool val, const char *const file,
                    const unsigned line)
{
    if (val)
        logger->log("Debug: true value at %s:%u", file, line);
    return val;
}

void reportStack(const char *const file,
                 const unsigned int line,
                 const char *const func,
                 const char *const name,
                 const char *const text)
{
#if defined __linux__ || defined __linux
    void *array[15];
    int size;
    char **strings;
    int i;
#endif  // defined __linux__ || defined __linux

    logger->log("--- %s --------------------------------------------",
        name);
    logger->log("%s:%u: '%s' in function `%s'",
        file,
        line,
        text,
        func);
#if defined __linux__ || defined __linux
    size = (int)backtrace(array, 15);
    strings = backtrace_symbols(array, size);
    for (i = 0; i < size; i++)
        logger->log1(strings[i]);
    free(strings);
#endif  // defined __linux__ || defined __linux

}

#endif  // ENABLE_ASSERTS
