/*
 *  The ManaPlus Client
 *  Copyright (C) 2016  The ManaPlus Developers
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

#include "catch.hpp"

#include "logger.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("CheckUtils")
{
    logger = new Logger;

    SECTION("reportFalse")
    {
        REQUIRE(reportFalse(false) == false);
        REQUIRE(reportFalse(true) == true);
    }

    SECTION("reportTrue")
    {
        REQUIRE(reportTrue(false) == false);
        REQUIRE(reportTrue(true) == true);
    }

    SECTION("failFalse")
    {
        REQUIRE_THROWS(failFalse(false) == false);
        REQUIRE(failFalse(true) == true);
    }

    SECTION("failTrue")
    {
        REQUIRE(failTrue(false) == false);
        REQUIRE_THROWS(failTrue(true) == true);
    }

    delete2(logger);
}
