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
#include "fs/virtfs/list.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/foreach.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

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

TEST_CASE("VirtFs1 enumerateFiles1", "")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::mountDirSilent("data",
        Append_false);
    VirtFs::mountDirSilent("../data",
        Append_false);

    VirtFs::List *list = nullptr;

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 28 : 27;
    const int cnt2 = 28;

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test");
    removeTemp(list->names);
    const size_t sz = list->names.size();
    REQUIRE(sz == cnt1);
    VirtFs::freeList(list);

    VirtFs::permitLinks(true);
    list = VirtFs::enumerateFiles("test/");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt2);
    VirtFs::freeList(list);

    VirtFs::permitLinks(true);
    list = VirtFs::enumerateFiles("test/units.xml");
    REQUIRE(list->names.empty());
    VirtFs::freeList(list);

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test\\");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt1);
    VirtFs::freeList(list);

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles2", "")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::mountDirSilent("data/test/dir1",
        Append_false);
    VirtFs::mountDirSilent("../data/test/dir1",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 5);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE_FALSE(inList(list, "file2.txt"));
    VirtFs::freeList(list);
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles3", "")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::mountDirSilent("data/test/dir1",
        Append_false);
    VirtFs::mountDirSilent("../data/test/dir1",
        Append_false);
    VirtFs::mountDirSilent("data/test/dir2",
        Append_false);
    VirtFs::mountDirSilent("../data/test/dir2",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFs::freeList(list);
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles4", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir");
    REQUIRE(list->names.size() == 2);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "test.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles5", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data//test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test.zip",
        Append_true);
    VirtFs::mountZip(prefix + "test2.zip",
        Append_true);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir");
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
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "test.zip");
    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles6", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 1);
    REQUIRE(inList(list, "dir"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "test.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles7", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 4);
    REQUIRE(inList(list, "dir"));
    REQUIRE(inList(list, "dir2"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles8", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);
    VirtFs::mountDirSilent(prefix + "data/test",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir2");
    REQUIRE(list->names.size() >= 6);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "paths.xml"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data/test");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles9", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);
    VirtFs::mountDirSilent(prefix + "data/test",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir");
    REQUIRE(list->names.size() == 4);
    REQUIRE(inList(list, "1"));
    REQUIRE(inList(list, "gpl"));
    REQUIRE(inList(list, "dye.png"));
    REQUIRE(inList(list, "hide.png"));
    VirtFs::freeList(list);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::unmountDir(prefix + "data/test");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles10", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "test.zip",
        "dir",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 2);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    VirtFs::freeList(list);

    VirtFs::unmountZip2(prefix + "test.zip",
        "dir");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles11", "")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "test2.zip",
        "dir",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("1");
    REQUIRE(list->names.size() == 2);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "test.txt"));
    VirtFs::freeList(list);

    VirtFs::unmountZip2(prefix + "test2.zip",
        "dir");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles12", "")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::mountDirSilent2("data/test",
        "dir2",
        Append_false);
    VirtFs::mountDirSilent2("../data/test",
        "dir2",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFs::freeList(list);

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles13", "")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::mountDirSilent2("data",
        "test",
        Append_false);
    VirtFs::mountDirSilent2("../data",
        "test",
        Append_false);

    VirtFs::List *list = nullptr;

    list = VirtFs::enumerateFiles("dir2");
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFs::freeList(list);

    VirtFs::deinit();
    delete2(logger);
}
