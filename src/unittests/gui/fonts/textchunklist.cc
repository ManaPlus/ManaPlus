/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#include "gui/fonts/font.h"
#include "gui/fonts/textchunk.h"

#include "debug.h"

TEST_CASE("TextChunkList empty", "TextChunkList")
{
    TextChunkList list;

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList add 1", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 3, 4, 255U), nullptr);

    list.insertFirst(chunk);

    REQUIRE(1 == list.size);
    REQUIRE(chunk == list.start);
    REQUIRE(chunk == list.end);
    REQUIRE(nullptr == chunk->prev);
    REQUIRE(nullptr == chunk->next);

    REQUIRE(1 == list.search.size());
    REQUIRE(chunk == (*list.search.find(TextChunkSmall(
        chunk->text, chunk->color, chunk->color2))).second);

    REQUIRE(1 == list.searchWidth.size());
    REQUIRE(chunk == (*list.searchWidth.find(chunk->text)).second);
    delete chunk;
}

TEST_CASE("TextChunkList add 2", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(3, 4, 5, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(2, 3, 4, 255U), Color(4, 5, 6, 255U), nullptr);

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);

    REQUIRE(2 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk2 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(chunk2 == chunk1->next);
    REQUIRE(chunk1 == chunk2->prev);
    REQUIRE(nullptr == chunk2->next);

    REQUIRE(2 == list.search.size());
    REQUIRE(chunk1 == (*list.search.find(TextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);
    REQUIRE(chunk2 == (*list.search.find(TextChunkSmall(
        chunk2->text, chunk2->color, chunk2->color2))).second);

    REQUIRE(1 == list.searchWidth.size());
    REQUIRE(chunk1 == (*list.searchWidth.find(chunk1->text)).second);
    delete chunk1;
    delete chunk2;
}

TEST_CASE("TextChunkList addRemoveBack 1", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);

    list.insertFirst(chunk);
    list.removeBack();

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList addRemoveBack 2", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(1, 2, 4, 255U), Color(1, 2, 5, 255U), nullptr);

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack();

    REQUIRE(1 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk1 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(nullptr == chunk1->next);

    REQUIRE(1 == list.search.size());
    REQUIRE(chunk1 == (*list.search.find(TextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);

    REQUIRE(1 == list.searchWidth.size());
    REQUIRE(chunk1 == (*list.searchWidth.find(chunk1->text)).second);
    delete chunk1;
}

TEST_CASE("TextChunkList addRemoveBack 3", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(2, 3, 4, 255U), Color(2, 3, 4, 255U), nullptr);

    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack(2);

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);

    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList addRemoveBack 4", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(2, 3, 4, 255U), Color(2, 3, 4, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test",
        Color(3, 4, 5, 255U), Color(3, 4, 5, 255U), nullptr);

    list.insertFirst(chunk3);
    list.insertFirst(chunk2);
    list.insertFirst(chunk1);
    list.removeBack();
    list.removeBack();

    REQUIRE(1 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk1 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(nullptr == chunk1->next);

    REQUIRE(1 == list.search.size());
    REQUIRE(chunk1 == (*list.search.find(TextChunkSmall(
        chunk1->text, chunk1->color, chunk1->color2))).second);

    REQUIRE(list.searchWidth.empty());
    delete chunk1;
}

TEST_CASE("TextChunkList moveToFirst 1", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 3, 4, 255U), nullptr);

    list.insertFirst(chunk);
    list.moveToFirst(chunk);

    REQUIRE(1 == list.size);
    REQUIRE(chunk == list.start);
    REQUIRE(chunk == list.end);
    REQUIRE(nullptr == chunk->prev);
    REQUIRE(nullptr == chunk->next);
    delete chunk;
}

