/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "fs/virtfs/direntry.h"
#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"

#ifndef UNITTESTS_CATCH
#include <algorithm>
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("VirtFs1 mountDir2", "")
{
    VirtFs::init(".");
    const std::string sep = dirSeparator;
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
}
