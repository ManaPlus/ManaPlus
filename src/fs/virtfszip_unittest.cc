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

#include "fs/files.h"
#include "fs/virtlist.h"
#include "fs/virtfszip.h"
#include "fs/virtzipentry.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("VirtFsZip getEntries")
{
    VirtFsZip::init();
    REQUIRE(VirtFsZip::getEntries().empty());
    REQUIRE(VirtFsZip::searchEntryByArchive("test.zip") == nullptr);
    VirtFsZip::deinit();
}

TEST_CASE("VirtFsZip addToSearchPath")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    std::vector<ZipLocalHeader*> headers;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    SECTION("simple 1")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "/test.zip",
            Append_false));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "file2.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 1);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "\\test.zip",
            Append_true));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "file2.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 1);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_false));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
            Append_false));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test2.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test3.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 2);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test2.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test.zip");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
            Append_true));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test2.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test3.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 2);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test2.zip");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test3.zip",
            Append_true));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test2.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test3.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test4.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 3);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test2.zip");
        REQUIRE(VirtFsZip::getEntries()[2]->mArchiveName ==
            prefix + "test3.zip");
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test3.zip",
            Append_false));
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test2.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test3.zip") != nullptr);
        REQUIRE(VirtFsZip::searchEntryByArchive(
            prefix + "test4.zip") == nullptr);
        REQUIRE(VirtFsZip::getEntries().size() == 3);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test3.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test.zip");
        REQUIRE(VirtFsZip::getEntries()[2]->mArchiveName ==
            prefix + "test2.zip");
    }

    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip removeFromSearchPath")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    std::vector<ZipLocalHeader*> headers;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    SECTION("simple 1")
    {
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(
            prefix + "test123.zip"));
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(prefix + "test2.zip"));
        REQUIRE(VirtFsZip::removeFromSearchPath(prefix + "test.zip"));
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
            Append_true));
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test3.zip",
            Append_false));
        REQUIRE(VirtFsZip::getEntries().size() == 3);
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(prefix + "test4.zip"));
        REQUIRE(VirtFsZip::removeFromSearchPath(prefix + "test.zip"));
        REQUIRE(VirtFsZip::getEntries().size() == 2);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test3.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test2.zip");
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(prefix + "test.zip"));
        REQUIRE(VirtFsZip::getEntries().size() == 2);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test3.zip");
        REQUIRE(VirtFsZip::getEntries()[1]->mArchiveName ==
            prefix + "test2.zip");
        REQUIRE(VirtFsZip::removeFromSearchPath(prefix + "//test2.zip"));
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(prefix + "test2.zip"));
        REQUIRE(VirtFsZip::getEntries().size() == 1);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test3.zip");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "\\test.zip",
            Append_true));
        REQUIRE(VirtFsZip::getEntries().size() == 1);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
        REQUIRE_THROWS(VirtFsZip::removeFromSearchPath(prefix + "test2.zip"));
        REQUIRE(VirtFsZip::removeFromSearchPath(prefix + "\\test.zip"));
        REQUIRE(VirtFsZip::getEntries().size() == 0);
        REQUIRE(VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
            Append_true));
        REQUIRE(VirtFsZip::getEntries().size() == 1);
        REQUIRE(VirtFsZip::getEntries()[0]->mArchiveName ==
            prefix + "test.zip");
    }

    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip exists")
{
    VirtFsZip::init();
    logger = new Logger();
    VirtFsZip::addToSearchPathSilent("data\\test/test2.zip",
        Append_false);
    VirtFsZip::addToSearchPathSilent("../data\\test/test2.zip",
        Append_false);

    REQUIRE(VirtFsZip::exists("dir2//units.xml") == true);
    REQUIRE(VirtFsZip::exists("test/units123.xml") == false);
    REQUIRE(VirtFsZip::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::exists("units1.xml") == false);
    REQUIRE(VirtFsZip::exists("dir/hide.png") == true);
    REQUIRE(VirtFsZip::exists("dir/brimmedhat.png") == false);

    VirtFsZip::addToSearchPathSilent("data/test/test.zip",
        Append_false);
    VirtFsZip::addToSearchPathSilent("../data/test/test.zip",
        Append_false);

    REQUIRE(VirtFsZip::exists("dir2\\units.xml") == true);
    REQUIRE(VirtFsZip::exists("test/units123.xml") == false);
    REQUIRE(VirtFsZip::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::exists("units1.xml") == false);
    REQUIRE(VirtFsZip::exists("dir/hide.png") == true);
    REQUIRE(VirtFsZip::exists("dir/brimmedhat.png") == true);

    VirtFsZip::removeFromSearchPathSilent("data/test/test2.zip");
    VirtFsZip::removeFromSearchPathSilent("../data/test/test2.zip");

    REQUIRE(VirtFsZip::exists("dir2//units.xml") == false);
    REQUIRE(VirtFsZip::exists("test/units123.xml") == false);
    REQUIRE(VirtFsZip::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::exists("units1.xml") == false);
    REQUIRE(VirtFsZip::exists("dir/\\/hide.png") == true);
    REQUIRE(VirtFsZip::exists("dir/brimmedhat.png") == true);

    REQUIRE_THROWS(VirtFsZip::exists("test/../units.xml"));

    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip getRealDir")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
        Append_false);

    REQUIRE(VirtFsZip::getRealDir(".") == "");
    REQUIRE(VirtFsZip::getRealDir("..") == "");
    REQUIRE(VirtFsZip::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFsZip::getRealDir("dir\\dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFsZip::getRealDir("zzz") == "");

    VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
        Append_false);
    REQUIRE(VirtFsZip::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFsZip::getRealDir("dir///hide.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFsZip::getRealDir("dir\\\\brimmedhat.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFsZip::getRealDir("zzz") == "");

    VirtFsZip::removeFromSearchPathSilent(prefix + "test.zip");

    REQUIRE(VirtFsZip::getRealDir("dir/brimmedhat.png") == "");
    REQUIRE(VirtFsZip::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFsZip::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFsZip::getRealDir("zzz") == "");

    VirtFsZip::removeFromSearchPathSilent(prefix + "test2.zip");
    VirtFsZip::deinit();
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

TEST_CASE("VirtFsZip enumerateFiles1")
{
    VirtFsZip::init();
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFsZip::enumerateFiles("dir");
    REQUIRE(list->names.size() == 2);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    VirtFsZip::freeList(list);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test.zip");
    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles2")
{
    VirtFsZip::init();
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data//test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
        Append_true);
    VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
        Append_true);

    VirtList *list = nullptr;

    list = VirtFsZip::enumerateFiles("dir");
    FOR_EACH (StringVectCIter, it, list->names)
    {
        logger->log("filename: " + *it);
    }

    REQUIRE(list->names.size() == 5);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "1"));
    REQUIRE(inList(list, "gpl"));
    REQUIRE(inList(list, "dye.png"));
    VirtFsZip::freeList(list);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test.zip");
    VirtFsZip::removeFromSearchPathSilent(prefix + "test2.zip");
    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip isDirectory")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
        Append_false);

    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir") == true);
    REQUIRE(VirtFsZip::isDirectory("dir2/") == true);
    REQUIRE(VirtFsZip::isDirectory("dir2//") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir//1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir\\1/") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFsZip::isDirectory("test/dir1\\") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ/") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ//") == false);

    VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
        Append_false);

    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2\\units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir") == true);
    REQUIRE(VirtFsZip::isDirectory("dir2/") == true);
    REQUIRE(VirtFsZip::isDirectory("dir2\\") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir//1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir//1/") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == true);
    REQUIRE(VirtFsZip::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFsZip::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ/") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ//") == false);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test2.zip");

    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("units.xml") == false);
    REQUIRE(VirtFsZip::isDirectory("dir") == true);
    REQUIRE(VirtFsZip::isDirectory("dir2/") == false);
    REQUIRE(VirtFsZip::isDirectory("dir2//") == false);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == false);
    REQUIRE(VirtFsZip::isDirectory("dir\\1") == false);
    REQUIRE(VirtFsZip::isDirectory("dir//1/") == false);
    REQUIRE(VirtFsZip::isDirectory("dir/1") == false);
    REQUIRE(VirtFsZip::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFsZip::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ/") == false);
    REQUIRE(VirtFsZip::isDirectory("testQ//") == false);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test.zip");
    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip openRead")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
        Append_false);

    VirtFile *file = nullptr;

    file = VirtFsZip::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2\\units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("dir//brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFsZip::addToSearchPathSilent(prefix + "test.zip",
        Append_false);

    file = VirtFsZip::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2//units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("dir/brimmedhat.png");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test.zip");

    file = VirtFsZip::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2\\/\\units.xml");
    REQUIRE(file != nullptr);
    VirtFsZip::close(file);
    file = VirtFsZip::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFsZip::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFsZip::removeFromSearchPathSilent(prefix + "test2.zip");

    VirtFsZip::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip read")
{
    VirtFsZip::init();
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFsZip::addToSearchPathSilent(prefix + "test2.zip",
        Append_false);
    VirtFile *file = nullptr;
    void *restrict buffer = nullptr;

    SECTION("test 1")
    {
        file = VirtFsZip::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFsZip::fileLength(file) == 23);
        const int fileSize = VirtFsZip::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFsZip::read(file, buffer, 1, fileSize) == fileSize);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 1\ntest line 2") == 0);
        REQUIRE(VirtFsZip::tell(file) == fileSize);
        REQUIRE(VirtFsZip::eof(file) == true);
    }

    SECTION("test 2")
    {
        file = VirtFsZip::openRead("dir2\\/test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFsZip::fileLength(file) == 23);
        const int fileSize = VirtFsZip::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFsZip::seek(file, 12) != 0);
        REQUIRE(VirtFsZip::eof(file) == false);
        REQUIRE(VirtFsZip::tell(file) == 12);
        REQUIRE(VirtFsZip::read(file, buffer, 1, 11) == 11);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 2") == 0);
        REQUIRE(VirtFsZip::eof(file) == true);
    }

    SECTION("test 3")
    {
        file = VirtFsZip::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFsZip::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize; f ++)
        {
            REQUIRE(VirtFsZip::seek(file, f) != 0);
            REQUIRE(VirtFsZip::eof(file) == false);
            REQUIRE(VirtFsZip::tell(file) == f);
        }
    }

    SECTION("test 4")
    {
        file = VirtFsZip::openRead("dir2/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFsZip::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f ++)
        {
            REQUIRE(VirtFsZip::read(file, buffer, 1, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(VirtFsZip::eof(file) == false);
            REQUIRE(VirtFsZip::tell(file) == f + 1);
        }
        REQUIRE(VirtFsZip::read(file, buffer, 1, 1) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFsZip::eof(file) == true);
        REQUIRE(VirtFsZip::tell(file) == fileSize);
    }

    SECTION("test 5")
    {
        file = VirtFsZip::openRead("dir2\\\\test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFsZip::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFsZip::read(file, buffer, 2, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFsZip::eof(file) == false);
            REQUIRE(VirtFsZip::tell(file) == f + 2);
        }
        REQUIRE(VirtFsZip::eof(file) == false);
        REQUIRE(VirtFsZip::tell(file) == 22);
        REQUIRE(VirtFsZip::read(file, buffer, 2, 1) == 0);
        REQUIRE(VirtFsZip::eof(file) == false);
    }

    SECTION("test 6")
    {
        file = VirtFsZip::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFsZip::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFsZip::read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFsZip::eof(file) == false);
            REQUIRE(VirtFsZip::tell(file) == f + 2);
        }
        REQUIRE(VirtFsZip::eof(file) == false);
        REQUIRE(VirtFsZip::tell(file) == 22);
        REQUIRE(VirtFsZip::read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFsZip::eof(file) == true);
    }

    VirtFsZip::close(file);
    free(buffer);
    VirtFsZip::removeFromSearchPathSilent(prefix + "test2.zip");
    VirtFsZip::deinit();
    delete2(logger);
}