TEST_CASE("TextChunkList moveToFirst 2", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(2, 3, 4, 255U), Color(1, 2, 3, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    REQUIRE(2 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk2 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(chunk2 == chunk1->next);
    REQUIRE(chunk1 == chunk2->prev);
    REQUIRE(nullptr == chunk2->next);
    delete chunk1;
    delete chunk2;
}

TEST_CASE("TextChunkList moveToFirst 3", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(1, 2, 4, 255U), Color(1, 2, 3, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test",
        Color(1, 2, 5, 255U), Color(1, 2, 3, 255U), nullptr);

    list.insertFirst(chunk3);
    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    REQUIRE(3 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk3 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(chunk2 == chunk1->next);
    REQUIRE(chunk1 == chunk2->prev);
    REQUIRE(chunk3 == chunk2->next);
    REQUIRE(chunk2 == chunk3->prev);
    REQUIRE(nullptr == chunk3->next);
    delete chunk1;
    delete chunk2;
    delete chunk3;
}

TEST_CASE("TextChunkList moveToFirst 4", "TextChunkList")
{
    TextChunkList list;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(), Color(), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(), Color(), nullptr);
    TextChunk *const chunk3 = new TextChunk("test3",
        Color(), Color(), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk3);
    list.insertFirst(chunk2);
    list.moveToFirst(chunk1);

    REQUIRE(3 == list.size);
    REQUIRE(chunk1 == list.start);
    REQUIRE(chunk3 == list.end);
    REQUIRE(nullptr == chunk1->prev);
    REQUIRE(chunk2 == chunk1->next);
    REQUIRE(chunk1 == chunk2->prev);
    REQUIRE(chunk3 == chunk2->next);
    REQUIRE(chunk2 == chunk3->prev);
    REQUIRE(nullptr == chunk3->next);
    delete chunk1;
    delete chunk2;
    delete chunk3;
}

TEST_CASE("TextChunkList clear 1", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk = new TextChunk("test",
        Color(), Color(), nullptr);

    list.insertFirst(chunk);
    list.clear();

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);
    REQUIRE(chunksLeft == textChunkCnt);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList clear 2", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 0, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 1, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 2, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.clear();

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);
    REQUIRE(chunksLeft == textChunkCnt);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList clear 3", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 0, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 1, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 2, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk1);
    REQUIRE((chunksLeft + 3) == textChunkCnt);
    REQUIRE(3 == list.search.size());
    REQUIRE(1 == list.searchWidth.size());

    list.removeBack();
    REQUIRE((chunksLeft + 2) == textChunkCnt);
    REQUIRE(2 == list.search.size());
    REQUIRE(list.searchWidth.empty());

    list.clear();
    REQUIRE(chunksLeft == textChunkCnt);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList clear 4", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk1 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 0, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test",
        Color(1, 2, 3, 255U), Color(2, 0, 1, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test3",
        Color(1, 2, 3, 255U), Color(2, 0, 2, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.moveToFirst(chunk2);
    REQUIRE((chunksLeft + 3) == textChunkCnt);
    REQUIRE(3 == list.search.size());
    REQUIRE(2 == list.searchWidth.size());

    list.removeBack(2);
    REQUIRE((chunksLeft + 1) == textChunkCnt);
    REQUIRE(1 == list.search.size());
    REQUIRE(list.searchWidth.empty());

    list.clear();
    REQUIRE(chunksLeft == textChunkCnt);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList remove 1", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk = new TextChunk("test",
        Color(), Color(), nullptr);

    list.insertFirst(chunk);
    list.remove(chunk);
    delete chunk;

    REQUIRE(0 == list.size);
    REQUIRE(nullptr == list.start);
    REQUIRE(nullptr == list.end);
    REQUIRE(chunksLeft == textChunkCnt);
    REQUIRE(list.search.empty());
    REQUIRE(list.searchWidth.empty());
}

TEST_CASE("TextChunkList remove 2", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk1 = new TextChunk("test1",
        Color(1, 2, 3, 255U), Color(2, 0, 0, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(1, 2, 3, 255U), Color(2, 0, 1, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test3",
        Color(1, 2, 3, 255U), Color(2, 0, 2, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.remove(chunk1);
    delete chunk1;

    REQUIRE(2 == list.size);
    REQUIRE(chunk3 == list.start);
    REQUIRE(chunk2 == list.end);
    REQUIRE((chunksLeft + 2) == textChunkCnt);
    REQUIRE(2 == list.search.size());
    REQUIRE(2 == list.searchWidth.size());
    delete chunk2;
    delete chunk3;
}

TEST_CASE("TextChunkList remove 3", "TextChunkList")
{
    TextChunkList list;
    const int chunksLeft = textChunkCnt;

    TextChunk *const chunk1 = new TextChunk("test1",
        Color(1, 2, 3, 255U), Color(2, 0, 0, 255U), nullptr);
    TextChunk *const chunk2 = new TextChunk("test2",
        Color(1, 2, 3, 255U), Color(2, 0, 1, 255U), nullptr);
    TextChunk *const chunk3 = new TextChunk("test3",
        Color(1, 2, 3, 255U), Color(2, 0, 2, 255U), nullptr);

    list.insertFirst(chunk1);
    list.insertFirst(chunk2);
    list.insertFirst(chunk3);
    list.remove(chunk2);
    delete chunk2;

    REQUIRE(2 == list.size);
    REQUIRE(chunk3 == list.start);
    REQUIRE(chunk1 == list.end);
    REQUIRE((chunksLeft + 2) == textChunkCnt);
    REQUIRE(2 == list.search.size());
    REQUIRE(2 == list.searchWidth.size());
    delete chunk1;
    delete chunk3;
}

TEST_CASE("TextChunkList sort 1", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item3("test line2",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    REQUIRE(false == (item1 < item2));
    REQUIRE(false == (item2 < item1));
    REQUIRE(item1 < item3);
    REQUIRE(false == (item3 < item1));
}

TEST_CASE("TextChunkList sort 2", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(2, 3, 4, 255U), Color(1, 2, 3, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}

TEST_CASE("TextChunkList sort 3", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 3, 4, 255U), Color(1, 2, 3, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}

TEST_CASE("TextChunkList sort 4", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 2, 4, 255U), Color(1, 2, 3, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}

TEST_CASE("TextChunkList sort 5", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 2, 3, 255U), Color(2, 2, 3, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}

TEST_CASE("TextChunkList sort 6", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 2, 3, 255U), Color(1, 3, 3, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}

TEST_CASE("TextChunkList sort 7", "TextChunkList")
{
    TextChunkSmall item1("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 3, 255U));
    TextChunkSmall item2("test line1",
        Color(1, 2, 3, 255U), Color(1, 2, 4, 255U));
    REQUIRE(item1 < item2);
    REQUIRE(false == (item2 < item1));
}
