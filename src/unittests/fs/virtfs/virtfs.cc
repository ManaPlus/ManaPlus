/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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
#include "fs/paths.h"

#include "fs/virtfs/fs.h"
#include "fs/virtfs/list.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("VirtFs dirSeparator", "")
{
    REQUIRE(dirSeparator != nullptr);
    REQUIRE(VirtFs::getDirSeparator() == std::string(dirSeparator));
    VirtFs::updateDirSeparator();
    REQUIRE(dirSeparator != nullptr);
    REQUIRE(VirtFs::getDirSeparator() == std::string(dirSeparator));
}

TEST_CASE("VirtFs getBaseDir", "")
{
    REQUIRE(VirtFs::getBaseDir() != nullptr);
}

TEST_CASE("VirtFs getUserDir", "")
{
    REQUIRE(VirtFs::getUserDir() != nullptr);
}

TEST_CASE("VirtFs exists1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1") == true);
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs exists2", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == false);
    REQUIRE(VirtFs::exists("test.txt") == true);
    REQUIRE(VirtFs::exists("dir/hide.png") == true);
    REQUIRE(VirtFs::exists("dir/gpl") == true);
    REQUIRE(VirtFs::exists("dir/gpl/zzz") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units.xml.") == false);
    REQUIRE(VirtFs::exists("units.xml2") == false);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs exists3", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);
    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

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

    VirtFs::unmountZip(prefix + "data/test/test.zip");
    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs exists4", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);
    VirtFs::mountDirSilent(prefix + "data/test", Append_false);

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

    VirtFs::unmountZip(prefix + "data/test/test.zip");
    VirtFs::unmountDirSilent(prefix + "data/test");
}

TEST_CASE("VirtFs exists5", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    const std::string realDir = getRealPath(prefix + "data");
    logger->log("real dir: " + realDir);
    REQUIRE_FALSE(VirtFs::exists(realDir));
}

TEST_CASE("VirtFs exists6", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/units.xml") == false);
    REQUIRE(VirtFs::exists("test.txt") == false);
    REQUIRE(VirtFs::exists("dir/hide.png") == false);
    REQUIRE(VirtFs::exists("dir/gpl") == false);
    REQUIRE(VirtFs::exists("dir/gpl/zzz") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml.") == false);
    REQUIRE(VirtFs::exists("units.xml2") == false);
    REQUIRE(VirtFs::exists("hide.png"));
    REQUIRE(VirtFs::exists("dye.png"));
    REQUIRE(VirtFs::exists("gpl"));
    REQUIRE(VirtFs::exists("gpl/zzz") == false);

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
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

static bool inList(const VirtFs::List *const list,
                   const std::string &name)
{
    FOR_EACH (StringVectCIter, it, list->names)
    {
        if (*it == name)
            return true;
    }
    return false;
}

TEST_CASE("VirtFs enumerateFiles1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    VirtFs::List *list = nullptr;

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 30 : 29;
    const int cnt2 = 30;

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
    REQUIRE(list->names.empty());
    VirtFs::freeList(list);

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs enumerateFiles2", "")
{
    VirtFs::mountDirSilent("data/test/dir1",
        Append_false);
    VirtFs::mountDirSilent("../data/test/dir1",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 5);
    VirtFs::freeList(list);

    VirtFs::unmountDirSilent("data/test/dir1");
    VirtFs::unmountDirSilent("../data/test/dir1");
}

TEST_CASE("VirtFs enumerateFiles3", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test.zip",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 1);
    REQUIRE(inList(list, "dir"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "data/test/test.zip");
}

TEST_CASE("VirtFs enumerateFiles4", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 4);
    REQUIRE(inList(list, "dir"));
    REQUIRE(inList(list, "dir2"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs enumerateFiles5", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);
    VirtFs::mountDirSilent(prefix + "data/test", Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir2");
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "paths.xml"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDirSilent(prefix + "data/test");
}

TEST_CASE("VirtFs isDirectory1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

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

    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs isDirectory2", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir/") == true);
    REQUIRE(VirtFs::isDirectory("dir//") == true);
    REQUIRE(VirtFs::isDirectory("dir2") == true);
    REQUIRE(VirtFs::isDirectory("dir3") == false);
    REQUIRE(VirtFs::isDirectory("test.txt") == false);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs isDirectory3", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data", Append_false);
    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

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

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data");
}

TEST_CASE("VirtFs openRead1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    VirtFs::File *file = nullptr;

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

    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

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

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

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

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs openRead2", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

    VirtFs::File *file = nullptr;

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

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs openRead3", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);
    VirtFs::mountDir(prefix + "data/test", Append_false);

    VirtFs::File *file = nullptr;

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

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data/test");
}

TEST_CASE("VirtFs getRealDir1", "")
{
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    const bool dir1 = VirtFs::mountDirSilent("data", Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent("../data", Append_false))
        == true);
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
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "data" + sep + "test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            ".." + sep + "data" + sep + "test");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    if (dir1 == true)
    {
        VirtFs::mountZip("data/test/test.zip", Append_false);
        REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") ==
            "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getRealDir("hide.png") ==
            "data" + sep + "test");
    }
    else
    {
        VirtFs::mountZip("../data/test/test.zip", Append_false);
        REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") ==
            ".." + sep + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getRealDir("hide.png") ==
            ".." + sep + "data" + sep + "test");
    }

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("dir/hide.png") ==
            "data" + sep + "test" + sep + "test.zip");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("dir/hide.png") ==
            ".." + sep + "data" + sep + "test" + sep + "test.zip");
    }
    REQUIRE(VirtFs::exists("dir/hide.png"));
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    if (dir1 == true)
        VirtFs::unmountZip("data/test/test.zip");
    else
        VirtFs::unmountZip("../data/test/test.zip");
}

