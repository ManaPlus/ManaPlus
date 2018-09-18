/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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
#include "fs/virtfs/rwops.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stringutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_rwops.h>
PRAGMA48(GCC diagnostic pop)

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

static bool inList(StringVect list,
                   const std::string &name)
{
    FOR_EACH (StringVectCIter, it, list)
    {
        if (*it == name)
            return true;
    }
    return false;
}

static bool inList(StringVect list,
                   const std::string &dir,
                   const std::string &name)
{
    const std::string path = pathJoin(dir, name);
    FOR_EACH (StringVectCIter, it, list)
    {
        if (*it == path)
            return true;
    }
    return false;
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

TEST_CASE("VirtFs2 isDirectory1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

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
    REQUIRE(VirtFs::isDirectory("test\\dir1/") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::mountDir(prefix + "data/test",
        Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);
    REQUIRE(VirtFs::isDirectory("test\\dir1") == true);

    VirtFs::unmountDir(prefix + "data/test");

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::unmountDirSilent(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 isDirectory2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == true);
    REQUIRE(VirtFs::isDirectory("dir2//") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1") == true);
    REQUIRE(VirtFs::isDirectory("dir\\1/") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1\\") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::mountZip(prefix + "data/test/test.zip",
        Append_false);

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2\\units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == true);
    REQUIRE(VirtFs::isDirectory("dir2\\") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1/") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//") == false);
    REQUIRE(VirtFs::isDirectory("dir/1") == false);
    REQUIRE(VirtFs::isDirectory("dir\\1") == false);
    REQUIRE(VirtFs::isDirectory("dir//1/") == false);
    REQUIRE(VirtFs::isDirectory("dir/1") == false);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::unmountZip(prefix + "data/test/test.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 isDirectory3", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir2(prefix + "data",
        "test",
        Append_false);

    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == false);
    REQUIRE(VirtFs::isDirectory("dir1") == true);
    REQUIRE(VirtFs::isDirectory("dir2//") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::unmountDirSilent2(prefix + "data",
        "test");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 isDirectory4", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("1") == true);
    REQUIRE(VirtFs::isDirectory("gpl") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == false);
    REQUIRE(VirtFs::isDirectory("dir/1") == false);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1\\") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 openRead1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

    VirtFs::File *file = nullptr;

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::mountDir(prefix + "data/test",
        Append_false);

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
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::unmountDir(prefix + "data/test");

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::unmountDir(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 openRead2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);

    VirtFs::File *file = nullptr;

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir//brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2//units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);

    VirtFs::unmountZip(prefix + "test.zip");

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2\\/\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::unmountZip(prefix + "test2.zip");

    VirtFs::deinit();
}

TEST_CASE("VirtFs2 openRead3", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir2(prefix + "data",
        "test",
        Append_false);

    VirtFs::File *file = nullptr;

    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("file1.txt");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("file2.txt");
    REQUIRE(file == nullptr);

    VirtFs::mountDir2(prefix + "data/test",
        "dir2",
        Append_false);

    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("file1.txt");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("file2.txt");
    REQUIRE(file != nullptr);
    VirtFs::close(file);

    VirtFs::unmountDir2(prefix + "data/test",
        "dir2");

    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("file1.txt");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("file2.txt");
    REQUIRE(file == nullptr);

    VirtFs::unmountDir2(prefix + "data",
        "test");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 openRead4", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "test2.zip",
        "dir",
        Append_false);

    VirtFs::File *file = nullptr;

    file = VirtFs::openRead("dye.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("1\\test.txt");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/dye.png");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dye.png1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::mountZip2(prefix + "test.zip",
        "dir",
        Append_false);

    file = VirtFs::openRead("dye.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("1\\test.txt");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/dye.png");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dye.png1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("brimmedhat.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);

    VirtFs::unmountZip2(prefix + "test.zip",
        "dir");

    file = VirtFs::openRead("dye.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("1\\test.txt");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir/dye.png");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dye.png1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::unmountZip2(prefix + "test2.zip",
        "dir");

    VirtFs::deinit();
}

TEST_CASE("VirtFs2 permitLinks", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

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

    VirtFs::unmountDirSilent(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 read1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

    VirtFs::File *file = VirtFs::openRead("test/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);
    REQUIRE(VirtFs::eof(file) != 0);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == 0);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFs::eof(file) != 0);

    VirtFs::close(file);
    free(buffer);

    VirtFs::unmountDir(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 read2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);
    VirtFs::File *file = nullptr;
    void *restrict buffer = nullptr;

    SECTION("test 1")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFs::fileLength(file) == 23);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 1\ntest line 2") == 0);
        REQUIRE(VirtFs::tell(file) == fileSize);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    SECTION("test 2")
    {
        file = VirtFs::openRead("dir2\\/test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFs::fileLength(file) == 23);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFs::seek(file, 12) != 0);
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 12);
        REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 2") == 0);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    SECTION("test 3")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize; f ++)
        {
            REQUIRE(VirtFs::seek(file, f) != 0);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f);
        }
    }

    SECTION("test 4")
    {
        file = VirtFs::openRead("dir2/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f ++)
        {
            REQUIRE(VirtFs::read(file, buffer, 1, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 1);
        }
        REQUIRE(VirtFs::read(file, buffer, 1, 1) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFs::eof(file) != 0);
        REQUIRE(VirtFs::tell(file) == fileSize);
    }

    SECTION("test 5")
    {
        file = VirtFs::openRead("dir2\\\\test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFs::read(file, buffer, 2, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 2);
        }
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 22);
        REQUIRE(VirtFs::read(file, buffer, 2, 1) == 0);
        REQUIRE(VirtFs::eof(file) == 0);
    }

    SECTION("test 6")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFs::read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 2);
        }
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 22);
        REQUIRE(VirtFs::read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    VirtFs::close(file);
    free(buffer);
    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 loadFile1", "")
{
    VirtFs::init(".");
    int fileSize = 0;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

    const char *const buffer = VirtFs::loadFile("test/test.txt", fileSize);
    REQUIRE(static_cast<const void*>(buffer) != nullptr);
    REQUIRE(fileSize == 23);
    REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
    delete [] buffer;

    VirtFs::unmountDir(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 loadFile2", "")
{
    VirtFs::init(".");
    int fileSize = 0;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    SECTION("test 1")
    {
        const char *restrict buffer = VirtFs::loadFile("dir2//test.txt",
            fileSize);
        REQUIRE(static_cast<const void*>(buffer) != nullptr);
        REQUIRE(fileSize == 23);
        REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
        delete [] buffer;
    }

    SECTION("test 2")
    {
        const char *restrict buffer = VirtFs::loadFile("dir2\\/test.txt",
            fileSize);
        REQUIRE(static_cast<const void*>(buffer) != nullptr);
        REQUIRE(fileSize == 23);
        REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
        delete [] buffer;
    }

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 loadFile3", "")
{
    VirtFs::init(".");
    int fileSize = 0;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir2(prefix + "data",
        "test",
        Append_false);

    const char *const buffer = VirtFs::loadFile("test.txt", fileSize);
    REQUIRE(static_cast<const void*>(buffer) != nullptr);
    REQUIRE(fileSize == 23);
    REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
    delete [] buffer;

    VirtFs::unmountDir2(prefix + "data",
        "test");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 loadFile4", "")
{
    VirtFs::init(".");
    int fileSize = 0;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir2",
        Append_false);

    SECTION("test 1")
    {
        const char *restrict buffer = VirtFs::loadFile("test.txt",
            fileSize);
        REQUIRE(static_cast<const void*>(buffer) != nullptr);
        REQUIRE(fileSize == 23);
        REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
        delete [] buffer;
    }

    SECTION("test 2")
    {
        const char *restrict buffer = VirtFs::loadFile("test.txt",
            fileSize);
        REQUIRE(static_cast<const void*>(buffer) != nullptr);
        REQUIRE(fileSize == 23);
        REQUIRE(strncmp(buffer, "test line 1\ntest line 2", 23) == 0);
        delete [] buffer;
    }

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir2");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 rwops_read1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

    SDL_RWops *file = VirtFs::rwopsOpenRead("test/test.txt");
    REQUIRE(file != nullptr);
#ifdef USE_SDL2
    REQUIRE(file->size(file) == 23);
#endif  // USE_SDL2

    const int fileSize = 23;

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(file->read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(file->seek(file, 0, SEEK_CUR) == fileSize);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(file->seek(file, 12, SEEK_SET) != 0);
    REQUIRE(file->seek(file, 0, SEEK_CUR) == 12);
    REQUIRE(file->read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);

    file->close(file);
    free(buffer);

    VirtFs::unmountDir(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 rwops_read2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);
    SDL_RWops *file = nullptr;
    void *restrict buffer = nullptr;

    SECTION("test 1")
    {
        file = VirtFs::rwopsOpenRead("dir2//test.txt");
        REQUIRE(file != nullptr);
#ifdef USE_SDL2
        REQUIRE(file->size(file) == 23);
#endif  // USE_SDL2
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(file->read(file, buffer, 1, fileSize) == fileSize);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 1\ntest line 2") == 0);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == fileSize);
    }

    SECTION("test 2")
    {
        file = VirtFs::rwopsOpenRead("dir2\\/test.txt");
        REQUIRE(file != nullptr);
#ifdef USE_SDL2
        REQUIRE(file->size(file) == 23);
#endif  // USE_SDL2
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(file->seek(file, 12, SEEK_SET) != 0);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 12);
        REQUIRE(file->read(file, buffer, 1, 11) == 11);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 2") == 0);
    }

    SECTION("test 3")
    {
        file = VirtFs::rwopsOpenRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize; f ++)
        {
            REQUIRE(file->seek(file, f, SEEK_SET) == f);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f);
        }
    }

    SECTION("test 4")
    {
        file = VirtFs::rwopsOpenRead("dir2/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f ++)
        {
            REQUIRE(file->read(file, buffer, 1, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 1);
        }
        REQUIRE(file->read(file, buffer, 1, 1) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == fileSize);
    }

    SECTION("test 5")
    {
        file = VirtFs::rwopsOpenRead("dir2\\\\test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 2, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 2, 1) == 0);
    }

    SECTION("test 6")
    {
        file = VirtFs::rwopsOpenRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
    }

    SECTION("test 7")
    {
        file = VirtFs::rwopsOpenRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(file->seek(file, -2, SEEK_CUR) == f);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f);
            REQUIRE(file->seek(file, 2, SEEK_CUR) == f + 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
    }

    SECTION("test 8")
    {
        file = VirtFs::rwopsOpenRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->seek(file, -f - 2, SEEK_END) == 23 - f - 2);
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[23 - f - 2]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[23 - f - 2 + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == 23 - f);
        }
        // 3
        REQUIRE(file->seek(file, 1, SEEK_CUR) == 4);
        // 4
        REQUIRE(file->read(file, buffer, 1, 2) == 2);
        // 6
        REQUIRE(static_cast<char*>(buffer)[0] == str[4]);
        REQUIRE(static_cast<char*>(buffer)[1] == str[5]);
        REQUIRE(file->seek(file, -7, SEEK_CUR) == -1);
        REQUIRE(file->seek(file, 6, SEEK_SET) == 6);
        REQUIRE(file->seek(file, -6, SEEK_CUR) == 0);
    }

    if (file != nullptr)
        file->close(file);
    free(buffer);
    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 rwops_read3", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);
    SDL_RWops *file = nullptr;
    void *restrict buffer = nullptr;

    SECTION("test 1")
    {
        file = VirtFs::rwopsOpenRead("test/test.txt");
        REQUIRE(file != nullptr);
#ifdef USE_SDL2
        REQUIRE(file->size(file) == 23);
#endif  // USE_SDL2
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(file->read(file, buffer, 1, fileSize) == fileSize);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 1\ntest line 2") == 0);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == fileSize);
    }

    SECTION("test 2")
    {
        file = VirtFs::rwopsOpenRead("test\\test.txt");
        REQUIRE(file != nullptr);
#ifdef USE_SDL2
        REQUIRE(file->size(file) == 23);
#endif  // USE_SDL2
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(file->seek(file, 12, SEEK_SET) != 0);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 12);
        REQUIRE(file->read(file, buffer, 1, 11) == 11);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 2") == 0);
    }

    SECTION("test 3")
    {
        file = VirtFs::rwopsOpenRead("test\\/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;

        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize; f ++)
        {
            REQUIRE(file->seek(file, f, SEEK_SET) == f);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f);
        }
    }

    SECTION("test 4")
    {
        file = VirtFs::rwopsOpenRead("test/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f ++)
        {
            REQUIRE(file->read(file, buffer, 1, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 1);
        }
        REQUIRE(file->read(file, buffer, 1, 1) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(file->seek(file, 0, SEEK_CUR) == fileSize);
    }

    SECTION("test 5")
    {
        file = VirtFs::rwopsOpenRead("test///test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 2, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 2, 1) == 0);
    }

    SECTION("test 6")
    {
        file = VirtFs::rwopsOpenRead("test\\\\test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
    }

    SECTION("test 7")
    {
        file = VirtFs::rwopsOpenRead("test//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(file->seek(file, -2, SEEK_CUR) == f);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f);
            REQUIRE(file->seek(file, 2, SEEK_CUR) == f + 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == f + 2);
        }
        REQUIRE(file->seek(file, 0, SEEK_CUR) == 22);
        REQUIRE(file->read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
    }

    SECTION("test 8")
    {
        file = VirtFs::rwopsOpenRead("test/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = 23;
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(file->seek(file, -f - 2, SEEK_END) == 23 - f - 2);
            REQUIRE(file->read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[23 - f - 2]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[23 - f - 2 + 1]);
            REQUIRE(file->seek(file, 0, SEEK_CUR) == 23 - f);
        }
        // 3
        REQUIRE(file->seek(file, 1, SEEK_CUR) == 4);
        // 4
        REQUIRE(file->read(file, buffer, 1, 2) == 2);
        // 6
        REQUIRE(static_cast<char*>(buffer)[0] == str[4]);
        REQUIRE(static_cast<char*>(buffer)[1] == str[5]);
        REQUIRE(file->seek(file, -7, SEEK_CUR) == -1);
        REQUIRE(file->seek(file, 6, SEEK_SET) == 6);
        REQUIRE(file->seek(file, -6, SEEK_CUR) == 0);
    }

    if (file != nullptr)
        file->close(file);
    free(buffer);
    VirtFs::unmountDir(prefix + "data");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFiles zip1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    StringVect list;
    VirtFs::getFiles("dir", list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "dye.png"));
    REQUIRE(inList(list, "hide.png"));

    list.clear();
    VirtFs::getFiles("dir2", list);
    REQUIRE(list.size() == 4);
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "paths.xml"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFiles zip2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);

    StringVect list;
    VirtFs::getFiles(dirSeparator, list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "dye.png"));
    REQUIRE(inList(list, "hide.png"));

    list.clear();
    VirtFs::getFiles("1", list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "test.txt"));

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getDirs1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    StringVect list;
    VirtFs::getDirs("dir", list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "1"));
    REQUIRE(inList(list, "gpl"));
    list.clear();

    VirtFs::getDirs("dir2", list);
    REQUIRE(list.empty());

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getDirs2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    StringVect list;

    SECTION("dir1")
    {
        VirtFs::mountDir(prefix + "data/test",
            Append_false);

        VirtFs::getDirs("/", list);
//        REQUIRE(list.size() == 2);
        REQUIRE(inList(list, "dir1"));
        REQUIRE(inList(list, "dir2"));
        list.clear();

        VirtFs::getDirs("dir1", list);
        REQUIRE(list.empty());

        VirtFs::unmountDir(prefix + "data/test");
    }

    SECTION("dir2")
    {
        VirtFs::mountDir(prefix + "data",
            Append_false);

        VirtFs::getDirs("sfx", list);
        REQUIRE(inList(list, "system"));
        list.clear();

        VirtFs::getDirs("evol", list);
        REQUIRE(list.size() == 2);
        REQUIRE(inList(list, "icons"));
        REQUIRE(inList(list, "images"));

        VirtFs::unmountDir(prefix + "data");
    }

    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getDirs3", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);

    StringVect list;
    VirtFs::getDirs(dirSeparator, list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "1"));
    REQUIRE(inList(list, "gpl"));
    list.clear();

    VirtFs::getDirs("1", list);
    REQUIRE(list.empty());

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getDirs4", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    StringVect list;

    SECTION("dir1")
    {
        VirtFs::mountDir2(prefix + "data",
            "test",
            Append_false);

        VirtFs::getDirs("/", list);
        REQUIRE(inList(list, "dir1"));
        REQUIRE(inList(list, "dir2"));
        list.clear();

        VirtFs::getDirs("dir1", list);
        REQUIRE(list.empty());

        VirtFs::unmountDir2(prefix + "data",
            "test");
    }

    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFilesWithDir1", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip(prefix + "data/test/test2.zip",
        Append_false);

    StringVect list;
    VirtFs::getFilesWithDir("dir", list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "dir", "dye.png"));
    REQUIRE(inList(list, "dir", "hide.png"));
    list.clear();

    VirtFs::getFilesWithDir("dir2", list);
    REQUIRE(list.size() == 4);
    REQUIRE(inList(list, "dir2", "hide.png"));
    REQUIRE(inList(list, "dir2", "paths.xml"));
    REQUIRE(inList(list, "dir2", "test.txt"));
    REQUIRE(inList(list, "dir2", "units.xml"));
    list.clear();

    VirtFs::getFilesWithDir(dirSeparator, list);
    REQUIRE(list.size() > 2);
    REQUIRE(inList(list, dirSeparator, "test.txt"));
    REQUIRE(inList(list, dirSeparator, "units.xml"));
    list.clear();

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFilesWithDir2", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    StringVect list;

    SECTION("dir1")
    {
        VirtFs::mountDir(prefix + "data/graphics",
            Append_false);

        VirtFs::getFilesWithDir("/", list);
        REQUIRE(list.size() <= 5);
        VirtFs::unmountDir(prefix + "data/graphics");
    }

    SECTION("dir2")
    {
        VirtFs::mountDir(prefix + "data",
            Append_false);

        VirtFs::getFilesWithDir("music", list);
        REQUIRE(list.size() <= 5);
        REQUIRE(!list.empty());
        REQUIRE(inList(list, "music", "keprohm.ogg"));
        list.clear();

        VirtFs::getFilesWithDir(pathJoin("evol", "icons"), list);
        REQUIRE(list.size() == 3);
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.ico"));
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.png"));
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.xpm"));

        VirtFs::unmountDir(prefix + "data");
    }

    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFilesWithDir3", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountZip2(prefix + "data/test/test2.zip",
        "dir",
        Append_false);

    StringVect list;
    VirtFs::getFilesWithDir("1", list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, "1", "file1.txt"));
    REQUIRE(inList(list, "1", "test.txt"));
    list.clear();

    VirtFs::getFilesWithDir(dirSeparator, list);
    REQUIRE(list.size() == 2);
    REQUIRE(inList(list, dirSeparator, "dye.png"));
    REQUIRE(inList(list, dirSeparator, "hide.png"));
    list.clear();

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::deinit();
}

TEST_CASE("VirtFs2 getFilesWithDir4", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    StringVect list;

    SECTION("dir1")
    {
        VirtFs::mountDir2(prefix + "data/graphics",
            "sprites",
            Append_false);

        VirtFs::getFilesWithDir("/", list);
        REQUIRE(list.size() <= 16);
        VirtFs::unmountDir2(prefix + "data/graphics",
            "sprites");
    }

    SECTION("dir2")
    {
        VirtFs::mountDir2(prefix + "data",
            "test",
            Append_false);

        VirtFs::getFilesWithDir("dir1", list);
        REQUIRE(list.size() <= 6);
        REQUIRE(!list.empty());
        REQUIRE(inList(list, "dir1", "file1.txt"));
        list.clear();

        VirtFs::unmountDir2(prefix + "data",
            "test");
    }

    VirtFs::deinit();
}
