/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "textmanager.h"

#include "text.h"

#include <cstring>

#include "debug.h"

TextManager *textManager = nullptr;

TextManager::TextManager() :
    mTextList()
{
}

void TextManager::addText(Text *const text)
{
    place(text, nullptr, text->mX, text->mY, text->mHeight);
    mTextList.push_back(text);
}

void TextManager::moveText(Text *const text, const int x, const int y) const
{
    text->mX = x;
    text->mY = y;
    place(text, text, text->mX, text->mY, text->mHeight);
}

void TextManager::removeText(const Text *const text)
{
    FOR_EACH (TextList::iterator, ptr, mTextList)
    {
        if (*ptr == text)
        {
            mTextList.erase(ptr);
            return;
        }
    }
}

TextManager::~TextManager()
{
}

void TextManager::draw(Graphics *const graphics,
                       const int xOff, const int yOff)
{
    BLOCK_START("TextManager::draw")
    FOR_EACH (TextList::const_iterator, bPtr, mTextList)
        (*bPtr)->draw(graphics, xOff, yOff);
    BLOCK_END("TextManager::draw")
}

void TextManager::place(const Text *const textObj, const Text *const omit,
                        const int &x A_UNUSED, int &y, const int h) const
{
    const int xLeft = textObj->mX;
    const int xRight1 = xLeft + textObj->mWidth;
    const int TEST = 50;  // Number of lines to test for text
    const int nBeings = 30;
    bool occupied[TEST];  // is some other text obscuring this line?
    std::memset(&occupied, 0, sizeof(occupied));  // set all to false
    const int wantedTop = (TEST - h) / 2;   // Entry in occupied at top of text
    const int occupiedTop = y - wantedTop;  // Line in map representing
                                            // to of occupied
    int cnt = 0;

    for (TextList::const_iterator ptr = mTextList.begin(),
         pEnd = mTextList.end(); ptr != pEnd && cnt < nBeings; ++ptr, cnt ++)
    {
        const Text *const text = *ptr;

        if (text != omit && text->mX + 1 <= xRight1
            && text->mX + text->mWidth > xLeft)
        {
            int from = text->mY - occupiedTop;
            int to = from + text->mHeight - 1;
            if (to < 0 || from >= TEST)  // out of range considered
                continue;
            if (from < 0)
                from = 0;
            if (to >= TEST)
                to = TEST - 1;
            for (int i = from; i <= to; ++i)
                occupied[i] = true;
        }
    }
    bool ok = true;
    const int sz = wantedTop + h;
    for (int i = wantedTop; i < sz; i ++)
    {
        if (occupied[i])
        {
            ok = false;
            break;
        }
    }

    if (ok)
        return;

    // Have to move it up or down, so find nearest spaces either side
    int consec = 0;
    int upSlot = -1;  // means not found
    const int sz2 = wantedTop + h - 2;
    for (int seek = sz2; seek >= 0; --seek)
    {
        if (occupied[seek])
        {
            consec = 0;
        }
        else
        {
            if (++consec == h)
            {
                upSlot = seek;
                break;
            }
        }
    }
    int downSlot = -1;
    consec = 0;
    for (int seek = wantedTop + 1; seek < TEST; ++seek)
    {
        if (occupied[seek])
        {
            consec = 0;
        }
        else
        {
            if (++consec == h)
            {
                downSlot = seek - h + 1;
                break;
            }
        }
    }
    if (upSlot == -1 && downSlot == -1)  // no good solution, so leave as is
    {
        return;
    }
    if (upSlot == -1)  // must go down
    {
        y += downSlot - wantedTop;
        return;
    }
    if (downSlot == -1)  // must go up
    {
        y -= wantedTop - upSlot;
        return;
    }
    if (wantedTop - upSlot > downSlot - wantedTop)  // down is better
    {
        y += downSlot - wantedTop;
    }
    else
    {
        y -= wantedTop - upSlot;
    }
}
