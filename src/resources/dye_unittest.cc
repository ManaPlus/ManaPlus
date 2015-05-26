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

#include "catch.hpp"

#include "resources/dye.h"

#include "resources/dyepalette.h"

#include "debug.h"

TEST_CASE("Dye replaceSOGLColor 1")
{
    DyePalette palette("#00ff00,000011", 6);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x02;
    data[2] = 0x03;
    data[3] = 0x10;
    palette.replaceSOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x01 == data[0]);
    REQUIRE(0x02 == data[1]);
    REQUIRE(0x03 == data[2]);
    REQUIRE(0x10 == data[3]);
}

TEST_CASE("Dye replaceSOGLColor 2")
{
    DyePalette palette("#01ff02,030411", 6);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0xff;
    data[2] = 0x02;
    data[3] = 0x20;
    palette.replaceSOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x03 == data[0]);
    REQUIRE(0x04 == data[1]);
    REQUIRE(0x11 == data[2]);
    REQUIRE(0x20 == data[3]);
}

TEST_CASE("Dye replaceSOGLColor 3")
{
    DyePalette palette("#404040,200000,0100ee,102030", 6);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0xee;
    data[3] = 0x40;
    palette.replaceSOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x10 == data[0]);
    REQUIRE(0x20 == data[1]);
    REQUIRE(0x30 == data[2]);
    REQUIRE(0x40 == data[3]);
}


TEST_CASE("Dye replaceAOGLColor 1")
{
    DyePalette palette("#00ff0010,00001120", 8);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x02;
    data[2] = 0x03;
    data[3] = 0x10;
    palette.replaceAOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x01 == data[0]);
    REQUIRE(0x02 == data[1]);
    REQUIRE(0x03 == data[2]);
    REQUIRE(0x10 == data[3]);
}

TEST_CASE("Dye replaceAOGLColor 2")
{
    DyePalette palette("#00ff0120,020311ff", 8);
    uint8_t data[4];
    data[0] = 0x00;
    data[1] = 0xff;
    data[2] = 0x01;
    data[3] = 0x20;
    palette.replaceAOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x02 == data[0]);
    REQUIRE(0x03 == data[1]);
    REQUIRE(0x11 == data[2]);
    REQUIRE(0xff == data[3]);
}

TEST_CASE("Dye replaceAOGLColor 3")
{
    DyePalette palette("#40404040,20000000,0100ee40,102030ff", 8);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x00;
    data[2] = 0xee;
    data[3] = 0x40;
    palette.replaceAOGLColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x10 == data[0]);
    REQUIRE(0x20 == data[1]);
    REQUIRE(0x30 == data[2]);
    REQUIRE(0xff == data[3]);
}


TEST_CASE("Dye replaceSColor 1")
{
    DyePalette palette("#00ff00,000011", 6);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x02;
    data[2] = 0x03;
    data[3] = 0x10;
    palette.replaceSColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x01 == data[0]);
    REQUIRE(0x02 == data[1]);
    REQUIRE(0x03 == data[2]);
    REQUIRE(0x10 == data[3]);
}

TEST_CASE("Dye replaceSColor 2")
{
    DyePalette palette("#403020,706050", 6);
    uint8_t data[4];
    data[0] = 0x10;
    data[1] = 0x20;
    data[2] = 0x30;
    data[3] = 0x40;
    palette.replaceSColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x10 == data[0]);
    REQUIRE(0x50 == data[1]);
    REQUIRE(0x60 == data[2]);
    REQUIRE(0x70 == data[3]);
}

TEST_CASE("Dye replaceSColor 3")
{
    DyePalette palette("#123456,000000,ff3020,706050", 6);
    uint8_t data[4];
    data[0] = 0x10;
    data[1] = 0x20;
    data[2] = 0x30;
    data[3] = 0xff;
    palette.replaceSColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x10 == data[0]);
    REQUIRE(0x50 == data[1]);
    REQUIRE(0x60 == data[2]);
    REQUIRE(0x70 == data[3]);
}


TEST_CASE("Dye replaceAColor 1")
{
    DyePalette palette("#00ff0010,00001120", 8);
    uint8_t data[4];
    data[0] = 0x01;
    data[1] = 0x02;
    data[2] = 0x03;
    data[3] = 0x10;
    palette.replaceAColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x01 == data[0]);
    REQUIRE(0x02 == data[1]);
    REQUIRE(0x03 == data[2]);
    REQUIRE(0x10 == data[3]);
}

TEST_CASE("Dye replaceAColor 2")
{
    DyePalette palette("#02ff0120,040311ff", 8);
    uint8_t data[4];
    data[0] = 0x20;
    data[1] = 0x01;
    data[2] = 0xff;
    data[3] = 0x02;
    palette.replaceAColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0xff == data[0]);
    REQUIRE(0x11 == data[1]);
    REQUIRE(0x03 == data[2]);
    REQUIRE(0x04 == data[3]);
}

TEST_CASE("Dye replaceAColor 3")
{
    DyePalette palette("#40404040,20000000,0100ee40,102030ff", 8);
    uint8_t data[4];
    data[0] = 0x40;
    data[1] = 0xee;
    data[2] = 0x00;
    data[3] = 0x01;
    palette.replaceAColor(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0xff == data[0]);
    REQUIRE(0x30 == data[1]);
    REQUIRE(0x20 == data[2]);
    REQUIRE(0x10 == data[3]);
}

TEST_CASE("Dye normalDye 1")
{
    Dye dye("R:#203040,506070");
    uint8_t data[4];
    data[0] = 0x55;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x50;
    dye.normalDye(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x55 == data[0]);
    REQUIRE(0x28 == data[1]);
    REQUIRE(0x1e == data[2]);
    REQUIRE(0x14 == data[3]);
}

TEST_CASE("Dye normalDye 2")
{
    Dye dye("G:#203040,506070");
    uint8_t data[4];
    data[0] = 0x60;
    data[1] = 0x00;
    data[2] = 0x50;
    data[3] = 0x00;
    dye.normalDye(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x60 == data[0]);
    REQUIRE(0x28 == data[1]);
    REQUIRE(0x1e == data[2]);
    REQUIRE(0x14 == data[3]);
}

TEST_CASE("Dye normalOGLDye 1")
{
    Dye dye("R:#203040,506070");
    uint8_t data[4];
    data[0] = 0x55;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x50;
    dye.normalOGLDye(reinterpret_cast<uint32_t*>(&data[0]), 1);
    REQUIRE(0x15 == data[0]);
    REQUIRE(0x20 == data[1]);
    REQUIRE(0x2a == data[2]);
    REQUIRE(0x50 == data[3]);
}
