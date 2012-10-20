/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#include "gui/spellpopup.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/label.h"

#include "units.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

SpellPopup::SpellPopup():
    Popup("SpellPopup", "spellpopup.xml"),
    mItemName(new Label(this)),
    mItemComment(new Label(this))
{
    mItemName->setFont(boldFont);
    mItemName->setForegroundColor(getThemeColor(Theme::POPUP));
    mItemComment->setForegroundColor(getThemeColor(Theme::POPUP));

    add(mItemName);
    add(mItemComment);

    addMouseListener(this);
}

SpellPopup::~SpellPopup()
{
}

void SpellPopup::setItem(const TextCommand *const spell)
{
    if (spell)
    {
        mItemName->setCaption(spell->getName());
        mItemComment->setCaption(spell->getComment());
    }
    else
    {
        mItemName->setCaption("?");
        mItemComment->setCaption("");
    }

    mItemName->adjustSize();
    mItemComment->adjustSize();
    int minWidth = mItemName->getWidth();
    if (mItemComment->getWidth() > minWidth)
        minWidth = mItemComment->getWidth();

    mItemName->setPosition(0, 0);
    mItemComment->setPosition(0, mItemName->getHeight());

    if (mItemComment->getCaption() != "")
        setContentSize(minWidth, 2 * getFont()->getHeight());
    else
        setContentSize(minWidth, getFont()->getHeight());
}

void SpellPopup::view(const int x, const int y)
{
    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > mainGraphics->mWidth)
    {
        if (mainGraphics->mWidth > getWidth())
            posX = mainGraphics->mWidth - getWidth();
        else
            posX = 0;
    }
    if (posY + getHeight() > mainGraphics->mHeight)
    {
        if (y > getHeight() + distance)
            posY = y - getHeight() - distance;
    }

    setPosition(posX, posY);
    setVisible(true);
    requestMoveToTop();
}

void SpellPopup::mouseMoved(gcn::MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(false);
}
