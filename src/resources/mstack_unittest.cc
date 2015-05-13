/*
 *  The ManaPlus Client
 *  Copyright (C) 2015  The ManaPlus Developers
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

#include "gui/cliprect.h"

#include "resources/mstack.h"

#include "gtest/gtest.h"

#include "debug.h"

TEST(mstack, push1)
{
    MStack<ClipRect> stack(10);
    EXPECT_EQ(-1, stack.mPointer - stack.mStack);
    EXPECT_EQ(0, stack.mStack[0].xOffset);

    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    ClipRect &val2 = stack.top();
    EXPECT_EQ(0, stack.mPointer - stack.mStack);
    EXPECT_EQ(10, val2.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(0, stack.mStack[1].xOffset);

    val2.yOffset = 2;
    EXPECT_EQ(2, val1.yOffset);
}

TEST(mstack, push2)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    const ClipRect &val2 = stack.top();
    EXPECT_EQ(10, val2.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);

    val1.yOffset = 2;
    EXPECT_EQ(2, val2.yOffset);
    EXPECT_EQ(2, stack.mStack[0].yOffset);
}

TEST(mstack, push3)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    val1 = stack.top();
    EXPECT_EQ(10, val1.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
}

TEST(mstack, push4)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    EXPECT_EQ(10, val1.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(0, stack.mStack[1].xOffset);
    EXPECT_EQ(0, stack.mStack[2].xOffset);

    ClipRect &val2 = stack.push();
    val2.xOffset = 20;
    EXPECT_EQ(20, val2.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(20, stack.mStack[1].xOffset);
    EXPECT_EQ(0, stack.mStack[2].xOffset);

    ClipRect &val3 = stack.push();
    val3.xOffset = 30;
    EXPECT_EQ(30, val3.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(20, stack.mStack[1].xOffset);
    EXPECT_EQ(30, stack.mStack[2].xOffset);
}

TEST(mstack, pop1)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    EXPECT_EQ(10, stack.mStack[0].xOffset);

    stack.pop();
    EXPECT_EQ(-1, stack.mPointer - stack.mStack);
}

TEST(mstack, pop2)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    EXPECT_EQ(0, stack.mPointer - stack.mStack);

    val1.xOffset = 10;
    EXPECT_EQ(10, stack.mStack[0].xOffset);

    ClipRect &val2 = stack.push();
    EXPECT_EQ(1, stack.mPointer - stack.mStack);

    val2.xOffset = 20;
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(20, stack.mStack[1].xOffset);

    stack.pop();
    EXPECT_EQ(0, stack.mPointer - stack.mStack);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(20, stack.mStack[1].xOffset);

    ClipRect &val3 = stack.top();
    EXPECT_EQ(0, stack.mPointer - stack.mStack);
    EXPECT_EQ(10, val1.xOffset);
    EXPECT_EQ(20, val2.xOffset);
    EXPECT_EQ(10, val3.xOffset);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(20, stack.mStack[1].xOffset);
    EXPECT_EQ(0, stack.mStack[2].xOffset);
}

TEST(mstack, clear1)
{
    MStack<ClipRect> stack(10);
    EXPECT_EQ(-1, stack.mPointer - stack.mStack);
    EXPECT_EQ(0, stack.mStack[0].xOffset);

    ClipRect &val1 = stack.push();
    val1.xOffset = 10;

    stack.clear();
    EXPECT_EQ(-1, stack.mPointer - stack.mStack);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(0, stack.mStack[1].xOffset);
}

TEST(mstack, getpop1)
{
    MStack<ClipRect> stack(10);
    ClipRect &val1 = stack.push();
    val1.xOffset = 10;
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(10, val1.xOffset);

    ClipRect &val2 = stack.getPop();
    EXPECT_EQ(-1, stack.mPointer - stack.mStack);
    EXPECT_EQ(10, stack.mStack[0].xOffset);
    EXPECT_EQ(10, val2.xOffset);
}
