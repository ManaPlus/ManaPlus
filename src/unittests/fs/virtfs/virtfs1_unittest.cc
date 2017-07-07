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

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/fs.h"
#include "fs/virtfs/rwops.h"
#include "fs/virtfs/list.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
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

TEST_CASE("VirtFs1 getEntries", "")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getEntries().empty());
    REQUIRE(VirtFs::searchByRootInternal("test", std::string()) == nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 getBaseDir", "")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getBaseDir() != nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 mountDir", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    SECTION("simple 1")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1") ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1/",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1") ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1" + sep);
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_false));
        REQUIRE(VirtFs::mountDirSilentTest("dir2",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1") ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir2");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1\\",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir2",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir2",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir3/test",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir3" + sep + "test" + sep, std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root == "dir3" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[2]->subDir.empty());
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir3" + sep + "test");
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir2",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest("dir3\\test",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            "dir1" + sep + "", std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir2" + sep + "", std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir3" + sep + "test" + sep, std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "test" + sep + "", std::string()) == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[2]->subDir.empty());
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3" + sep + "test");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir2");
    }

    SECTION("subDir 1")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            dirSeparator,
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("subDir 1.2")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir2" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1" + sep) ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir2" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("subDir 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1/",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir2" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir2" + sep) ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1" + sep);
    }

    SECTION("subDir 3")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir3",
            Append_false));
        REQUIRE(VirtFs::mountDirSilentTest2("dir2",
            "dir4",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir3" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1" + sep) ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, "dir4" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir3" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir4" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir3" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir2");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
    }

    SECTION("subDir 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1\\",
            "dir3",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir2",
            "dir4",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir3" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1" + sep) ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, "dir4" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir3" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir4" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
    }

    SECTION("subDir 5")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir3",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir2",
            "dir4",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir3/test",
            "dir5",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir3" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir2" + sep, "dir4" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir3" + sep + "test" + sep, "dir5" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir3" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root == "dir3" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir4" + sep);
        REQUIRE(VirtFs::getEntries()[2]->subDir == "dir5" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir3" + sep + "test");
    }

    SECTION("subDir 6")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir2",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir3\\test",
            "dir3\\test",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            "dir1" + sep + "", "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir2" + sep + "", "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "dir3" + sep + "test" + sep,
            "dir3" + sep + "test" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            "test" + sep + "", "dir1" + sep) == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir ==
            "dir3" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[2]->subDir == "dir2" + sep);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3" + sep + "test");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir2");
    }

    SECTION("subDir 7")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir2" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir2" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");

        REQUIRE(VirtFs::unmountDirSilent("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("subDir 8")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir2" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, "dir2" + sep) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");

        REQUIRE(VirtFs::unmountDirSilent2("dir1", "dir2"));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("subDir 9")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            dirSeparator,
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("dir1" + sep, "dir1") ==
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal("test" + sep, std::string()) ==
            nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 mountZip", "")
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
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip", std::string()) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test2.zip",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_true));
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test2.zip",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::mountDir(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test2.zip",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir.empty());
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::mountDir(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test2.zip",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir.empty());
    }

    SECTION("subDir 1")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip", "dir1" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("subDir 2")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_false));
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test2.zip",
            "dir2",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip", "dir1" + sep) !=
            nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir1" + sep);
    }

    SECTION("subDir 3")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_true));
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test2.zip",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
    }

    SECTION("subDir 4")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_false));
        REQUIRE(VirtFs::mountDir2(prefix + "data/test",
            "dir2",
            Append_false));
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test2.zip",
            "dir3",
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir3" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir == "dir1" + sep);
    }

    SECTION("subDir 5")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_false));
        REQUIRE(VirtFs::mountDir2(prefix + "data/test",
            "dir2",
            Append_false));
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test2.zip",
            "dir3",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir3" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir == "dir3" + sep);
    }

    SECTION("subDir 6")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            dirSeparator,
            Append_false));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 unmount", "")
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
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE(VirtFs::unmountDir("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2/dir3" + sep));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().empty());
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_true));
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test2.zip",
            Append_true));
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);

        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFs::mountZip(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::mountDir(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::mountZip(prefix + "data\\test/test2.zip",
            Append_false));

        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);

        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            std::string()) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            std::string()) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
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
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE_THROWS(VirtFs::unmountDir2("dir1", "dir1"));
        REQUIRE(VirtFs::unmountDir2("dir1", "dir2"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir4" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir4" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir.empty());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE(VirtFs::unmountDir("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2/dir3" + sep));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir4" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
    }

    SECTION("subDir 4")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::unmountDir2("dir1", "dir2"));
        REQUIRE(VirtFs::getEntries().empty());
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir3",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("subDir 5")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_true));
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test2.zip",
            "dir2",
            Append_true));

        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);

        VirtFs::unmountZip2(prefix + "data/test/test.zip",
            "dir1");
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            std::string()) == nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("subDir 6")
    {
        REQUIRE(VirtFs::mountZip2(prefix + "data/test/test.zip",
            "dir1",
            Append_false));
        REQUIRE(VirtFs::mountDir2(prefix + "data/test",
            "dir2",
            Append_false));
        REQUIRE(VirtFs::mountZip2(prefix + "data\\test/test2.zip",
            "dir3",
            Append_false));

        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir3" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->subDir == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);

        VirtFs::unmountZip2(prefix + "data/test/test.zip",
            "dir1");
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test.zip",
            "dir1" + sep) == nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "test2.zip",
            "dir3" + sep) != nullptr);
        REQUIRE(VirtFs::searchByRootInternal(
            prefix + "data" + sep + "test" + sep + "",
            "dir2" + sep) != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->subDir == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == "dir2" + sep);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 exists1", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent("data/",
        Append_false);
    VirtFs::mountDirSilent("..\\data",
        Append_false);

    REQUIRE(VirtFs::exists("test"));
    REQUIRE(VirtFs::exists("test/"));
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1/"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test//units.xml") == true);
    REQUIRE(VirtFs::exists("test/\\units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

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

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1\\"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    if (dir1 == true)
        VirtFs::unmountDirSilent("data/test");
    else
        VirtFs::unmountDirSilent("../data/test");

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 exists2", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::mountDirSilent2("data/",
        "test",
        Append_false);
    VirtFs::mountDirSilent2("..\\data",
        "test",
        Append_false);

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt") == false);
    REQUIRE(VirtFs::exists("file2.txt") == false);

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

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt"));
    REQUIRE(VirtFs::exists("file2.txt"));

    if (dir1 == true)
        VirtFs::unmountDirSilent2("data/test", "dir2");
    else
        VirtFs::unmountDirSilent2("../data/test", "dir2");

    REQUIRE(VirtFs::exists("test") == false);
    REQUIRE(VirtFs::exists("test/") == false);
    REQUIRE(VirtFs::exists("dir1"));
    REQUIRE(VirtFs::exists("dir1/"));
    REQUIRE(VirtFs::exists("dir") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml"));
    REQUIRE(VirtFs::exists("file1.txt") == false);
    REQUIRE(VirtFs::exists("file2.txt") == false);

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
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

TEST_CASE("VirtFs1 getRealDir1", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    const bool dir1 = VirtFs::mountDirSilent("data",
        Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent("../data",
        Append_false)) == true);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "../data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountDirSilent("data/test",
        Append_false);
    VirtFs::mountDirSilent("../data/test",
        Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "data" + sep + "test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            ".." + sep + "data" + sep + "test");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getRealDir2", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    std::string name("data/test/test.zip");
    std::string prefix("data" + sep + "test" + sep);
    if (Files::existsLocal(name) == false)
        prefix = ".." + sep + prefix;
    VirtFs::mountZip(prefix + "test2.zip",
        Append_false);

    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir/1") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir\\dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir///hide.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("dir\\\\brimmedhat.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountZip(prefix + "test.zip");

    REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png").empty());
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getRealDir3", "")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".").empty());
    REQUIRE(VirtFs::getRealDir("..").empty());
    const bool dir1 = VirtFs::mountDirSilent2("data",
        "test",
        Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent2("../data",
        "test",
        Append_false)) == true);
    REQUIRE(VirtFs::getRealDir("file1.txt").empty());
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("dir1") == "data");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("dir1") == ".." + sep + "data");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::mountDirSilent2("data/test",
        "dir2",
        Append_false);
    VirtFs::mountDirSilent2("../data/test",
        "dir2",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir").empty());
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") == "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") ==
            ".." + sep + "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz").empty());

    VirtFs::deinit();
    delete2(logger);
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
