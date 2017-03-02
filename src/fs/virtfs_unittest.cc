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
#include "fs/virtfstools.h"
#include "fs/virtlist.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

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

TEST_CASE("VirtFs exists1")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs exists2")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == false);
    REQUIRE(VirtFs::exists("test.txt") == true);
    REQUIRE(VirtFs::exists("dir/hide.png") == true);
    REQUIRE(VirtFs::exists("dir/gpl") == true);
    REQUIRE(VirtFs::exists("dir/gpl/zzz") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units.xml.") == false);
    REQUIRE(VirtFs::exists("units.xml2") == false);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs exists3")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test.zip", Append_false);
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == false);
    REQUIRE(VirtFs::exists("dir/brimmedhat.png"));
    REQUIRE(VirtFs::exists("dir//brimmedhat.png"));
    REQUIRE(VirtFs::exists("dir//hide.png"));
    REQUIRE(VirtFs::exists("dir/1"));
    REQUIRE(VirtFs::exists("dir/gpl"));
    REQUIRE(VirtFs::exists("dir/dye.png"));
    REQUIRE(VirtFs::exists("dir/2") == false);
    REQUIRE(VirtFs::exists("dir2/2") == false);
    REQUIRE(VirtFs::exists("dir2/paths.xml"));

    VirtFs::removeZipFromSearchPath("data/test/test.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test.zip");
    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs exists4")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test.zip", Append_false);
    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == false);
    REQUIRE(VirtFs::exists("dir/brimmedhat.png"));
    REQUIRE(VirtFs::exists("dir//brimmedhat.png"));
    REQUIRE(VirtFs::exists("dir//hide.png"));
    REQUIRE(VirtFs::exists("dir/1") == false);
    REQUIRE(VirtFs::exists("dir/gpl") == false);
    REQUIRE(VirtFs::exists("dir/dye.png") == false);
    REQUIRE(VirtFs::exists("dir/2") == false);
    REQUIRE(VirtFs::exists("dir2/2") == false);
    REQUIRE(VirtFs::exists("dir2/paths.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("dir1/file1.txt"));
    REQUIRE(VirtFs::exists("dir2/file2.txt"));
    REQUIRE(VirtFs::exists("dir2/file3.txt") == false);

    VirtFs::removeZipFromSearchPath("data/test/test.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test.zip");
    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");
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

TEST_CASE("VirtFs enumerateFiles1")
{
    logger = new Logger;

    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    VirtList *list = nullptr;

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 28 : 27;
    const int cnt2 = 28;

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test");
    removeTemp(list->names);
    const size_t sz = list->names.size();
    REQUIRE(sz == cnt1);
    VirtFs::freeList(list);

    VirtFs::permitLinks(true);
    list = VirtFs::enumerateFiles("test");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt2);
    VirtFs::freeList(list);

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt1);
    VirtFs::freeList(list);

    list = VirtFs::enumerateFiles("test/units.xml");
    removeTemp(list->names);
    REQUIRE(list->names.size() == 0);
    VirtFs::freeList(list);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs enumerateFiles2")
{
    logger = new Logger;

    VirtFs::addDirToSearchPath("data/test/dir1",
        Append_false);
    VirtFs::addDirToSearchPath("../data/test/dir1",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 5);
    VirtFs::freeList(list);

    VirtFs::removeDirFromSearchPath("data/test/dir1");
    VirtFs::removeDirFromSearchPath("../data/test/dir1");
    delete2(logger);
}

TEST_CASE("VirtFs enumerateFiles3")
{
    logger = new Logger;

    VirtFs::addZipToSearchPath("data/test/test.zip",
        Append_false);
    VirtFs::addZipToSearchPath("../data/test/test.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 1);
    REQUIRE(inList(list, "dir"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath("data/test/test.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test.zip");
    delete2(logger);
}

TEST_CASE("VirtFs enumerateFiles4")
{
    logger = new Logger;

    VirtFs::addZipToSearchPath("data/test/test2.zip",
        Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 4);
    REQUIRE(inList(list, "dir"));
    REQUIRE(inList(list, "dir2"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs enumerateFiles5")
{
    logger = new Logger;

    VirtFs::addZipToSearchPath("data/test/test2.zip",
        Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip",
        Append_false);
    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("dir2");
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "paths.xml"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");
    delete2(logger);
}

TEST_CASE("VirtFs isDirectory1")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("test//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("test//") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1/") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1//") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1/") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs isDirectory2")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir/") == true);
    REQUIRE(VirtFs::isDirectory("dir//") == true);
    REQUIRE(VirtFs::isDirectory("dir2") == true);
    REQUIRE(VirtFs::isDirectory("dir3") == false);
    REQUIRE(VirtFs::isDirectory("test.txt") == false);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs isDirectory3")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test"));
    REQUIRE(VirtFs::isDirectory("test//dye.png") == false);
    REQUIRE(VirtFs::isDirectory("dir"));
    REQUIRE(VirtFs::isDirectory("dir/"));
    REQUIRE(VirtFs::isDirectory("dir//"));
    REQUIRE(VirtFs::isDirectory("dir2"));
    REQUIRE(VirtFs::isDirectory("dir3") == false);
    REQUIRE(VirtFs::isDirectory("test.txt") == false);
    REQUIRE(VirtFs::isDirectory("dir/hide.png") == false);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs openRead1")
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
    delete2(logger);
}

TEST_CASE("VirtFs openRead2")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    VirtFile *file = nullptr;

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/hide.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir//hide.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs openRead3")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);
    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);

    VirtFile *file = nullptr;

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/hide.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir//hide.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/dye.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/dye.pn_");
    REQUIRE(file == nullptr);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");
    delete2(logger);
}

TEST_CASE("VirtFs addZipToSearchPath")
{
    // +++ need implement
}

TEST_CASE("VirtFs removeZipFromSearchPath")
{
    // +++ need implement
}

TEST_CASE("VirtFs getRealDir1")
{
    logger = new Logger();
    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
    const bool dir1 = VirtFs::addDirToSearchPath("data", Append_false);
    REQUIRE((dir1 || VirtFs::addDirToSearchPath("../data", Append_false)) ==
        true);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "data/test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "../data/test");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    if (dir1 == true)
    {
        VirtFs::addZipToSearchPath("data/test/test.zip", Append_false);
        REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") ==
            "data/test/test.zip");
        REQUIRE(VirtFs::getRealDir("hide.png") == "data/test");
    }
    else
    {
        VirtFs::addZipToSearchPath("../data/test/test.zip", Append_false);
        REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") ==
            "../data/test/test.zip");
        REQUIRE(VirtFs::getRealDir("hide.png") == "../data/test");
    }

    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("dir/hide.png") == "data/test/test.zip");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("dir/hide.png") == "../data/test/test.zip");
    }
    REQUIRE(VirtFs::exists("dir/hide.png"));
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    VirtFs::removeZipFromSearchPath("data/test/test.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test.zip");
    delete2(logger);
}

