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

#include "gui/font.h"
#include "gui/theme.h"

#include "gtest/gtest.h"

#include "debug.h"

TEST(TextChunkList, empty)
{
    TextChunkList list;

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, add1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 3, 4));

    list.insertFirst(chunk);

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk, list.start);
    EXPECT_EQ(chunk, list.end);
    EXPECT_EQ(nullptr, chunk->prev);
    EXPECT_EQ(nullptr, chunk->next);

    EXPECT_EQ(1, list.search.size());
    EXPECT_EQ(chunk, (*list.search.find(SDLTextChunkSmall(
        chunk->text, chunk->color, chunk->color2))).second);

    EXPECT_EQ(1, list.searchWidth.size());
    EXPECT_EQ(chunk, (*list.searchWidth.find(chunk->text)).second);
}

TEST(TextChunkList, add2)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(3, 4, 5));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(2, 3, 4), Color(4, 5, 6));

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);

    EXPECT_EQ(2, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk2, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(chunk2, chunk1->next);
    EXPECT_EQ(chunk1, chunk2->prev);
    EXPECT_EQ(nullptr, chunk2->next);

    EXPECT_EQ(2, list.search.size());
    EXPECT_EQ(chunk1, (*list.search.find(SDLTextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);
    EXPECT_EQ(chunk2, (*list.search.find(SDLTextChunkSmall(
        chunk2->text, chunk2->color, chunk2->color2))).second);

    EXPECT_EQ(1, list.searchWidth.size());
    EXPECT_EQ(chunk1, (*list.searchWidth.find(chunk1->text)).second);
}

TEST(TextChunkList, addRemoveBack1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));

    list.insertFirst(chunk);
    list.removeBack();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, addRemoveBack2)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunk *chunk2 = new SDLTextChunk("test2",
        Color(1, 2, 4), Color(1, 2, 5));

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack();

    EXPECT_EQ(1, list.size);
    EXPECT_EQ(chunk1, list.start);
    EXPECT_EQ(chunk1, list.end);
    EXPECT_EQ(nullptr, chunk1->prev);
    EXPECT_EQ(nullptr, chunk1->next);

    EXPECT_EQ(1, list.search.size());
    EXPECT_EQ(chunk1, (*list.search.find(SDLTextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);

    EXPECT_EQ(1, list.searchWidth.size());
    EXPECT_EQ(chunk1, (*list.searchWidth.find(chunk1->text)).second);
}

TEST(TextChunkList, addRemoveBack3)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunk *chunk2 = new SDLTextChunk("test2",
        Color(2, 3, 4), Color(2, 3, 4));

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack(2);

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);

    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, addRemoveBack4)
{
    TextChunkList list;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunk *chunk2 = new SDLTextChunk("test2",
        Color(2, 3, 4), Color(2, 3, 4));
    SDLTextChunk *chunk3 = new SDLTextChunk("test",
        Color(3, 4, 5), Color(3, 4, 5));

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

    EXPECT_EQ(1, list.search.size());
    EXPECT_EQ(chunk1, (*list.search.find(SDLTextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);

    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, moveToFirst1)
{
    TextChunkList list;

    SDLTextChunk *chunk = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 3, 4));

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

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(2, 3, 4), Color(1, 2, 3));

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

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(1, 2, 4), Color(1, 2, 3));
    SDLTextChunk *chunk3 = new SDLTextChunk("test",
        Color(1, 2, 5), Color(1, 2, 3));

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

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(), Color());
    SDLTextChunk *chunk2 = new SDLTextChunk("test2",
        Color(), Color());
    SDLTextChunk *chunk3 = new SDLTextChunk("test3",
        Color(), Color());

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

    SDLTextChunk *chunk = new SDLTextChunk("test", Color(), Color());

    list.insertFirst(chunk);
    list.clear();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, clear2)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 0));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 1));
    SDLTextChunk *chunk3 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 2));

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.clear();

    EXPECT_EQ(0, list.size);
    EXPECT_EQ(nullptr, list.start);
    EXPECT_EQ(nullptr, list.end);
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, clear3)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 0));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 1));
    SDLTextChunk *chunk3 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 2));

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk1);
    EXPECT_EQ(chunksLeft + 3, sdlTextChunkCnt);
    EXPECT_EQ(3, list.search.size());
    EXPECT_EQ(1, list.searchWidth.size());

    list.removeBack();
    EXPECT_EQ(chunksLeft + 2, sdlTextChunkCnt);
    EXPECT_EQ(2, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());

    list.clear();
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, clear4)
{
    TextChunkList list;
    int chunksLeft = sdlTextChunkCnt;

    SDLTextChunk *chunk1 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 0));
    SDLTextChunk *chunk2 = new SDLTextChunk("test",
        Color(1, 2, 3), Color(2, 0, 1));
    SDLTextChunk *chunk3 = new SDLTextChunk("test3",
        Color(1, 2, 3), Color(2, 0, 2));

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk2);
    EXPECT_EQ(chunksLeft + 3, sdlTextChunkCnt);
    EXPECT_EQ(3, list.search.size());
    EXPECT_EQ(2, list.searchWidth.size());

    list.removeBack(2);
    EXPECT_EQ(chunksLeft + 1, sdlTextChunkCnt);
    EXPECT_EQ(1, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());

    list.clear();
    EXPECT_EQ(chunksLeft, sdlTextChunkCnt);
    EXPECT_EQ(0, list.search.size());
    EXPECT_EQ(0, list.searchWidth.size());
}

TEST(TextChunkList, sort1)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item3("test line2",
        Color(1, 2, 3), Color(1, 2, 3));
    EXPECT_EQ(false, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
    EXPECT_EQ(true, item1 < item3);
    EXPECT_EQ(false, item3 < item1);
}

TEST(TextChunkList, sort2)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(2, 3, 4), Color(1, 2, 3));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}

TEST(TextChunkList, sort3)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 3, 4), Color(1, 2, 3));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}

TEST(TextChunkList, sort4)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 2, 4), Color(1, 2, 3));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}

TEST(TextChunkList, sort5)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 2, 3), Color(2, 2, 3));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}

TEST(TextChunkList, sort6)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 2, 3), Color(1, 3, 3));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}

TEST(TextChunkList, sort7)
{
    SDLTextChunkSmall item1("test line1",
        Color(1, 2, 3), Color(1, 2, 3));
    SDLTextChunkSmall item2("test line1",
        Color(1, 2, 3), Color(1, 2, 4));
    EXPECT_EQ(true, item1 < item2);
    EXPECT_EQ(false, item2 < item1);
}
