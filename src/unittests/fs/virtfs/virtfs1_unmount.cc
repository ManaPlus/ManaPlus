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

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("VirtFs1 unmount", "")
{
    VirtFs::init(".");
    std::string name("data/test/test.zip");
    std::string prefix;
    const std::string sep = dirSeparator;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest("dir1",
            Append_true));
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

    SECTION("subDir 2")
    {
        REQUIRE(VirtFs::mountDirSilentTest2("dir1",
            "dir2",
            Append_true));
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
}
