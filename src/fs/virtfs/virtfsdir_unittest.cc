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

#ifndef USE_PHYSFS

#include "catch.hpp"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfsdir.h"
#include "fs/virtfstools.h"
#include "fs/virtlist.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("VirtFsDir getEntries")
{
    VirtFsDir::init(".");
    REQUIRE(VirtFsDir::getEntries().empty());
    REQUIRE(VirtFsDir::searchEntryByRoot("test") == nullptr);
    VirtFsDir::deinit();
}

TEST_CASE("VirtFsDir getBaseDir")
{
    VirtFsDir::init(".");
    REQUIRE(VirtFsDir::getBaseDir() != nullptr);
    VirtFsDir::deinit();
}

TEST_CASE("VirtFsDir addToSearchPath")
{
    VirtFsDir::init(".");
    logger = new Logger();
    SECTION("simple 1")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_false,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 1);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1");
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1/",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 1);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1/");
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_false,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir2",
            Append_false,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir2/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 2);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir2/");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir2");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir1");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1\\",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir2",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir2/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 2);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir2/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir2");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir2",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir3/test",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir2/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir3/test/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 3);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir2/");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir2");
        REQUIRE(VirtFsDir::getEntries()[2]->mRootDir == "dir3/test/");
        REQUIRE(VirtFsDir::getEntries()[2]->mUserDir == "dir3/test");
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir2",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir3/test",
            Append_false,
            SkipError_true));
        REQUIRE(VirtFsDir::searchEntryByRoot("dir1/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir2/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("dir3/test/") != nullptr);
        REQUIRE(VirtFsDir::searchEntryByRoot("test/") == nullptr);
        REQUIRE(VirtFsDir::getEntries().size() == 3);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir3/test/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir3/test");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir1");
        REQUIRE(VirtFsDir::getEntries()[2]->mRootDir == "dir2/");
        REQUIRE(VirtFsDir::getEntries()[2]->mUserDir == "dir2");
    }

    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir removeFromSearchPath")
{
    VirtFsDir::init(".");
    logger = new Logger();

    SECTION("simple 1")
    {
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir1"));
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir1/"));
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir2"));
        REQUIRE(VirtFsDir::removeFromSearchPath("dir1"));
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir2//dir3",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir3",
            Append_false,
            SkipError_true));
        REQUIRE(VirtFsDir::getEntries().size() == 3);
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir2"));
        REQUIRE(VirtFsDir::removeFromSearchPath("dir1"));
        REQUIRE(VirtFsDir::getEntries().size() == 2);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir3/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir3");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir2/dir3/");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir2/dir3");
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir1"));
        REQUIRE(VirtFsDir::getEntries().size() == 2);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir3/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir3");
        REQUIRE(VirtFsDir::getEntries()[1]->mRootDir == "dir2/dir3/");
        REQUIRE(VirtFsDir::getEntries()[1]->mUserDir == "dir2/dir3");
        REQUIRE(VirtFsDir::removeFromSearchPath("dir2/dir3"));
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir2/dir3/"));
        REQUIRE(VirtFsDir::getEntries().size() == 1);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir3/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir3");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::getEntries().size() == 1);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1");
        REQUIRE_THROWS(VirtFsDir::removeFromSearchPath("dir2"));
        REQUIRE(VirtFsDir::removeFromSearchPath("dir1"));
        REQUIRE(VirtFsDir::getEntries().size() == 0);
        REQUIRE(VirtFsDir::addToSearchPathSilent("dir1",
            Append_true,
            SkipError_true));
        REQUIRE(VirtFsDir::getEntries().size() == 1);
        REQUIRE(VirtFsDir::getEntries()[0]->mRootDir == "dir1/");
        REQUIRE(VirtFsDir::getEntries()[0]->mUserDir == "dir1");
    }

    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir exists")
{
    VirtFsDir::init(".");
    logger = new Logger();
    VirtFsDir::addToSearchPathSilent("data/",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("..\\data",
        Append_false,
        SkipError_false);

    REQUIRE(VirtFsDir::exists("test") == true);
    REQUIRE(VirtFsDir::exists("test/dir1"));
    REQUIRE(VirtFsDir::exists("test/dir") == false);
    REQUIRE(VirtFsDir::exists("test//units.xml") == true);
    REQUIRE(VirtFsDir::exists("test/\\units123.xml") == false);
    REQUIRE(VirtFsDir::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::exists("units.xml") == false);

    VirtFsDir::addToSearchPathSilent("data//test",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("..//data\\test",
        Append_false,
        SkipError_false);

    REQUIRE(VirtFsDir::exists("test") == true);
    REQUIRE(VirtFsDir::exists("test/dir1"));
    REQUIRE(VirtFsDir::exists("test/dir") == false);
    REQUIRE(VirtFsDir::exists("test\\units.xml") == true);
    REQUIRE(VirtFsDir::exists("test/units123.xml") == false);
    REQUIRE(VirtFsDir::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::exists("units.xml") == true);

    VirtFsDir::removeFromSearchPathSilent("data/test");
    VirtFsDir::removeFromSearchPathSilent("../data/test");

    REQUIRE(VirtFsDir::exists("test") == true);
    REQUIRE(VirtFsDir::exists("test/dir1"));
    REQUIRE(VirtFsDir::exists("test/dir") == false);
    REQUIRE(VirtFsDir::exists("test\\units.xml") == true);
    REQUIRE(VirtFsDir::exists("test/units123.xml") == false);
    REQUIRE(VirtFsDir::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::exists("units.xml") == false);

    REQUIRE_THROWS(VirtFsDir::exists("test/../units.xml"));

    VirtFsDir::deinit();
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

TEST_CASE("VirtFsDir getRealDir")
{
    VirtFsDir::init(".");
    logger = new Logger();
    REQUIRE(VirtFsDir::getRealDir(".") == "");
    REQUIRE(VirtFsDir::getRealDir("..") == "");
    const bool dir1 = VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    REQUIRE((dir1 || VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false)) == true);
    if (dir1 == true)
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFsDir::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFsDir::getRealDir("test//test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "../data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFsDir::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFsDir::getRealDir("test//test.txt") ==
            "../data");
    }
    REQUIRE(VirtFsDir::getRealDir("zzz") == "");

    VirtFsDir::addToSearchPathSilent("data/test",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test",
        Append_false,
        SkipError_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFsDir::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFsDir::getRealDir("test.txt") ==
            "data/test");
    }
    else
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "../data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFsDir::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFsDir::getRealDir("test.txt") ==
            "../data/test");
    }
    REQUIRE(VirtFsDir::getRealDir("zzz") == "");

    VirtFsDir::removeFromSearchPathSilent("data/test");
    VirtFsDir::removeFromSearchPathSilent("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFsDir::getRealDir("test") == "../data");
        REQUIRE(VirtFsDir::getRealDir("test/test.txt") ==
            "../data");
    }
    REQUIRE(VirtFsDir::getRealDir("zzz") == "");

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
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

