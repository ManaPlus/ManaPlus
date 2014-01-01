/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "utils/checkutils.h"

#include "logger.h"

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
