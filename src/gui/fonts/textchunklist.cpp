/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/fonts/textchunklist.h"

#include "gui/fonts/textchunk.h"

#include "debug.h"

TextChunkList::TextChunkList() :
    start(nullptr),
    end(nullptr),
    size(0),
    search(),
    searchWidth()
{
}

void TextChunkList::insertFirst(TextChunk *restrict const item)
{
    TextChunk *restrict const oldFirst = start;
    if (start != nullptr)
        start->prev = item;
    item->prev = nullptr;
    if (oldFirst != nullptr)
        item->next = oldFirst;
    else
        end = item;
    start = item;
    size ++;
    search[TextChunkSmall(item->text, item->color, item->color2)] = item;
    searchWidth[item->text] = item;
}

void TextChunkList::moveToFirst(TextChunk *restrict const item)
{
    if (item == start)
        return;

    TextChunk *restrict const oldPrev = item->prev;
    if (oldPrev != nullptr)
        oldPrev->next = item->next;
    TextChunk *restrict const oldNext = item->next;
    if (oldNext != nullptr)
        oldNext->prev = item->prev;
    else
        end = oldPrev;
    TextChunk *restrict const oldFirst = start;
    if (start != nullptr)
        start->prev = item;
    item->prev = nullptr;
    item->next = oldFirst;
    start = item;
}

void TextChunkList::remove(const TextChunk *restrict const item)
{
    if (item == nullptr)
        return;

    TextChunk *restrict const oldPrev = item->prev;
    TextChunk *restrict const oldNext = item->next;
    if (oldPrev != nullptr)
        oldPrev->next = item->next;
    else
        start = oldNext;
    if (oldNext != nullptr)
        oldNext->prev = item->prev;
    else
        end = oldPrev;

    search.erase(TextChunkSmall(item->text,
        item->color, item->color2));
    searchWidth.erase(item->text);
    size --;
}

void TextChunkList::removeBack()
{
    TextChunk *restrict oldEnd = end;
    if (oldEnd != nullptr)
    {
        end = oldEnd->prev;
        if (end != nullptr)
            end->next = nullptr;
        else
            start = nullptr;
        search.erase(TextChunkSmall(oldEnd->text,
            oldEnd->color, oldEnd->color2));
        searchWidth.erase(oldEnd->text);
        delete oldEnd;
        size --;
    }
}

void TextChunkList::removeBack(int n)
{
    TextChunk *restrict item = end;
    while ((n != 0) && (item != nullptr))
    {
        n --;
        TextChunk *oldEnd = item;
        item = item->prev;
        search.erase(TextChunkSmall(oldEnd->text,
            oldEnd->color, oldEnd->color2));
        searchWidth.erase(oldEnd->text);
        delete oldEnd;
        size --;
    }
    if (item != nullptr)
    {
        item->next = nullptr;
        end = item;
    }
    else
    {
        start = nullptr;
        end = nullptr;
    }
}

void TextChunkList::clear()
{
    search.clear();
    searchWidth.clear();
    TextChunk *restrict item = start;
    while (item != nullptr)
    {
        TextChunk *restrict const item2 = item->next;
        delete item;
        item = item2;
    }
    start = nullptr;
    end = nullptr;
    size = 0;
}