TEST_CASE("VirtFsDir enumerateFiles1")
{
    VirtFsDir::init(".");
    logger = new Logger;

    VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false);

    VirtList *list = nullptr;

    const int cnt1 = VirtFsDir::exists("test/test2.txt") ? 27 : 26;
    const int cnt2 = 27;

    VirtFsDir::permitLinks(false);
    list = VirtFsDir::enumerateFiles("test");
    removeTemp(list->names);
    const size_t sz = list->names.size();
    REQUIRE(sz == cnt1);
    VirtFsDir::freeList(list);

    VirtFsDir::permitLinks(true);
    list = VirtFsDir::enumerateFiles("test/");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt2);
    VirtFsDir::freeList(list);

    VirtFsDir::permitLinks(true);
    list = VirtFsDir::enumerateFiles("test/units.xml");
    REQUIRE(list->names.size() == 0);
    VirtFsDir::freeList(list);

    VirtFsDir::permitLinks(false);
    list = VirtFsDir::enumerateFiles("test\\");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt1);
    VirtFsDir::freeList(list);

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir enumerateFiles2")
{
    VirtFsDir::init(".");
    logger = new Logger;

    VirtFsDir::addToSearchPathSilent("data/test/dir1",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test/dir1",
        Append_false,
        SkipError_false);

    VirtList *list = nullptr;

    list = VirtFsDir::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 5);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE_FALSE(inList(list, "file2.txt"));
    VirtFsDir::freeList(list);
    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir enumerateFiles3")
{
    VirtFsDir::init(".");
    logger = new Logger;

    VirtFsDir::addToSearchPathSilent("data/test/dir1",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test/dir1",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("data/test/dir2",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test/dir2",
        Append_false,
        SkipError_false);

    VirtList *list = nullptr;

    list = VirtFsDir::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 6);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFsDir::freeList(list);
    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir isDirectory")
{
    VirtFsDir::init(".");
    logger = new Logger();
    VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false);

    REQUIRE(VirtFsDir::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFsDir::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test//units123.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test") == true);
    REQUIRE(VirtFsDir::isDirectory("test/") == true);
    REQUIRE(VirtFsDir::isDirectory("test//") == true);
    REQUIRE(VirtFsDir::isDirectory("test/dir1") == true);
    REQUIRE(VirtFsDir::isDirectory("test//dir1") == true);
    REQUIRE(VirtFsDir::isDirectory("test//dir1/") == true);
    REQUIRE(VirtFsDir::isDirectory("test//dir1//") == true);
    REQUIRE(VirtFsDir::isDirectory("test\\dir1/") == true);
    REQUIRE(VirtFsDir::isDirectory("test/dir1//") == true);
    REQUIRE(VirtFsDir::isDirectory("testQ") == false);
    REQUIRE(VirtFsDir::isDirectory("testQ/") == false);
    REQUIRE(VirtFsDir::isDirectory("testQ//") == false);

    VirtFsDir::addToSearchPathSilent("data/test",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test",
        Append_false,
        SkipError_false);

    REQUIRE(VirtFsDir::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFsDir::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test") == true);
    REQUIRE(VirtFsDir::isDirectory("testQ") == false);
    REQUIRE(VirtFsDir::isDirectory("test/dir1") == true);
    REQUIRE(VirtFsDir::isDirectory("test\\dir1") == true);

    VirtFsDir::removeFromSearchPathSilent("data/test");
    VirtFsDir::removeFromSearchPathSilent("../data/test");

    REQUIRE(VirtFsDir::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("units.xml") == false);
    REQUIRE(VirtFsDir::isDirectory("units.xml/") == false);
    REQUIRE(VirtFsDir::isDirectory("test") == true);
    REQUIRE(VirtFsDir::isDirectory("test/") == true);
    REQUIRE(VirtFsDir::isDirectory("testQ") == false);
    REQUIRE(VirtFsDir::isDirectory("test/dir1") == true);

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir openRead")
{
    VirtFsDir::init(".");
    logger = new Logger();
    VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false);

    VirtFile *file = nullptr;

    file = VirtFsDir::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsDir::close(file);
    file = VirtFsDir::openRead("test\\units.xml");
    REQUIRE(file != nullptr);
    VirtFsDir::close(file);
    file = VirtFsDir::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsDir::addToSearchPathSilent("data/test",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data/test",
        Append_false,
        SkipError_false);

    file = VirtFsDir::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsDir::close(file);
    file = VirtFsDir::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFsDir::close(file);
    file = VirtFsDir::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsDir::removeFromSearchPathSilent("data/test");
    VirtFsDir::removeFromSearchPathSilent("../data/test");

    file = VirtFsDir::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFsDir::close(file);
    file = VirtFsDir::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsDir::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
    delete2(logger);
}


TEST_CASE("VirtFsDir permitLinks")
{
    VirtFsDir::init(".");
    logger = new Logger();
    VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false);

    const int cnt1 = VirtFsDir::exists("test/test2.txt") ? 25 : 24;
    const int cnt2 = 25;

    StringVect list;
    VirtFsDir::permitLinks(false);
    VirtFsDir::getFiles("test", list);
    removeTemp(list);
    const size_t sz = list.size();
    REQUIRE(sz == cnt1);

    list.clear();
    VirtFsDir::permitLinks(true);
    VirtFsDir::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt2);

    list.clear();
    VirtFsDir::permitLinks(false);
    VirtFsDir::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt1);

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsDir read")
{
    VirtFsDir::init(".");
    logger = new Logger();
    VirtFsDir::addToSearchPathSilent("data",
        Append_false,
        SkipError_false);
    VirtFsDir::addToSearchPathSilent("../data",
        Append_false,
        SkipError_false);

    VirtFile *file = VirtFsDir::openRead("test/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFsDir::fileLength(file) == 23);
    const int fileSize = VirtFsDir::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFsDir::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFsDir::tell(file) == fileSize);
    REQUIRE(VirtFsDir::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFsDir::seek(file, 12) != 0);
    REQUIRE(VirtFsDir::eof(file) == false);
    REQUIRE(VirtFsDir::tell(file) == 12);
    REQUIRE(VirtFsDir::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFsDir::eof(file) == true);

    VirtFsDir::close(file);
    free(buffer);

    VirtFsDir::removeFromSearchPathSilent("data");
    VirtFsDir::removeFromSearchPathSilent("../data");
    VirtFsDir::deinit();
    delete2(logger);
}
#endif  // USE_PHYSFS
