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

#include "fs/files.h"

#include "fs/virtfs/fs.h"
#include "fs/virtfs/zipreader.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("throw VirtFs1 exists1", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent("data/",
        Append_false);
    VirtFs::mountDirSilent("..\\data",
        Append_false);

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

    if (dir1 == true)
        VirtFs::unmountDirSilent("data/test");
    else
        VirtFs::unmountDirSilent("../data/test");

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("throw VirtFs1 exists2", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent2("data/",
        "test",
        Append_false);
    VirtFs::mountDirSilent2("..\\data",
        "test",
        Append_false);

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

    if (dir1 == true)
        VirtFs::unmountDirSilent2("data/test", "dir2");
    else
        VirtFs::unmountDirSilent2("../data/test", "dir2");

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("throw Zip readCompressedFile", "")
{
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("empty")
    {
        REQUIRE_THROWS(VirtFs::ZipReader::readCompressedFile(nullptr));
    }

    delete2(logger);
}

TEST_CASE("throw Zip readFile", "")
{
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("empty")
    {
        REQUIRE_THROWS(VirtFs::ZipReader::readFile(nullptr));
    }

    delete2(logger);
}

TEST_CASE("throw VirtFs1 unmount", "")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    const std::string sep = dirSeparator;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("simple 1")
    {
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1/"));
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir2//dir3",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir3",
            Append_false));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::unmountDir("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2/dir3" + sep));
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
    }

    SECTION("subDir 1")
    {
        REQUIRE_THROWS(VirtFs::unmountDir2("dir1", "dir1"));
        REQUIRE_THROWS(VirtFs::unmountDir2("dir1/", "dir1/"));
    }

    SECTION("subDir 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir2("dir1", "dir2"));
    }

    SECTION("subDir 3")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir2//dir3",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir3",
            "dir4",
            Append_false));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE_THROWS(VirtFs::unmountDir2("dir1", "dir1"));
        REQUIRE(VirtFs::unmountDir2("dir1", "dir2"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::unmountDir("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2/dir3" + sep));
    }

    SECTION("subDir 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::unmountDir2("dir1", "dir2"));
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir3",
            Append_true));
    }

    VirtFs::deinit();
    delete2(logger);
}
