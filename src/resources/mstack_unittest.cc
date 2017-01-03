/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2017  The ManaPlus Developers
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

#include "gui/cliprect.h"

#include "resources/mstack.h"

#include "debug.h"

TEST_CASE("mstack push 1")
{
    MStack<ClipRect> stack(10);
    REQUIRE(-1 == (stack.mPointer - stack.mStack));
    REQUIRE(0 == stack.mStack[0].xOffset);

    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    ClipRect &val2 = stack.top();
    REQUIRE(0 == stack.mPointer - stack.mStack);
    REQUIRE(10 == val2.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(0 == stack.mStack[1].xOffset);

    val2.yOffset = 2;
    REQUIRE(2 == val1.yOffset);
}

TEST_CASE("mstack push 2")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    const ClipRect &val2 = stack.top();
    REQUIRE(10 == val2.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);

    val1.yOffset = 2;
    REQUIRE(2 == val2.yOffset);
    REQUIRE(2 == stack.mStack[0].yOffset);
}

TEST_CASE("mstack push 3")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    ClipRect &val2 = stack.top();
    REQUIRE(10 == val2.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
}

TEST_CASE("mstack push 4")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    REQUIRE(10 == val1.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(0 == stack.mStack[1].xOffset);
    REQUIRE(0 == stack.mStack[2].xOffset);

    ClipRect &val2 = stack.push();
    val2.xOffset = 20;
    REQUIRE(20 == val2.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(20 == stack.mStack[1].xOffset);
    REQUIRE(0 == stack.mStack[2].xOffset);

    ClipRect &val3 = stack.push();
    val3.xOffset = 30;
    REQUIRE(30 == val3.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(20 == stack.mStack[1].xOffset);
    REQUIRE(30 == stack.mStack[2].xOffset);
}

TEST_CASE("mstack pop 1")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    REQUIRE(10 == stack.mStack[0].xOffset);

    stack.pop();
    REQUIRE(-1 == stack.mPointer - stack.mStack);
}

TEST_CASE("mstack pop 2")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    REQUIRE(0 == stack.mPointer - stack.mStack);

    val1.xOffset = 10;
    REQUIRE(10 == stack.mStack[0].xOffset);

    ClipRect &val2 = stack.push();
    REQUIRE(1 == stack.mPointer - stack.mStack);

    val2.xOffset = 20;
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(20 == stack.mStack[1].xOffset);

    stack.pop();
    REQUIRE(0 == stack.mPointer - stack.mStack);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(20 == stack.mStack[1].xOffset);

    ClipRect &val3 = stack.top();
    REQUIRE(0 == stack.mPointer - stack.mStack);
    REQUIRE(10 == val1.xOffset);
    REQUIRE(20 == val2.xOffset);
    REQUIRE(10 == val3.xOffset);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(20 == stack.mStack[1].xOffset);
    REQUIRE(0 == stack.mStack[2].xOffset);
}

TEST_CASE("mstack clear 1")
{
    MStack<ClipRect> stack(10);
    REQUIRE(-1 == stack.mPointer - stack.mStack);
    REQUIRE(0 == stack.mStack[0].xOffset);

    ClipRect &val1 = stack.push();
    val1.xOffset = 10;

    stack.clear();
    REQUIRE(-1 == stack.mPointer - stack.mStack);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(0 == stack.mStack[1].xOffset);
}

TEST_CASE("mstack getpop 1")
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(10 == val1.xOffset);

    ClipRect &val2 = stack.getPop();
    REQUIRE(-1 == stack.mPointer - stack.mStack);
    REQUIRE(10 == stack.mStack[0].xOffset);
    REQUIRE(10 == val2.xOffset);
}