TEST_CASE("VirtFs getrealDir2")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);
    VirtFs::addDirToSearchPath("data/test", Append_false);
    VirtFs::addDirToSearchPath("../data/test", Append_false);
    const bool dir1 = VirtFs::addDirToSearchPath("data", Append_false);
    REQUIRE((dir1 || VirtFs::addDirToSearchPath("../data", Append_false)) ==
        true);

    REQUIRE(VirtFs::getRealDir("zzz") == "");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("dir1/file1.txt") ==
            "data/test");
        REQUIRE(VirtFs::getRealDir("hide.png") == "data/test");
        REQUIRE(VirtFs::getRealDir("dir//hide.png") ==
            "data/test/test2.zip");
        REQUIRE(VirtFs::getRealDir("dir/1//test.txt") ==
            "data/test/test2.zip");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("dir1/file1.txt") ==
            "../data/test");
        REQUIRE(VirtFs::getRealDir("hide.png") == "../data/test");
        REQUIRE(VirtFs::getRealDir("dir//hide.png") ==
            "../data/test/test2.zip");
        REQUIRE(VirtFs::getRealDir("dir/1//test.txt") ==
            "../data/test/test2.zip");
    }

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    VirtFs::removeDirFromSearchPath("data/test");
    VirtFs::removeDirFromSearchPath("../data/test");
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs permitLinks")
{
    logger = new Logger();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 26 : 25;
    const int cnt2 = 26;

    StringVect list;
    VirtFs::permitLinks(false);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    const size_t sz = list.size();
    REQUIRE(sz == cnt1);

    list.clear();
    VirtFs::permitLinks(true);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt2);

    list.clear();
    VirtFs::permitLinks(false);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt1);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs read1")
{
    logger = new Logger();
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
    REQUIRE(VirtFs::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == false);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFs::eof(file) == true);

    VirtFs::close(file);
    free(buffer);

    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("VirtFs read2")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);

    VirtFile *file = VirtFs::openRead("dir2/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);
    REQUIRE(VirtFs::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == false);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFs::eof(file) == true);

    VirtFs::close(file);
    free(buffer);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    delete2(logger);
}

TEST_CASE("VirtFs read3")
{
    logger = new Logger();
    VirtFs::addZipToSearchPath("data/test/test2.zip", Append_false);
    VirtFs::addZipToSearchPath("../data/test/test2.zip", Append_false);
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    VirtFile *file = VirtFs::openRead("dir2/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);
    REQUIRE(VirtFs::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == false);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFs::eof(file) == true);

    VirtFs::close(file);
    free(buffer);

    VirtFs::removeZipFromSearchPath("data/test/test2.zip");
    VirtFs::removeZipFromSearchPath("../data/test/test2.zip");
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}
