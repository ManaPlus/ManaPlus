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

#include "catch.hpp"

#include "utils/checkutils.h"
#include "utils/virtfs.h"

#include "debug.h"

TEST_CASE("VirtFs dirSeparator")
{
    REQUIRE(dirSeparator != nullptr);
    REQUIRE(VirtFs::getDirSeparator() == std::string(dirSeparator));
    VirtFs::updateDirSeparator();
    REQUIRE(dirSeparator != nullptr);
    REQUIRE(VirtFs::getDirSeparator() == std::string(dirSeparator));
}

TEST_CASE("VirtFs getBaseDir")
{
    REQUIRE(VirtFs::getBaseDir() != nullptr);
}

TEST_CASE("VirtFs getUserDir")
{
    REQUIRE(VirtFs::getUserDir() != nullptr);
}

TEST_CASE("VirtFs exists")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
}

TEST_CASE("VirtFs enumerateFiles")
{
    // +++ need implement
}

TEST_CASE("VirtFs isDirectory")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
}

TEST_CASE("VirtFs openRead")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    VirtFile *file = nullptr;

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
//    file = VirtFs::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
//    file = VirtFs::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
//    file = VirtFs::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
}

TEST_CASE("VirtFs read")
{
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    VirtFile *file = VirtFs::openRead("test/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);

    VirtFs::close(file);
    free(buffer);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
}
