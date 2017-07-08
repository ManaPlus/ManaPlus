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

#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("VirtFs1 exists1", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent("data/",
        Append_false);
    VirtFs::mountDirSilent("..\\data",
        Append_false);

    REQUIRE(VirtFs::exists("test"));
    REQUIRE(VirtFs::exists("test/"));
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1/"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test//units.xml") == true);
    REQUIRE(VirtFs::exists("test/\\units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    if (dir1 == true)
    {
        VirtFs::mountDir("data//test",
            Append_false);
    }
    else
    {
        VirtFs::mountDirSilent("..//data\\test",
            Append_false);
    }

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1\\"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    if (dir1 == true)
        VirtFs::unmountDirSilent("data/test");
    else
        VirtFs::unmountDirSilent("../data/test");

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 exists2", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent2("data/",
        "test",
        Append_false);
    VirtFs::mountDirSilent2("..\\data",
        "test",
        Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt") == false);
    REQUIRE(VirtFs::exists("file2.txt") == false);

    if (dir1 == true)
    {
        VirtFs::mountDir2("data//test",
            "dir2",
            Append_false);
    }
    else
    {
        VirtFs::mountDirSilent2("..//data\\test",
            "dir2",
            Append_false);
    }

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt"));
    REQUIRE(VirtFs::exists("file2.txt"));

    if (dir1 == true)
        VirtFs::unmountDirSilent2("data/test", "dir2");
    else
        VirtFs::unmountDirSilent2("../data/test", "dir2");

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt") == false);
    REQUIRE(VirtFs::exists("file2.txt") == false);

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}
