/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/widgets/characterdisplay.h"

#include "gui/charselectdialog.h"

#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "debug.h"

CharacterDisplay::CharacterDisplay(const Widget2 *const widget,
                                   CharSelectDialog *const charSelectDialog) :
    Container(widget),
    mCharacter(nullptr),
    mPlayerBox(new PlayerBox(nullptr)),
    mName(new Label(this, "wwwwwwwwwwwwwwwwwwwwwwww"))
{
    mPlayerBox->setActionEventId("select");
    mPlayerBox->addActionListener(charSelectDialog);

    LayoutHelper h(this);
    ContainerPlacer placer = h.getPlacer(0, 0);

    placer(0, 0, mPlayerBox, 3, 5);
    placer(0, 5, mName, 3);

    update();

    mName->setAlignment(Graphics::CENTER);
    mName->adjustSize();

    setWidth(80);
    setHeight(120);
}

void CharacterDisplay::setCharacter(Net::Character *const character)
{
    if (mCharacter == character)
        return;

    mCharacter = character;
    mPlayerBox->setPlayer(character ? character->dummy : nullptr);
    update();
}

void CharacterDisplay::requestFocus()
{
}

void CharacterDisplay::setActive(const bool active A_UNUSED)
{
}

void CharacterDisplay::update()
{
    if (mCharacter)
        mName->setCaption(mCharacter->dummy->getName());
    else
        mName->setCaption("");

    distributeResizedEvent();
}
