/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#include "utils/xmlutils.h"

#include "catch.hpp"
#include "client.h"
#include "logger.h"

#include "utils/physfstools.h"
#include "utils/xml.h"

#include "resources/resourcemanager.h"

#include "debug.h"

TEST_CASE("xmlutils readXmlIntVector 1")
{
    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    logger = new Logger();
    resourceManager->addToSearchPath("data", false);
    resourceManager->addToSearchPath("../data", false);

    std::vector<int> arr;

    readXmlIntVector("graphics/gui/browserbox.xml",
        "skinset",
        "widget",
        "option",
        "value",
        arr);

    REQUIRE(5 == arr.size());
    REQUIRE(1 == arr[0]);
    REQUIRE(15 == arr[1]);
    REQUIRE(0 == arr[2]);
    REQUIRE(1 == arr[3]);
    REQUIRE(1 == arr[4]);
}
