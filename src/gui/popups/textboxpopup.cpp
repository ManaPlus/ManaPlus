/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/popups/textboxpopup.h"

#include "gui/widgets/textbox.h"

#include "gui/fonts/font.h"

#include "render/graphics.h"

#include "debug.h"

TextBoxPopup *textBoxPopup = nullptr;

TextBoxPopup::TextBoxPopup() :
    Popup("TextBoxPopup", "textboxpopup.xml"),
    mTextBox(new TextBox(this))
{
}

void TextBoxPopup::postInit()
{
    Popup::postInit();
    const int fontHeight = getFont()->getHeight();
    setMinHeight(fontHeight);
    mTextBox->setEditable(false);
    mTextBox->setOpaque(Opaque_false);
    add(mTextBox);
    addMouseListener(this);
}

TextBoxPopup::~TextBoxPopup()
{
}

void TextBoxPopup::show(const int x, const int y, const std::string &str)
{
    mTextBox->setTextWrapped(str, 190);
    setContentSize(mTextBox->getWidth(), mTextBox->getHeight());
    const int distance = 20;

    const Rect &rect = mDimension;
    int posX = std::max(0, x - rect.width / 2);
    int posY = y + distance;

    if (posX + rect.width > mainGraphics->mWidth)
        posX = mainGraphics->mWidth - rect.width;
    if (posY + rect.height > mainGraphics->mHeight)
        posY = y - rect.height - distance;

    setPosition(posX, posY);
    setVisible(Visible_true);
    requestMoveToTop();
}

void TextBoxPopup::mouseMoved(MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(Visible_false);
}
