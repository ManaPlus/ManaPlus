/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#include "fs/virtfs/fs.h"

#include "utils/checkutils.h"

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