TEST_CASE("VirtFs getrealDir2", "")
{
    const std::string sep = dirSeparator;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);
    VirtFs::mountDir(prefix + "data/test", Append_false);
    VirtFs::mountDir(prefix + "data", Append_false);

    REQUIRE(VirtFs::getRealDir("zzz").empty());

    REQUIRE(VirtFs::getRealDir("dir1/file1.txt") ==
        prefix + "data" + sep + "test");
    REQUIRE(VirtFs::getRealDir("hide.png") ==
        prefix + "data" + sep + "test");
    REQUIRE(VirtFs::getRealDir("dir//hide.png") ==
        prefix + "data" + sep + "test" + sep + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir/1//test.txt") ==
        prefix + "data" + sep + "test" + sep + "test2.zip");

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data/test");
    VirtFs::unmountDir(prefix + "data");
}

TEST_CASE("VirtFs getrealDir3", "")
{
    const std::string sep = dirSeparator;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);
    VirtFs::mountDir(prefix + "data/test", Append_false);

    REQUIRE(VirtFs::getRealDir("zzz").empty());

    REQUIRE(VirtFs::getRealDir("dir1/file1.txt") ==
        prefix + "data" + sep + "test");
    REQUIRE(VirtFs::getRealDir("hide.png") ==
        prefix + "data" + sep + "test");
    REQUIRE(VirtFs::getRealDir("hide.png") ==
        prefix + "data" + sep + "test");
    REQUIRE(VirtFs::getRealDir("1//test.txt") ==
        prefix + "data" + sep + "test" + sep + "test2.zip");

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::unmountDir(prefix + "data/test");
}

TEST_CASE("VirtFs permitLinks1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 27 : 26;
    const int cnt2 = 27;

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

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs permitLinks2", "")
{
    VirtFs::mountDirSilent2("data",
        "test",
        Append_false);
    VirtFs::mountDirSilent2("../data",
        "test",
        Append_false);

    const int cnt1 = VirtFs::exists("test2.txt") ? 27 : 26;
    const int cnt2 = 27;

    StringVect list;
    VirtFs::permitLinks(false);
    VirtFs::getFiles(dirSeparator, list);
    removeTemp(list);
    const size_t sz = list.size();
    REQUIRE(sz == cnt1);

    list.clear();
    VirtFs::permitLinks(true);
    VirtFs::getFiles(dirSeparator, list);
    removeTemp(list);
    REQUIRE(list.size() == cnt2);

    list.clear();
    VirtFs::permitLinks(false);
    VirtFs::getFiles(dirSeparator, list);
    removeTemp(list);
    REQUIRE(list.size() == cnt1);

    VirtFs::unmountDirSilent2("data",
        "test");
    VirtFs::unmountDirSilent2("../data",
        "test");
}

TEST_CASE("VirtFs read1", "")
{
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    VirtFs::File *file = VirtFs::openRead("test/test.txt");
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

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

TEST_CASE("VirtFs read2", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);

    VirtFs::File *file = VirtFs::openRead("dir2/test.txt");
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

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
}

TEST_CASE("VirtFs read3", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip", Append_false);
    VirtFs::mountDir(prefix + "data", Append_false);

    VirtFs::File *file = VirtFs::openRead("dir2/test.txt");
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

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data");
}

TEST_CASE("VirtFs read4", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data/test", Append_true);
    VirtFs::mountZip(prefix + "data/test/test5.zip", Append_true);

    VirtFs::File *file = VirtFs::openRead("dir1/file1.txt");
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

    VirtFs::unmountZip(prefix + "data/test/test5.zip");
    VirtFs::unmountDir(prefix + "data/test");
}

TEST_CASE("VirtFs read5", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test5.zip", Append_true);
    VirtFs::mountDir(prefix + "data/test", Append_true);

    VirtFs::File *file = VirtFs::openRead("dir1/file1.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 3\ntest line 4") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);
    REQUIRE(VirtFs::eof(file) == true);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == false);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 4") == 0);
    REQUIRE(VirtFs::eof(file) == true);

    VirtFs::close(file);
    free(buffer);

    VirtFs::unmountZip(prefix + "data/test/test5.zip");
    VirtFs::unmountDir(prefix + "data/test");
}
