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

#include "logger.h"

#include "gui/sdlfont.h"
#include "gui/theme.h"

#include "gtest/gtest.h"

#include "debug.h"

TEST(TextChunkList, empty)
{
    TextChunkList list;

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
}

TEST(TextChunkList, add1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test", gcn::Color(), gcn::Color());

    list.insertFirst(chunk);

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk, list.start);
    EXPECT_EQ(chunk, list.end);
    EXPECT_EQ(nullptr, chunk->prev);
    EXPECT_EQ(nullptr, chunk->next);
}

TEST(TextChunkList, add2)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test", gcn::Color(), gcn::Color());

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);

    EXPECT_EQ(2, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk2, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(chunk2, chunk1->next);
    EXPECT_EQ(chunk1, chunk2->prev);
    EXPECT_EQ(nullptr, chunk2->next);
}

TEST(TextChunkList, addRemoveBack1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test", gcn::Color(), gcn::Color());

    list.insertFirst(chunk);
    list.removeBack();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
}

TEST(TextChunkList, addRemoveBack2)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack();

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk1, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(nullptr, chunk1->next);
}

TEST(TextChunkList, addRemoveBack3)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack(2);

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
}

TEST(TextChunkList, addRemoveBack4)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk3);
    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack();
    list.removeBack();

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk1, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(nullptr, chunk1->next);
}

TEST(TextChunkList, moveToFirst1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test", gcn::Color(), gcn::Color());

    list.insertFirst(chunk);
    list.moveToFirst(chunk);

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk, list.start);
    EXPECT_EQ(chunk, list.end);
    EXPECT_EQ(nullptr, chunk->prev);
    EXPECT_EQ(nullptr, chunk->next);
}

TEST(TextChunkList, moveToFirst2)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    EXPECT_EQ(2, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk2, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(chunk2, chunk1->next);
    EXPECT_EQ(chunk1, chunk2->prev);
    EXPECT_EQ(nullptr, chunk2->next);
}

TEST(TextChunkList, moveToFirst3)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk3);
    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    EXPECT_EQ(3, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk3, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(chunk2, chunk1->next);
    EXPECT_EQ(chunk1, chunk2->prev);
    EXPECT_EQ(chunk3, chunk2->next);
    EXPECT_EQ(chunk2, chunk3->prev);
    EXPECT_EQ(nullptr, chunk3->next);
}

TEST(TextChunkList, moveToFirst4)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk1);
    list.insertFirst(chunk3);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    EXPECT_EQ(3, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk3, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(chunk2, chunk1->next);
    EXPECT_EQ(chunk1, chunk2->prev);
    EXPECT_EQ(chunk3, chunk2->next);
    EXPECT_EQ(chunk2, chunk3->prev);
    EXPECT_EQ(nullptr, chunk3->next);
}

TEST(TextChunkList, clear1)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk = new SDLTextChunk("test", gcn::Color(), gcn::Color());

    list.insertFirst(chunk);
    list.clear();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
}

TEST(TextChunkList, clear2)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test1", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.clear();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
}

TEST(TextChunkList, clear3)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test1", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk1);
    EXPECT_EQ(chunksLeft + 3, sdlTextChunkCnt);

    list.removeBack();
    EXPECT_EQ(chunksLeft + 2, sdlTextChunkCnt);

    list.clear();
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
}

TEST(TextChunkList, clear4)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test1", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2", gcn::Color(), gcn::Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3", gcn::Color(), gcn::Color());

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk2);
    EXPECT_EQ(chunksLeft + 3, sdlTextChunkCnt);

    list.removeBack(2);
    EXPECT_EQ(chunksLeft + 1, sdlTextChunkCnt);

    list.clear();
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
}
