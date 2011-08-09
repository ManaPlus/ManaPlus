/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/spellpopup.h"

#include "gui/gui.h"
#include "gui/palette.h"

#include "textcommand.h"

#include "graphics.h"
#include "units.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>
#include <guichan/widgets/label.hpp>

#include "debug.h"

SpellPopup::SpellPopup():
    Popup("SpellPopup")
{
    // Item Name
    mItemName = new gcn::Label;
    mItemName->setFont(boldFont);
    mItemName->setPosition(getPadding(), getPadding());

    add(mItemName);
    addMouseListener(this);
}

SpellPopup::~SpellPopup()
{
}

void SpellPopup::setItem(TextCommand *spell)
{
    if (spell)
        mItemName->setCaption(spell->getName());
    else
        mItemName->setCaption("?");

    mItemName->adjustSize();
    int minWidth = mItemName->getWidth();

    minWidth += 8;
    setWidth(minWidth);

    setContentSize(minWidth, getPadding() + getFont()->getHeight());
}

void SpellPopup::view(int x, int y)
{
    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > graphics->mWidth)
    {
        if (graphics->mWidth > getWidth())
            posX = graphics->mWidth - getWidth();
        else
            posX = 0;
    }
    if (posY + getHeight() > graphics->mHeight)
    {
        if (y > getHeight() + distance)
            posY = y - getHeight() - distance;
        else
            y = 0;
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
