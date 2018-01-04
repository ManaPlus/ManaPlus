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

TEST_CASE("VirtFs1 mountZip1", "")
{
    VirtFs::init(".");
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

    VirtFs::deinit();
}
