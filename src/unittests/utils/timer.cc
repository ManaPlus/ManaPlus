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

#include "unittests/unittests.h"

#include "const/utils/timer.h"

#include "utils/timer.h"

#include <climits>

#include "debug.h"

static const int MAX_TICK_VALUE = INT_MAX / 2;

TEST_CASE("timer const", "")
{
    REQUIRE(MILLISECONDS_IN_A_TICK != 0);
}

TEST_CASE("timer get_elapsed_time", "")
{
    tick_time = 0;
    REQUIRE(get_elapsed_time(0) == 0);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 1) == 1 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 2) == 2 * MILLISECONDS_IN_A_TICK);

    tick_time = 1;
    REQUIRE(get_elapsed_time(0) == 1 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(1) == 0 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 1) == 2 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 2) == 3 * MILLISECONDS_IN_A_TICK);

    tick_time = 10;
    REQUIRE(get_elapsed_time(0) == 10 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(10) == 0 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 1) ==
        11 * MILLISECONDS_IN_A_TICK);

    tick_time = 10000;
    REQUIRE(get_elapsed_time(0) == 10000 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(10) == 9990 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(10000) == 0 * MILLISECONDS_IN_A_TICK);
    REQUIRE(get_elapsed_time(MAX_TICK_VALUE - 1) ==
        10001 * MILLISECONDS_IN_A_TICK);
}

TEST_CASE("timer get_elapsed_time1", "")
{
    tick_time = 0;
    REQUIRE(get_elapsed_time1(0) == 0);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 1) == 1);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 2) == 2);

    tick_time = 1;
    REQUIRE(get_elapsed_time1(0) == 1);
    REQUIRE(get_elapsed_time1(1) == 0);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 1) == 2);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 2) == 3);

    tick_time = 10;
    REQUIRE(get_elapsed_time1(0) == 10);
    REQUIRE(get_elapsed_time1(10) == 0);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 1) == 11);

    tick_time = 10000;
    REQUIRE(get_elapsed_time1(0) == 10000);
    REQUIRE(get_elapsed_time1(10) == 9990);
    REQUIRE(get_elapsed_time1(10000) == 0);
    REQUIRE(get_elapsed_time1(MAX_TICK_VALUE - 1) == 10001);
}
