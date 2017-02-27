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

#include "fs/virtfs.h"
#include "fs/virtfsphys.h"
#include "fs/virtfstools.h"
#include "fs/virtlist.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("VirtFsPhys dirSeparator")
{
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
    REQUIRE(VirtFsPhys::getBaseDir() != nullptr);
}

TEST_CASE("VirtFsPhys getUserDir")
{
    REQUIRE(VirtFsPhys::getUserDir() != nullptr);
}

TEST_CASE("VirtFsPhys exists")
{
    logger = new Logger();
    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == false);

    VirtFsPhys::addDirToSearchPath("data/test", Append_false);
    VirtFsPhys::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == true);

    VirtFsPhys::removeDirFromSearchPath("data/test");
    VirtFsPhys::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFsPhys::exists("test/units.xml") == true);
    REQUIRE(VirtFsPhys::exists("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::exists("units.xml") == false);

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
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
    logger = new Logger;

    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

    VirtList *list = nullptr;

    const int cnt1 = VirtFsPhys::exists("test/test2.txt") ? 27 : 26;
    const int cnt2 = 27;

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

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys enumerateFiles2")
{
    logger = new Logger;

    VirtFsPhys::addDirToSearchPath("data/test/dir1",
        Append_false);
    VirtFsPhys::addDirToSearchPath("../data/test/dir1",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFsPhys::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 5);
    VirtFsPhys::freeList(list);

    VirtFsPhys::removeDirFromSearchPath("data/test/dir1");
    VirtFsPhys::removeDirFromSearchPath("../data/test/dir1");
    delete2(logger);
}

TEST_CASE("VirtFsPhys isDirectory")
{
    logger = new Logger();
    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

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

    VirtFsPhys::addDirToSearchPath("data/test", Append_false);
    VirtFsPhys::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFsPhys::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFsPhys::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test") == true);
    REQUIRE(VirtFsPhys::isDirectory("testQ") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1") == true);

    VirtFsPhys::removeDirFromSearchPath("data/test");
    VirtFsPhys::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFsPhys::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml") == false);
    REQUIRE(VirtFsPhys::isDirectory("units.xml/") == false);
    REQUIRE(VirtFsPhys::isDirectory("test") == true);
    REQUIRE(VirtFsPhys::isDirectory("test/") == true);
    REQUIRE(VirtFsPhys::isDirectory("testQ") == false);
    REQUIRE(VirtFsPhys::isDirectory("test/dir1") == true);

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys openRead")
{
    logger = new Logger();
    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

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

    VirtFsPhys::addDirToSearchPath("data/test", Append_false);
    VirtFsPhys::addDirToSearchPath("../data/test", Append_false);

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

    VirtFsPhys::removeDirFromSearchPath("data/test");
    VirtFsPhys::removeDirFromSearchPath("../data/test");

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

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys addZipToSearchPath")
{
    // +++ need implement
}

TEST_CASE("VirtFsPhys removeZipFromSearchPath")
{
    // +++ need implement
}

TEST_CASE("VirtFsPhys getRealDir")
{
    logger = new Logger();
    REQUIRE(VirtFsPhys::getRealDir(".") == "");
    REQUIRE(VirtFsPhys::getRealDir("..") == "");
    const bool dir1 = VirtFsPhys::addDirToSearchPath("data", Append_false);
    REQUIRE((dir1 || VirtFsPhys::addDirToSearchPath(
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

    VirtFsPhys::addDirToSearchPath("data/test", Append_false);
    VirtFsPhys::addDirToSearchPath("../data/test", Append_false);
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
        VirtFsPhys::addZipToSearchPath("data/test/test.zip", Append_false);
        REQUIRE(VirtFsPhys::getRealDir("dir/brimmedhat.png") ==
            "data/test/test.zip");
        REQUIRE(VirtFsPhys::getRealDir("hide.png") == "data/test");
    }
    else
    {
        VirtFsPhys::addZipToSearchPath("../data/test/test.zip", Append_false);
        REQUIRE(VirtFsPhys::getRealDir("dir/brimmedhat.png") ==
            "../data/test/test.zip");
        REQUIRE(VirtFsPhys::getRealDir("hide.png") == "../data/test");
    }

    VirtFsPhys::removeDirFromSearchPath("data/test");
    VirtFsPhys::removeDirFromSearchPath("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFsPhys::getRealDir("dir/hide.png") == "data/test/test.zip");
    }
    else
    {
        REQUIRE(VirtFsPhys::getRealDir("test") == "../data");
        REQUIRE(VirtFsPhys::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFsPhys::getRealDir("dir/hide.png") == "../data/test/test.zip");
    }
    REQUIRE(VirtFsPhys::exists("dir/hide.png"));
    REQUIRE(VirtFsPhys::getRealDir("zzz") == "");

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    VirtFsPhys::removeZipFromSearchPath("data/test/test.zip");
    VirtFsPhys::removeZipFromSearchPath("../data/test/test.zip");
    delete2(logger);
}

TEST_CASE("VirtFsPhys permitLinks")
{
    logger = new Logger();
    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

    const int cnt1 = VirtFsPhys::exists("test/test2.txt") ? 25 : 24;
    const int cnt2 = 25;

    StringVect list;
    VirtFsPhys::permitLinks(false);
    VirtFsPhys::getFiles("test", list);
    removeTemp(list);
    const size_t sz = list.size();
    REQUIRE(sz == cnt1);

    list.clear();
    VirtFsPhys::permitLinks(true);
    VirtFsPhys::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt2);

    list.clear();
    VirtFsPhys::permitLinks(false);
    VirtFsPhys::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt1);

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFsPhys read")
{
    logger = new Logger();
    VirtFsPhys::addDirToSearchPath("data", Append_false);
    VirtFsPhys::addDirToSearchPath("../data", Append_false);

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

    VirtFsPhys::removeDirFromSearchPath("data");
    VirtFsPhys::removeDirFromSearchPath("../data");
    delete2(logger);
}
