/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef UTILS_CPU_H
#define UTILS_CPU_H

#include "localconsts.h"

namespace Cpu
{
    enum
    {
        FEATURE_EMPTY = 0,
        FEATURE_MMX   = 1,
        FEATURE_SSE   = 2,
        FEATURE_SSE2  = 4,
        FEATURE_SSSE3 = 8,
        FEATURE_SSE4  = 16,
        FEATURE_SSE42 = 32
    };

    void detect();

    void printFlags();
}  // namespace CPU

#endif  // UTILS_CPU_H
