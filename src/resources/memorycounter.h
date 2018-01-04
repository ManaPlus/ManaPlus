/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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

#ifndef RESOURCES_MEMORYCOUNTER_H
#define RESOURCES_MEMORYCOUNTER_H

#include <string>

#include "localconsts.h"

class MemoryCounter notfinal
{
    public:
        MemoryCounter();

        A_DEFAULT_COPY(MemoryCounter)

PRAGMACLANG(GCC diagnostic push)
PRAGMACLANG(GCC diagnostic ignored "-Wdeprecated")
        virtual ~MemoryCounter()
        { }
PRAGMACLANG(GCC diagnostic pop)

        int calcMemory(const int level) const;

        virtual int calcMemoryLocal() const;

        virtual int calcMemoryChilds(const int level) const;

        virtual std::string getCounterName() const
        { return "unknown"; }
};

#endif  // RESOURCES_MEMORYCOUNTER_H
