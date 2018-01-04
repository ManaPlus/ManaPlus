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

#include "utils/mrand.h"

#include <cstdlib>

#include "debug.h"

namespace
{
    constexpr const int randNumbers = 1024;
    int mPos = 0;
    int mRand[randNumbers];
}  // namespace

void initRand()
{
    for (int f = 0; f < randNumbers; f ++)
        mRand[f] = std::rand();
}

int mrand()
{
    if (mPos >= randNumbers)
        mPos = 0;
    return mRand[mPos++];
}
