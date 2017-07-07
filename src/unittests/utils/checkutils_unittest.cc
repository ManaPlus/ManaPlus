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

#include "unittests/unittests.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

namespace
{
    bool flag = false;
}  // namespace

static void testReturnFalseV(const bool val)
{
    flag = false;
    returnFalseVReal(val);
    flag = true;
}

static void testReturnTrueV(const bool val)
{
    flag = false;
    returnTrueVReal(val);
    flag = true;
}

static int testReturnFalse(const bool val)
{
    returnFalseReal(0, val);
    return 1;
}

static int testReturnTrue(const bool val)
{
    returnTrueReal(0, val);
    return 1;
}

static int testReturnNullptr(void *val)
{
    returnNullptrReal(0, val);
    return 1;
}

static void testReturnNullptrV(void *val)
{
    flag = false;
    returnNullptrVReal(val);
    flag = true;
}

static bool testFailAlways1()
{
    failAlways("test fail");
    return false;
}

static bool testFailAlways2()
{
    reportAlways("test fail");
    return false;
}

TEST_CASE("CheckUtils", "")
{
    logger = new Logger;

    SECTION("reportFalse")
    {
        REQUIRE(reportFalseReal(false) == false);
        REQUIRE(reportFalseReal(true) == true);
    }

    SECTION("reportTrue")
    {
        REQUIRE(reportTrueReal(false) == false);
        REQUIRE(reportTrueReal(true) == true);
    }

    SECTION("reportAlways")
    {
        reportAlwaysReal("test report");
    }

    SECTION("failFalse")
    {
PRAGMA4(GCC diagnostic push)
PRAGMA4(GCC diagnostic ignored "-Wunused-value")
        REQUIRE_THROWS(failFalse(false));
        REQUIRE(failFalse(true) == true);
        REQUIRE_THROWS(reportFalse(false));
        REQUIRE(reportFalse(true) == true);
PRAGMA4(GCC diagnostic pop)
    }

    SECTION("failTrue")
    {
PRAGMA4(GCC diagnostic push)
PRAGMA4(GCC diagnostic ignored "-Wunused-value")
        REQUIRE(failTrue(false) == false);
        REQUIRE_THROWS(failTrue(true));
        REQUIRE(reportTrue(false) == false);
        REQUIRE_THROWS(reportTrue(true));
PRAGMA4(GCC diagnostic pop)
    }

    SECTION("failAlways")
    {
PRAGMA4(GCC diagnostic push)
PRAGMA4(GCC diagnostic ignored "-Wunused-value")
        REQUIRE_THROWS(testFailAlways1());
        REQUIRE_THROWS(testFailAlways2());
PRAGMA4(GCC diagnostic pop)
    }

    SECTION("returnFalseV")
    {
        testReturnFalseV(false);
        REQUIRE(flag == false);
        testReturnFalseV(true);
        REQUIRE(flag == true);
    }

    SECTION("returnTrueV")
    {
        testReturnTrueV(false);
        REQUIRE(flag == true);
        testReturnTrueV(true);
        REQUIRE(flag == false);
    }

    SECTION("returnFalse")
    {
        REQUIRE(testReturnFalse(false) == 0);
        REQUIRE(testReturnFalse(true) == 1);
    }

    SECTION("returnTrue")
    {
        REQUIRE(testReturnTrue(false) == 1);
        REQUIRE(testReturnTrue(true) == 0);
    }

    SECTION("returnNullptr")
    {
        REQUIRE(testReturnNullptr(nullptr) == 0);
        REQUIRE(testReturnNullptr(reinterpret_cast<void*>(1)) == 1);
    }

    SECTION("returnNullptrV")
    {
        testReturnNullptrV(nullptr);
        REQUIRE(flag == false);
        testReturnNullptrV(reinterpret_cast<void*>(1));
        REQUIRE(flag == true);
    }

    delete2(logger);
}
