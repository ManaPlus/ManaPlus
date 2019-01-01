/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
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

#include "logger.h"

#include "fs/files.h"

#include "fs/virtfs/fszip.h"
#include "fs/virtfs/zipentry.h"
#include "fs/virtfs/zipreader.h"
#include "fs/virtfs/ziplocalheader.h"

#include "debug.h"

extern const char *dirSeparator;

TEST_CASE("Zip readArchiveInfo", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    const std::string sep = dirSeparator;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("test.zip")
    {
        name = prefix + "data/test/test.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 2);
        REQUIRE(entry->root == name);
        REQUIRE(headers[0]->fileName == "dir" + sep + "hide.png");
        REQUIRE(headers[0]->compressSize == 365);
        REQUIRE(headers[0]->uncompressSize == 368);
        REQUIRE(headers[1]->fileName == "dir" + sep + "brimmedhat.png");
        REQUIRE(headers[1]->compressSize == 1959);
        REQUIRE(headers[1]->uncompressSize == 1959);

        delete entry;
    }

    SECTION("test2.zip")
    {
        name = prefix + "data/test/test2.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 11);
        REQUIRE(entry->root == name);
        REQUIRE(headers[0]->fileName == "test.txt");
        REQUIRE(headers[0]->compressSize == 17);
        REQUIRE(headers[0]->uncompressSize == 23);

        REQUIRE(headers[1]->fileName == "dir2" + sep + "hide.png");
        REQUIRE(headers[1]->compressSize == 365);
        REQUIRE(headers[1]->uncompressSize == 368);

        REQUIRE(headers[2]->fileName == "dir2" + sep + "test.txt");
        REQUIRE(headers[2]->compressSize == 17);
        REQUIRE(headers[2]->uncompressSize == 23);

        REQUIRE(headers[3]->fileName == "dir2" + sep + "paths.xml");
        REQUIRE(headers[3]->compressSize == 154);
        REQUIRE(headers[3]->uncompressSize == 185);

        REQUIRE(headers[4]->fileName == "dir2" + sep + "units.xml");
        REQUIRE(headers[4]->compressSize == 202);
        REQUIRE(headers[4]->uncompressSize == 306);

        REQUIRE(headers[5]->fileName == "dir" + sep + "hide.png");
        REQUIRE(headers[5]->compressSize == 365);
        REQUIRE(headers[5]->uncompressSize == 368);

        REQUIRE(headers[6]->fileName ==
            "dir" + sep + "1" + sep + "test.txt");
        REQUIRE(headers[6]->compressSize == 17);
        REQUIRE(headers[6]->uncompressSize == 23);

        REQUIRE(headers[7]->fileName ==
            "dir" + sep + "1" + sep + "file1.txt");
        REQUIRE(headers[7]->compressSize == 17);
        REQUIRE(headers[7]->uncompressSize == 23);

        REQUIRE(headers[8]->fileName ==
            "dir" + sep + "gpl" + sep + "palette.gpl");
        REQUIRE(headers[8]->compressSize == 128);
        REQUIRE(headers[8]->uncompressSize == 213);

        REQUIRE(headers[9]->fileName ==
            "dir" + sep + "dye.png");
        REQUIRE(headers[9]->compressSize == 794);
        REQUIRE(headers[9]->uncompressSize == 794);

        REQUIRE(headers[10]->fileName == "units.xml");
        REQUIRE(headers[10]->compressSize == 202);
        REQUIRE(headers[10]->uncompressSize == 306);

        delete entry;
    }

    SECTION("test3.zip")
    {
        name = prefix + "data/test/test3.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 2);
        REQUIRE(entry->root == name);
        REQUIRE(headers[0]->fileName == "test.txt");
        REQUIRE(headers[0]->compressSize == 17);
        REQUIRE(headers[0]->uncompressSize == 23);
        REQUIRE(headers[1]->fileName == "units.xml");
        REQUIRE(headers[1]->compressSize == 202);
        REQUIRE(headers[1]->uncompressSize == 306);

        delete entry;
    }

    SECTION("test4.zip")
    {
        name = prefix + "data/test/test4.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(entry->root == name);
        REQUIRE(headers.empty());

        delete entry;
    }
}

TEST_CASE("Zip readCompressedFile", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("test2.zip")
    {
        name = prefix + "data/test/test2.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 11);
        REQUIRE(entry->root == name);
        // test.txt
        uint8_t *const buf = VirtFs::ZipReader::readCompressedFile(headers[0]);
        REQUIRE(buf != nullptr);
        delete [] buf;
        delete entry;
    }
}

TEST_CASE("Zip readFile", "")
{
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("test.zip")
    {
        name = prefix + "data/test/test.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 2);
        REQUIRE(entry->root == name);
        for (int f = 0; f < 2; f ++)
        {
            logger->log("test header: %s, %u, %u",
                headers[f]->fileName.c_str(),
                headers[f]->compressSize,
                headers[f]->uncompressSize);
            const uint8_t *const buf = VirtFs::ZipReader::readFile(headers[f]);
            REQUIRE(buf != nullptr);
            delete [] buf;
        }
        delete entry;
    }

    SECTION("test2.zip")
    {
        name = prefix + "data/test/test2.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 11);
        REQUIRE(entry->root == name);
        // test.txt
        const uint8_t *buf = VirtFs::ZipReader::readFile(headers[0]);
        REQUIRE(buf != nullptr);
        const std::string str = std::string(reinterpret_cast<const char*>(buf),
            headers[0]->uncompressSize);
        REQUIRE(str == "test line 1\ntest line 2");
        delete [] buf;
        for (int f = 0; f < 11; f ++)
        {
            logger->log("test header: %s, %u, %u",
                headers[f]->fileName.c_str(),
                headers[f]->compressSize,
                headers[f]->uncompressSize);
            buf = VirtFs::ZipReader::readFile(headers[f]);
            REQUIRE(buf != nullptr);
            delete [] buf;
        }
        delete entry;
    }

    SECTION("test3.zip")
    {
        name = prefix + "data/test/test3.zip";

        VirtFs::ZipEntry *const entry = new VirtFs::ZipEntry(name,
            dirSeparator,
            VirtFs::FsZip::getFuncs());
        STD_VECTOR<VirtFs::ZipLocalHeader*> &headers = entry->mHeaders;

        REQUIRE(VirtFs::ZipReader::readArchiveInfo(entry));
        REQUIRE(headers.size() == 2);
        REQUIRE(entry->root == name);
        for (int f = 0; f < 2; f ++)
        {
            logger->log("test header: %s, %u, %u",
                headers[f]->fileName.c_str(),
                headers[f]->compressSize,
                headers[f]->uncompressSize);
            const uint8_t *const buf = VirtFs::ZipReader::readFile(headers[f]);
            REQUIRE(buf != nullptr);
            delete [] buf;
        }
        delete entry;
    }
}
