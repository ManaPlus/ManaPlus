/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#include "logger.h"

#include "fs/files.h"
#include "fs/zip.h"
#include "fs/ziplocalheader.h"

#include "utils/delete2.h"
#include "utils/dtor.h"

#include "debug.h"

TEST_CASE("Zip readArchiveInfo")
{
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    std::vector<ZipLocalHeader*> headers;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("test 1")
    {
        name = prefix + "data/test/test.zip";

        REQUIRE(Zip::readArchiveInfo(name, headers));
        REQUIRE(headers.size() == 2);
        REQUIRE(headers[0]->archiveName == name);
        REQUIRE(headers[0]->fileName == "dir/hide.png");
        REQUIRE(headers[0]->compressSize == 365);
        REQUIRE(headers[0]->uncompressSize == 368);
        REQUIRE(headers[1]->archiveName == name);
        REQUIRE(headers[1]->fileName == "dir/brimmedhat.png");
        REQUIRE(headers[1]->compressSize == 1959);
        REQUIRE(headers[1]->uncompressSize == 1959);
    }

    SECTION("test 2")
    {
        name = prefix + "data/test/test2.zip";

        REQUIRE(Zip::readArchiveInfo(name, headers));
        REQUIRE(headers.size() == 11);
        REQUIRE(headers[0]->archiveName == name);
        REQUIRE(headers[0]->fileName == "test.txt");
        REQUIRE(headers[0]->compressSize == 17);
        REQUIRE(headers[0]->uncompressSize == 23);

        REQUIRE(headers[1]->archiveName == name);
        REQUIRE(headers[1]->fileName == "dir2/hide.png");
        REQUIRE(headers[1]->compressSize == 365);
        REQUIRE(headers[1]->uncompressSize == 368);

        REQUIRE(headers[2]->archiveName == name);
        REQUIRE(headers[2]->fileName == "dir2/test.txt");
        REQUIRE(headers[2]->compressSize == 17);
        REQUIRE(headers[2]->uncompressSize == 23);

        REQUIRE(headers[3]->archiveName == name);
        REQUIRE(headers[3]->fileName == "dir2/paths.xml");
        REQUIRE(headers[3]->compressSize == 154);
        REQUIRE(headers[3]->uncompressSize == 185);

        REQUIRE(headers[4]->archiveName == name);
        REQUIRE(headers[4]->fileName == "dir2/units.xml");
        REQUIRE(headers[4]->compressSize == 202);
        REQUIRE(headers[4]->uncompressSize == 306);

        REQUIRE(headers[5]->archiveName == name);
        REQUIRE(headers[5]->fileName == "dir/hide.png");
        REQUIRE(headers[5]->compressSize == 365);
        REQUIRE(headers[5]->uncompressSize == 368);

        REQUIRE(headers[6]->archiveName == name);
        REQUIRE(headers[6]->fileName == "dir/1/test.txt");
        REQUIRE(headers[6]->compressSize == 17);
        REQUIRE(headers[6]->uncompressSize == 23);

        REQUIRE(headers[7]->archiveName == name);
        REQUIRE(headers[7]->fileName == "dir/1/file1.txt");
        REQUIRE(headers[7]->compressSize == 17);
        REQUIRE(headers[7]->uncompressSize == 23);

        REQUIRE(headers[8]->archiveName == name);
        REQUIRE(headers[8]->fileName == "dir/gpl/palette.gpl");
        REQUIRE(headers[8]->compressSize == 128);
        REQUIRE(headers[8]->uncompressSize == 213);

        REQUIRE(headers[9]->archiveName == name);
        REQUIRE(headers[9]->fileName == "dir/dye.png");
        REQUIRE(headers[9]->compressSize == 794);
        REQUIRE(headers[9]->uncompressSize == 794);

        REQUIRE(headers[10]->archiveName == name);
        REQUIRE(headers[10]->fileName == "units.xml");
        REQUIRE(headers[10]->compressSize == 202);
        REQUIRE(headers[10]->uncompressSize == 306);
    }

    SECTION("test 3")
    {
        name = prefix + "data/test/test3.zip";

        REQUIRE(Zip::readArchiveInfo(name, headers));
        REQUIRE(headers.size() == 2);
        REQUIRE(headers[0]->archiveName == name);
        REQUIRE(headers[0]->fileName == "test.txt");
        REQUIRE(headers[0]->compressSize == 17);
        REQUIRE(headers[0]->uncompressSize == 23);
        REQUIRE(headers[1]->archiveName == name);
        REQUIRE(headers[1]->fileName == "units.xml");
        REQUIRE(headers[1]->compressSize == 202);
        REQUIRE(headers[1]->uncompressSize == 306);
    }

    SECTION("test 4")
    {
        name = prefix + "data/test/test4.zip";

        REQUIRE(Zip::readArchiveInfo(name, headers));
        REQUIRE(headers.size() == 0);
    }

    delete_all(headers);
    delete2(logger);
}
