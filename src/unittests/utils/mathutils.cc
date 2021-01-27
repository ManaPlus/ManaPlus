/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "utils/mathutils.h"

#include "debug.h"

TEST_CASE("MathUtils powerOfTwo", "")
{
    REQUIRE(powerOfTwo(0) == 1);
    REQUIRE(powerOfTwo(1) == 1);
    REQUIRE(powerOfTwo(2) == 2);
    REQUIRE(powerOfTwo(3) == 4);
    REQUIRE(powerOfTwo(4) == 4);
    REQUIRE(powerOfTwo(5) == 8);
    REQUIRE(powerOfTwo(6) == 8);
    REQUIRE(powerOfTwo(7) == 8);
    REQUIRE(powerOfTwo(8) == 8);
    REQUIRE(powerOfTwo(9) == 16);
    REQUIRE(powerOfTwo(10) == 16);
    REQUIRE(powerOfTwo(11) == 16);
    REQUIRE(powerOfTwo(12) == 16);
    REQUIRE(powerOfTwo(13) == 16);
    REQUIRE(powerOfTwo(14) == 16);
    REQUIRE(powerOfTwo(15) == 16);
    REQUIRE(powerOfTwo(16) == 16);
    REQUIRE(powerOfTwo(17) == 32);
    REQUIRE(powerOfTwo(18) == 32);
    REQUIRE(powerOfTwo(19) == 32);
    REQUIRE(powerOfTwo(20) == 32);
    REQUIRE(powerOfTwo(21) == 32);
    REQUIRE(powerOfTwo(22) == 32);
    REQUIRE(powerOfTwo(23) == 32);
    REQUIRE(powerOfTwo(24) == 32);
    REQUIRE(powerOfTwo(25) == 32);
    REQUIRE(powerOfTwo(26) == 32);
    REQUIRE(powerOfTwo(27) == 32);
    REQUIRE(powerOfTwo(28) == 32);
    REQUIRE(powerOfTwo(29) == 32);
    REQUIRE(powerOfTwo(30) == 32);
    REQUIRE(powerOfTwo(31) == 32);
    REQUIRE(powerOfTwo(32) == 32);
    REQUIRE(powerOfTwo(33) == 64);
    REQUIRE(powerOfTwo(34) == 64);
    REQUIRE(powerOfTwo(35) == 64);
    REQUIRE(powerOfTwo(36) == 64);
    REQUIRE(powerOfTwo(37) == 64);
    REQUIRE(powerOfTwo(38) == 64);
    REQUIRE(powerOfTwo(39) == 64);
    REQUIRE(powerOfTwo(41) == 64);
    REQUIRE(powerOfTwo(42) == 64);
    REQUIRE(powerOfTwo(43) == 64);
    REQUIRE(powerOfTwo(44) == 64);
    REQUIRE(powerOfTwo(45) == 64);
    REQUIRE(powerOfTwo(46) == 64);
    REQUIRE(powerOfTwo(47) == 64);
    REQUIRE(powerOfTwo(48) == 64);
    REQUIRE(powerOfTwo(49) == 64);
    REQUIRE(powerOfTwo(50) == 64);
    REQUIRE(powerOfTwo(51) == 64);
    REQUIRE(powerOfTwo(52) == 64);
    REQUIRE(powerOfTwo(53) == 64);
    REQUIRE(powerOfTwo(54) == 64);
    REQUIRE(powerOfTwo(55) == 64);
    REQUIRE(powerOfTwo(56) == 64);
    REQUIRE(powerOfTwo(57) == 64);
    REQUIRE(powerOfTwo(58) == 64);
    REQUIRE(powerOfTwo(59) == 64);
    REQUIRE(powerOfTwo(60) == 64);
    REQUIRE(powerOfTwo(61) == 64);
    REQUIRE(powerOfTwo(62) == 64);
    REQUIRE(powerOfTwo(63) == 64);
    REQUIRE(powerOfTwo(64) == 64);
    REQUIRE(powerOfTwo(65) == 128);

    REQUIRE(powerOfTwo(1000000) == 1048576);
}

TEST_CASE("MathUtils tests fastSqrtInt", "")
{
    for (int f = 0; f < 1005; f ++)
        REQUIRE(fastSqrtInt(f) == CAST_S32(sqrt(f)));
}
