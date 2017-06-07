/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "test/unittests.h"

#include "utils/parameters.h"

#include "debug.h"

TEST_CASE("parameters basic 1", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "", ",", '\"') == true);
    REQUIRE(pars.empty());
}

TEST_CASE("parameters basic 2", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "one,two, tree", ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "one");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters basic 3", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, ", ,,,", ",", '\"') == true);
    REQUIRE(pars.empty());
}

TEST_CASE("parameters basic 4", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "one,,two, tree", ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "one");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters escape 1", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\\\"", ",", '\"') == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters escape 2", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\\\", test", ",", '\"') == true);
    REQUIRE(pars.size() == 2);
    REQUIRE(pars[0] == "\"");
    REQUIRE(pars[1] == "test");
}

TEST_CASE("parameters escape 3", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "test,\\\"", ",", '\"') == true);
    REQUIRE(pars.size() == 2);
    REQUIRE(pars[0] == "test");
    REQUIRE(pars[1] == "\"");
}

TEST_CASE("parameters quote 1", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one\",,two, tree", ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "one");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 2", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\",,two, tree", ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0].empty());
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 3", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one test\",,two, tree", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "one test");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 4", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\\\"one test\\\"\",,two, tree", ",", '\"')
        == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "\"one test\"");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 5", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\\\"one \\\"test\\\"\",,two, tree",
        ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "\"one \"test\"");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 6", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one, test\",,two, tree", ",", '\"')
        == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "one, test");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 7", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\\\"one, test\\\"\",,two, tree",
        ",", '\"') == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "\"one, test\"");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 8", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\\\"\",,two, tree", ",", '\"')
        == true);
    REQUIRE(pars.size() == 3);
    REQUIRE(pars[0] == "\"");
    REQUIRE(pars[1] == "two");
    REQUIRE(pars[2] == "tree");
}

TEST_CASE("parameters quote 9", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"\\\",,two, tree", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"\",,two, tree");
}

TEST_CASE("parameters quote 10", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters quote 11", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\\\"", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters quote 12", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, ",\"", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters quote 13", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\",", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\",");
}

TEST_CASE("parameters quote 14", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\\\",", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters quote 15", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, ",\\\"", ",", '\"')
        == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters quote 16", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one test\"", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "one test");
}

TEST_CASE("parameters quote 17", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one, test\"", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "one, test");
}

TEST_CASE("parameters quote 18", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one\\\" test\"", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "one\" test");
}

TEST_CASE("parameters quote 19", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one\\\" ,test\"", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "one\" ,test");
}

TEST_CASE("parameters quote 20", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"one\\\" test,\"", ",", '\"') ==
        true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "one\" test,");
}

TEST_CASE("parameters complex 1", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"test\" \"line\"", ",", '\"') == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "test\" \"line");
}

TEST_CASE("parameters complex 2", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"test\", \"line\"", ",", '\"') == true);
    REQUIRE(pars.size() == 2);
    REQUIRE(pars[0] == "test");
    REQUIRE(pars[1] == "line");
}

TEST_CASE("parameters complex 3", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"test,\" \"line\"", ",", '\"') == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "test,\" \"line");
}

TEST_CASE("parameters broken 1", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, "\"", ",", '\"') == true);
    REQUIRE(pars.size() == 1);
    REQUIRE(pars[0] == "\"");
}

TEST_CASE("parameters broken 2", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars, ",", ",", '\"') == true);
    REQUIRE(pars.empty());
}

TEST_CASE("parameters broken 3", "")
{
    StringVect pars;
    REQUIRE(splitParameters(pars,
        ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,", ",", '\"') == true);
    REQUIRE(pars.empty());
}
