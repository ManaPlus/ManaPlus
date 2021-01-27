/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#include "fs/files.h"

#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("VirtFs1 getRealDir1", "")
{
    VirtFs::init(".");
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    const bool dir1 = VirtFs::mountDirSilent("data",
        Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent("../data",
        Append_false)) == true);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "../data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountDirSilent("data/test",
        Append_false);
    VirtFs::mountDirSilent("../data/test",
        Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "data" + sep + "test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            ".." + sep + "data" + sep + "test");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 getRealDir2", "")
{
    VirtFs::init(".");
    const std::string sep = dirSeparator;
    std::string name("data/test/test.zip");
    std::string prefix("data" + sep + "test" + sep);
    if (Files::existsLocal(name) == false)
        prefix = ".." + sep + prefix;
    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);

    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir/1") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir\\dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir///hide.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("dir\\\\brimmedhat.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountZip(prefix + "test.zip");

    REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png").empty());
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 getRealDir3", "")
{
    VirtFs::init(".");
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    const bool dir1 = VirtFs::mountDirSilent2("data",
        "test",
        Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent2("../data",
        "test",
        Append_false)) == true);
    REQUIRE(VirtFs::getRealDir("file1.txt").empty());
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("dir1") == "data");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("dir1") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountDirSilent2("data/test",
        "dir2",
        Append_false);
    VirtFs::mountDirSilent2("../data/test",
        "dir2",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir").empty());
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") == "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") ==
            ".." + sep + "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::deinit();
}
