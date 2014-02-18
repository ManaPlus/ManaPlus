/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "gui/widgets/characterviewsmall.h"

#include "gui/widgets/button.h"
#include "gui/widgets/characterdisplay.h"
#include "gui/widgets/label.h"

#include "utils/stringutils.h"

#include "debug.h"

CharacterViewSmall::CharacterViewSmall(CharSelectDialog *const widget,
                                       std::vector<CharacterDisplay*>
                                       *const entries,
                                       const int padding) :
    CharacterViewBase(widget, padding),
    mSelectedEntry(nullptr),
    mPrevious(new Button(this, "<", "prev", this)),
    mNext(new Button(this, ">", "next", this)),
    mNumber(new Label(this, "??")),
    mCharacterEntries(entries)
{
    addKeyListener(widget);
    if (entries)
    {
        FOR_EACHP (std::vector<CharacterDisplay*>::iterator,
                   it, entries)
        {
            add(*it);
        }
        const int sz = static_cast<signed>(mCharacterEntries->size());
        mSelected = 0;
        mSelectedEntry = (*mCharacterEntries)[mSelected];
        mSelectedEntry->setVisible(true);
        mNumber->setCaption(strprintf("%d / %d", mSelected + 1, sz));
        mNumber->adjustSize();
    }
    add(mPrevious);
    add(mNext);
    add(mNumber);

    setHeight(200);
}

CharacterViewSmall::~CharacterViewSmall()
{
    removeKeyListener(mParent);
}

void CharacterViewSmall::show(const int i)
{
    const int sz = static_cast<signed>(mCharacterEntries->size());
    if (mSelectedEntry)
        mSelectedEntry->setVisible(false);
    if (i >= sz)
        mSelected = 0;
    else if (i < 0)
        mSelected = sz - 1;
    else
        mSelected = i;
    mSelectedEntry = (*mCharacterEntries)[mSelected];
    mSelectedEntry->setVisible(true);
    mNumber->setCaption(strprintf("%d / %d", mSelected + 1, sz));
    mNumber->adjustSize();
}

void CharacterViewSmall::resize()
{
    const CharacterDisplay *const firtChar = (*mCharacterEntries)[0];
    const int x = (getWidth() - firtChar->getWidth()) / 2;
    const int y = (getHeight() - firtChar->getHeight()) / 2;
    FOR_EACHP (std::vector<CharacterDisplay*>::iterator,
                it, mCharacterEntries)
    {
        (*it)->setPosition(x, y);
    }
    const int y2 = (getHeight() - mPrevious->getHeight()) / 2;
    const int y3 = y2 - 55;
    mPrevious->setPosition(x - mPrevious->getWidth() - 10, y3);
    mNext->setPosition(getWidth() - x + 10, y3);
    mNumber->setPosition(10, y2);
}

void CharacterViewSmall::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "next")
    {
        mSelected ++;
        show(mSelected);
        mParent->updateState();
    }
    else if (eventId == "prev")
    {
        mSelected --;
        show(mSelected);
        mParent->updateState();
    }
}
