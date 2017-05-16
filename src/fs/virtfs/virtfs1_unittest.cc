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

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/fs.h"
#include "fs/virtfs/rwops.h"
#include "fs/virtfs/list.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/stringutils.h"

#include <SDL_rwops.h>

#include "debug.h"

TEST_CASE("VirtFs1 getEntries")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getEntries().empty());
    REQUIRE(VirtFs::searchByRootInternal("test", std::string()) == nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 getBaseDir")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getBaseDir() != nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 mountDir")
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[2]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[2]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 mountZip")
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 unmount")
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE_THROWS(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2" + sep + "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2" + sep + "dir3");
        REQUIRE(VirtFs::unmountDir("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::unmountDir("dir2/dir3" + sep));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtFs::DirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE_THROWS(VirtFs::unmountDir("dir2"));
        REQUIRE(VirtFs::unmountDir("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 0);
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1" + sep);
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep + "test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data" + sep + "test" + sep + "test.zip");
        REQUIRE(VirtFs::getEntries()[2]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[0]->subDir == std::string());
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data" + sep + "test" + sep);
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries()[1]->subDir == std::string());
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
        REQUIRE(VirtFs::getEntries().size() == 0);
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

TEST_CASE("VirtFs1 exists1")
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

TEST_CASE("VirtFs1 exists2")
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

TEST_CASE("VirtFs1 getRealDir1")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
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
    REQUIRE(VirtFs::getRealDir("zzz") == "");

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
    REQUIRE(VirtFs::getRealDir("zzz") == "");

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
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getRealDir2")
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

    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir/1") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir\\dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::mountZip(prefix + "test.zip",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir///hide.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("dir\\\\brimmedhat.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::unmountZip(prefix + "test.zip");

    REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") == "");
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::unmountZip(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getRealDir3")
{
    VirtFs::init(".");
    logger = new Logger();
    const std::string sep = dirSeparator;
    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
    const bool dir1 = VirtFs::mountDirSilent2("data",
        "test",
        Append_false);
    REQUIRE((dir1 || VirtFs::mountDirSilent2("../data",
        "test",
        Append_false)) == true);
    REQUIRE(VirtFs::getRealDir("file1.txt") == "");
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
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::mountDirSilent2("data/test",
        "dir2",
        Append_false);
    VirtFs::mountDirSilent2("../data/test",
        "dir2",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir") == "");
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") == "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == "data" + sep + "test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("file1.txt") ==
            ".." + sep + "data" + sep + "test");
        REQUIRE(VirtFs::getRealDir("simplefile.txt") == ".." + sep + "data");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

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

TEST_CASE("VirtFs1 enumerateFiles1")
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
    REQUIRE(list->names.size() == 0);
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

TEST_CASE("VirtFs1 enumerateFiles2")
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

TEST_CASE("VirtFs1 enumerateFiles3")
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
    const size_t sz = list->names.size();
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFs::freeList(list);
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles4")
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

TEST_CASE("VirtFsZip enumerateFiles5")
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

TEST_CASE("VirtFsZip enumerateFiles6")
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

TEST_CASE("VirtFsZip enumerateFiles7")
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

TEST_CASE("VirtFsZip enumerateFiles8")
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

TEST_CASE("VirtFsZip enumerateFiles9")
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

TEST_CASE("VirtFsZip enumerateFiles10")
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

TEST_CASE("VirtFsZip enumerateFiles11")
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

TEST_CASE("VirtFs1 enumerateFiles12")
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

TEST_CASE("VirtFs1 enumerateFiles13")
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


TEST_CASE("VirtFs1 isDirectory1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 isDirectory2")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 isDirectory3")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 isDirectory4")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead2")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead3")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead4")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 permitLinks")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDir(prefix + "data",
        Append_false);

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

    VirtFs::unmountDirSilent(prefix + "data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 read1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 read2")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 loadFile1")
{
    VirtFs::init(".");
    int fileSize = 0;
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 loadFile2")
{
    VirtFs::init(".");
    int fileSize = 0;
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 loadFile3")
{
    VirtFs::init(".");
    int fileSize = 0;
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 loadFile4")
{
    VirtFs::init(".");
    int fileSize = 0;
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 rwops_read1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 rwops_read2")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 rwops_read3")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 getFiles zip1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 getFiles zip2")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 getDirs1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    REQUIRE(list.size() == 0);

    VirtFs::unmountZip(prefix + "data/test/test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getDirs2")
{
    VirtFs::init(".");
    logger = new Logger();
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
        REQUIRE(list.size() == 0);

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
    delete2(logger);
}

TEST_CASE("VirtFs1 getDirs3")
{
    VirtFs::init(".");
    logger = new Logger();
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
    REQUIRE(list.size() == 0);

    VirtFs::unmountZip2(prefix + "data/test/test2.zip",
        "dir");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getDirs4")
{
    VirtFs::init(".");
    logger = new Logger();
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
        REQUIRE(list.size() == 0);

        VirtFs::unmountDir2(prefix + "data",
            "test");
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getFilesWithDir1")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 getFilesWithDir2")
{
    VirtFs::init(".");
    logger = new Logger();
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
        REQUIRE(list.size() >= 1);
        REQUIRE(inList(list, "music", "keprohm.ogg"));
        list.clear();

        VirtFs::getFilesWithDir(pathJoin("evol", "icons"), list);
#ifdef WIN32
        REQUIRE(list.size() == 1);
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.ico"));
#else  // WIN32

        REQUIRE(list.size() == 3);
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.ico"));
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.png"));
        REQUIRE(inList(list, pathJoin("evol", "icons"), "evol-client.xpm"));
#endif  // WIN32

        VirtFs::unmountDir(prefix + "data");
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getFilesWithDir3")
{
    VirtFs::init(".");
    logger = new Logger();
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
    delete2(logger);
}

TEST_CASE("VirtFs1 getFilesWithDir4")
{
    VirtFs::init(".");
    logger = new Logger();
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
        REQUIRE(list.size() >= 1);
        REQUIRE(inList(list, "dir1", "file1.txt"));
        list.clear();

        VirtFs::unmountDir2(prefix + "data",
            "test");
    }

    VirtFs::deinit();
    delete2(logger);
}
