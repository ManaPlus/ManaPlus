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

#ifdef USE_PHYSFS
#include "catch.hpp"

#include "fs/paths.h"
#include "fs/virtfs.h"
#include "fs/virtlist.h"

#include "fs/physfs/virtfsphys.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("VirtFsPhys dirSeparator")
{
    VirtFsPhys::initFuncs();
    REQUIRE(VirtFs::getDirSeparator() != nullptr);
    REQUIRE(VirtFsPhys::getDirSeparator() ==
        std::string(VirtFs::getDirSeparator()));
    VirtFsPhys::updateDirSeparator();
    REQUIRE(VirtFs::getDirSeparator() != nullptr);
    REQUIRE(VirtFsPhys::getDirSeparator() ==
        std::string(VirtFs::getDirSeparator()));
}

TEST_CASE("VirtFsPhys getBaseDir")
{
    VirtFsPhys::initFuncs();
    REQUIRE(VirtFsPhys::getBaseDir() != nullptr);
}

TEST_CASE("VirtFsPhys getUserDir")
{
    VirtFsPhys::initFuncs();
    REQUIRE(VirtFsPhys::getUserDir() != nullptr);
    REQUIRE(VirtFsPhys::getUserDir() == getHomePath());
}

TEST_CASE("VirtFsPhys exists")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    VirtFsPhys::mountDir("data", Append_false);
    VirtFsPhys::mountDir("../data", Append_false);

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == false);

    VirtFsPhys::mountDir("data/test", Append_false);
    VirtFsPhys::mountDir("../data/test", Append_false);

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == true);

    VirtFsPhys::unmountDir("data/test");
    VirtFsPhys::unmountDir("../data/test");

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == false);

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys exists2")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    VirtFsPhys::mountZip("data/test/test2.zip", Append_false);
    VirtFsPhys::mountZip("../data/test/test2.zip", Append_false);

    REQUIRE(VirtFsPhys::exists("test/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("test.txt") == true);
    REQUIRE(VirtFsPhys::exists("units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == true);

    VirtFsPhys::unmountZip("data/test/test2.zip");
    VirtFsPhys::unmountZip("../data/test/test2.zip");
    delete2(logger);
}

static void removeTemp(StringVect &restrict list)
{
    int cnt = 0;
    std::sort(list.begin(), list.end());

    FOR_EACH (StringVectIter, it, list)
    {
        if (*it != "serverlistplus.xml.part")
        {
            logger->log("file: %d %s",
                cnt,
                (*it).c_str());
            cnt ++;
        }
    }

    FOR_EACH (StringVectIter, it, list)
    {
        if (*it == "serverlistplus.xml.part")
        {
            list.erase(it);
            return;
        }
    }
}

TEST_CASE("VirtFsPhys enumerateFiles1")
{
    VirtFsPhys::initFuncs();
    logger = new Logger;

    VirtFsPhys::mountDir("data", Append_false);
    VirtFsPhys::mountDir("../data", Append_false);

    VirtList *list = nullptr;

    const int cnt1 = VirtFsPhys::exists("test/test2.txt") ? 28 : 27;
    const int cnt2 = 28;

    VirtFsPhys::permitLinks(false);
    list = VirtFsPhys::enumerateFiles("test");
    removeTemp(list->names);
    const size_t sz = list->names.size();
    REQUIRE(sz == cnt1);
    VirtFsPhys::freeList(list);

    VirtFsPhys::permitLinks(true);
    list = VirtFsPhys::enumerateFiles("test");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt2);
    VirtFsPhys::freeList(list);

    VirtFsPhys::permitLinks(false);
    list = VirtFsPhys::enumerateFiles("test");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt1);
    VirtFsPhys::freeList(list);

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys enumerateFiles2")
{
    VirtFsPhys::initFuncs();
    logger = new Logger;

    VirtFsPhys::mountDir("data/test/dir1",
        Append_false);
    VirtFsPhys::mountDir("../data/test/dir1",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFsPhys::enumerateFiles("/");
    REQUIRE(list->names.size() == 5);
    VirtFsPhys::freeList(list);

    VirtFsPhys::unmountDir("data/test/dir1");
    VirtFsPhys::unmountDir("../data/test/dir1");
    delete2(logger);
}

static bool inList(VirtList *list,
                   const std::string &name)
{
    FOR_EACH (StringVectCIter, it, list->names)
    {
        if (*it == name)
            return true;
    }
    return false;
}

TEST_CASE("VirtFsPhys enumerateFiles3")
{
    VirtFsPhys::initFuncs();
    logger = new Logger;

    VirtFsPhys::mountZip("data/test/test.zip",
        Append_false);
    VirtFsPhys::mountZip("../data/test/test.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFsPhys::enumerateFiles("/");
    REQUIRE(inList(list, "units.xml") == false);
    REQUIRE(inList(list, "test.txt") == false);
    VirtFsPhys::freeList(list);

    VirtFsPhys::unmountZip("data/test/test.zip");
    VirtFsPhys::unmountZip("../data/test/test.zip");
    delete2(logger);
}

TEST_CASE("VirtFsPhys enumerateFiles4")
{
    VirtFsPhys::initFuncs();
    logger = new Logger;

    VirtFsPhys::mountZip("data/test/test2.zip",
        Append_false);
    VirtFsPhys::mountZip("../data/test/test2.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFsPhys::enumerateFiles("/");
    REQUIRE(inList(list, "units.xml") == true);
    REQUIRE(inList(list, "test.txt") == true);
    VirtFsPhys::freeList(list);

    VirtFsPhys::unmountZip("data/test/test2.zip");
    VirtFsPhys::unmountZip("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFsPhys isDirectory")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    VirtFsPhys::mountDir("data", Append_false);
    VirtFsPhys::mountDir("../data", Append_false);

    REQUIRE(VirtFsPhys::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFsPhys::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test//units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/") == true);
    REQUIRE(VirtFsPhys::isDirectory("test//") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1") == true);
    REQUIRE(VirtFsPhys::isDirectory("test//dir1") == true);
    REQUIRE(VirtFsPhys::isDirectory("test//dir1/") == true);
    REQUIRE(VirtFsPhys::isDirectory("test//dir1//") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1/") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1//") == true);
    REQUIRE(VirtFsPhys::isDirectory("testQ") == false);
    REQUIRE(VirtFsPhys::isDirectory("testQ/") == false);
    REQUIRE(VirtFsPhys::isDirectory("testQ//") == false);

    VirtFsPhys::mountDir("data/test", Append_false);
    VirtFsPhys::mountDir("../data/test", Append_false);

    REQUIRE(VirtFsPhys::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFsPhys::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test") == true);
    REQUIRE(VirtFsPhys::isDirectory("testQ") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1") == true);

    VirtFsPhys::unmountDir("data/test");
    VirtFsPhys::unmountDir("../data/test");

    REQUIRE(VirtFsPhys::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml/") == false);
    REQUIRE(VirtFsPhys::isDirectory("test") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/") == true);
    REQUIRE(VirtFsPhys::isDirectory("testQ") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1") == true);

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys openRead")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    VirtFsPhys::mountDir("data", Append_false);
    VirtFsPhys::mountDir("../data", Append_false);

    VirtFile *file = nullptr;

    file = VirtFsPhys::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsPhys::close(file);
    file = VirtFsPhys::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("units.xml");
    REQUIRE(file == nullptr);
//    file = VirtFsPhys::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsPhys::mountDir("data/test", Append_false);
    VirtFsPhys::mountDir("../data/test", Append_false);

    file = VirtFsPhys::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsPhys::close(file);
    file = VirtFsPhys::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFsPhys::close(file);
//    file = VirtFsPhys::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsPhys::unmountDir("data/test");
    VirtFsPhys::unmountDir("../data/test");

    file = VirtFsPhys::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsPhys::close(file);
    file = VirtFsPhys::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("units.xml");
    REQUIRE(file == nullptr);
//    file = VirtFsPhys::openRead("test");
//    REQUIRE(file == nullptr);
    file = VirtFsPhys::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys mountZip")
{
    // +++ need implement
}

TEST_CASE("VirtFsPhys unmountZip")
{
    // +++ need implement
}

TEST_CASE("VirtFsPhys getRealDir")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    REQUIRE(VirtFsPhys::getRealDir(".") == "");
    REQUIRE(VirtFsPhys::getRealDir("..") == "");
    const bool dir1 = VirtFsPhys::mountDir("data", Append_false);
    REQUIRE((dir1 || VirtFsPhys::mountDir(
        "../data", Append_false)) == true);
    if (dir1 == true)
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "../data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "../data");
    }
    REQUIRE(VirtFsPhys::getRealDir("zzz") == "");

    VirtFsPhys::mountDir("data/test", Append_false);
    VirtFsPhys::mountDir("../data/test", Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFsPhys::getRealDir("test.txt") ==
            "data/test");
    }
    else
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "../data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFsPhys::getRealDir("test.txt") ==
            "../data/test");
    }
    REQUIRE(VirtFsPhys::getRealDir("zzz") == "");

    if (dir1 == true)
    {
        VirtFsPhys::mountZip("data/test/test.zip", Append_false);
        REQUIRE(VirtFsPhys::getRealDir("dir/brimmedhat.png") ==
            "data/test/test.zip");
        REQUIRE(VirtFsPhys::getRealDir("hide.png") == "data/test");
    }
    else
    {
        VirtFsPhys::mountZip("../data/test/test.zip", Append_false);
        REQUIRE(VirtFsPhys::getRealDir("dir/brimmedhat.png") ==
            "../data/test/test.zip");
        REQUIRE(VirtFsPhys::getRealDir("hide.png") == "../data/test");
    }

    VirtFsPhys::unmountDir("data/test");
    VirtFsPhys::unmountDir("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFsPhys::getRealDir("dir/hide.png") ==
            "data/test/test.zip");
    }
    else
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "../data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFsPhys::getRealDir("dir/hide.png") ==
            "../data/test/test.zip");
    }
    REQUIRE(VirtFsPhys::exists("dir/hide.png"));
    REQUIRE(VirtFsPhys::getRealDir("zzz") == "");

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    VirtFsPhys::unmountZip("data/test/test.zip");
    VirtFsPhys::unmountZip("../data/test/test.zip");
    delete2(logger);
}

TEST_CASE("VirtFsPhys read")
{
    VirtFsPhys::initFuncs();
    logger = new Logger();
    VirtFsPhys::mountDir("data", Append_false);
    VirtFsPhys::mountDir("../data", Append_false);

    VirtFile *file = VirtFsPhys::openRead("test/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFsPhys::fileLength(file) == 23);
    const int fileSize = VirtFsPhys::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFsPhys::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFsPhys::tell(file) == fileSize);
    REQUIRE(VirtFsPhys::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFsPhys::seek(file, 12) != 0);
    REQUIRE(VirtFsPhys::eof(file) == false);
    REQUIRE(VirtFsPhys::tell(file) == 12);
    REQUIRE(VirtFsPhys::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFsPhys::eof(file) == true);

    VirtFsPhys::close(file);
    free(buffer);

    VirtFsPhys::unmountDir("data");
    VirtFsPhys::unmountDir("../data");
    delete2(logger);
}
#endif  // USE_PHYSFS
