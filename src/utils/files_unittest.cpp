/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#include "utils/files.h"

#include "logger.h"

#include "utils/physfstools.h"

#include "gtest/gtest.h"

#include "resources/resourcemanager.h"

#include "debug.h"

static void init()
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager *resman = ResourceManager::getInstance();
    resman->addToSearchPath("data", false);
    resman->addToSearchPath("../data", false);
}

TEST(Files, renameFile)
{
    init();
    const int sz = 1234567;
    char *buf = new char[sz];
    for (int f = 0; f < sz; f ++)
        buf[f] = f;

    const std::string name1 = "file1.test";
    const std::string name2 = "file2.test";
    FILE *file = fopen(name1.c_str(), "w+b");
    fwrite(buf, 1, sz, file);
    fclose(file);

    EXPECT_EQ(0, Files::renameFile(name1, name2));
    char *buf2 = new char[sz];
    FILE *file2 = fopen(name2.c_str(), "rb");
    EXPECT_NE(nullptr, file2);
    fread(buf2, 1, sz, file2);
    fclose(file2);
    ::remove(name1.c_str());
    ::remove(name2.c_str());

    for (int f = 0; f < sz; f ++)
        EXPECT_EQ(buf[f], buf2[f]);

    delete [] buf;
    delete [] buf2;
}
