/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2018  The ManaPlus Developers
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

#include "unittests/unittests.h"

#include "logger.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_endian.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

namespace
{
    union ByteOrderData final
    {
        uint32_t dwordData;
        uint8_t byteData[4];
    } __attribute__((packed));
}  // namespace

TEST_CASE("endian test", "")
{
    ByteOrderData data;
    data.byteData[0] = 0x10;
    data.byteData[1] = 0x20;
    data.byteData[2] = 0x30;
    data.byteData[3] = 0x40;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    REQUIRE(data.dwordData == 0x10203040);
    logger->log("big endian detected");
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    REQUIRE(data.dwordData == 0x40302010);
    logger->log("little endian detected");
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
}
