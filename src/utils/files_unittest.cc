/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "catch.hpp"
#include "logger.h"

#include "utils/physfstools.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("Files renameFile")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    const int sz = 1234567;
    char *buf = new char[sz];
    for (int f = 0; f < sz; f ++)
        buf[f] = f;

    const std::string name1 = "file1.test";
    const std::string name2 = "file2.test";
    FILE *file = fopen(name1.c_str(), "w+b");
    fwrite(buf, 1, sz, file);
    fclose(file);

    REQUIRE(0 == Files::renameFile(name1, name2));
    char *buf2 = new char[sz];
    FILE *file2 = fopen(name2.c_str(), "rb");
    REQUIRE_FALSE(nullptr == file2);
    fread(buf2, 1, sz, file2);
    fclose(file2);
    ::remove(name1.c_str());
    ::remove(name2.c_str());

    for (int f = 0; f < sz; f ++)
        REQUIRE(buf[f] == buf2[f]);

    delete [] buf;
    delete [] buf2;
    ResourceManager::deleteInstance();
}

TEST_CASE("Files existsLocal")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);
    REQUIRE(Files::existsLocal(Files::getPath("help/about.txt")) == true);
    REQUIRE_FALSE(Files::existsLocal(Files::getPath("help/about1.txt")));
    REQUIRE_FALSE(Files::existsLocal(Files::getPath("help1/about.txt")));
    ResourceManager::deleteInstance();
}

TEST_CASE("Files loadTextFileString")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);
    REQUIRE(Files::loadTextFileString("test/simplefile.txt") ==
        "this is test \nfile.");
    ResourceManager::deleteInstance();
}

TEST_CASE("Files loadTextFile")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    StringVect lines;
    Files::loadTextFile("test/simplefile.txt", lines);
    REQUIRE(lines.size() == 2);
    REQUIRE(lines[0] == "this is test ");
    REQUIRE(lines[1] == "file.");
    ResourceManager::deleteInstance();
}

TEST_CASE("Files saveTextFile")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    const std::string dir = Files::getPath("test");
    REQUIRE(dir.size() > 0);
    Files::saveTextFile(dir, "tempfile.txt", "test line\ntext line2");
    std::string data = Files::loadTextFileString("test/tempfile.txt");
    ::remove((dir + "/tempfile.txt").c_str());
    REQUIRE(data == "test line\ntext line2\n");
    ResourceManager::deleteInstance();
}

TEST_CASE("Files getFilesInDir")
{
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    StringVect list;
    Files::getFilesInDir("test",
        list,
        ".gpl");
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == "test/palette.gpl");

    list.clear();
    Files::getFilesInDir("perserver/default",
        list,
        ".xml");
    REQUIRE(list.size() == 5);
    REQUIRE(list[0] == "perserver/default/charcreation.xml");
    REQUIRE(list[1] == "perserver/default/deadmessages.xml");
    REQUIRE(list[2] == "perserver/default/defaultcommands.xml");
    REQUIRE(list[3] == "perserver/default/features.xml");
    REQUIRE(list[4] == "perserver/default/weapons.xml");
    ResourceManager::deleteInstance();
}
