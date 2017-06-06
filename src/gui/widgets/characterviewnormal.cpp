/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "gui/widgets/characterviewnormal.h"

#include "configuration.h"

#include "gui/widgets/characterdisplay.h"

#include "debug.h"

CharacterViewNormal::CharacterViewNormal(CharSelectDialog *const widget,
                                         std::vector<CharacterDisplay*>
                                         *const entries,
                                         const int padding) :
    CharacterViewBase(widget, padding),
    mCharacterEntries(entries)
{
    addKeyListener(widget);
    if (entries != nullptr)
    {
        FOR_EACHP (std::vector<CharacterDisplay*>::iterator,
                    it, entries)
        {
            CharacterDisplay *const character = *it;
            add(character);
            character->setVisible(Visible_true);
        }
        const size_t sz = mCharacterEntries->size();
        if (mSelected >= 0 && mSelected < CAST_S32(sz))
        {
            CharacterDisplay *const display = (*mCharacterEntries)[mSelected];
            if (display != nullptr)
                display->setSelect(false);
        }
        if (sz > 0)
        {
            mSelected = 0;
            CharacterDisplay *const display = (*mCharacterEntries)[0];
            display->setSelect(true);
            setWidth(display->getWidth() * 5 + mPadding * 2);
        }
        else
        {
            mSelected = -1;
        }
    }
    setHeight(210 + config.getIntValue("fontSize") * 2);
}

CharacterViewNormal::~CharacterViewNormal()
{
    removeKeyListener(mParent);
}

void CharacterViewNormal::show(const int i)
{
    const int sz = CAST_S32(mCharacterEntries->size());
    if (i >= 0 && i < sz)
    {
        if (mSelected >= 0)
            (*mCharacterEntries)[mSelected]->setSelect(false);
        mSelected = i;
        (*mCharacterEntries)[i]->setSelect(true);
    }
}

void CharacterViewNormal::resize()
{
    const int sz = CAST_S32(mCharacterEntries->size());
    if (sz <= 0)
        return;
    const CharacterDisplay *const firtChar = (*mCharacterEntries)[0];
    int y = 0;
    const int width = firtChar->getWidth();
    const int height = firtChar->getHeight();
    for (int f = 0; f < 5; f ++)
        (*mCharacterEntries)[f]->setPosition(f * width, y);
    y += height;

    for (int f = 5; f < sz; f ++)
        (*mCharacterEntries)[f]->setPosition((f - 5) * width, y);
}

void CharacterViewNormal::action(const ActionEvent &event A_UNUSED)
{
}
