/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#include "resources/memorycounter.h"

#include "resources/memorymanager.h"

#include "debug.h"

MemoryCounter::MemoryCounter()
{
}

int MemoryCounter::calcMemoryLocal() const
{
    return 0;
}

int MemoryCounter::calcMemory(const int level) const
{
    const int sumLocal = calcMemoryLocal();
    const int sumChilds = calcMemoryChilds(level);
    memoryManager.printMemory(getCounterName(),
        level,
        sumLocal,
        sumChilds);
    return sumLocal + sumChilds;
}

int MemoryCounter::calcMemoryChilds(const int level A_UNUSED) const
{
    return 0;
}
