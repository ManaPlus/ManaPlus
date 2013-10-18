/*
 *  Speech bubbles
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/popups/speechbubble.h"

#include "gui/sdlfont.h"

#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"

#include <guichan/font.hpp>

#include "debug.h"

SpeechBubble::SpeechBubble() :
    Popup("Speech", "speechbubble.xml"),
    mText(),
    mCaption(new Label(this)),
    mSpeechBox(new TextBox(this))
{
    setContentSize(140, 46);
    setMinWidth(29);
    setMinHeight(29);

    mCaption->setFont(boldFont);
    mSpeechBox->setEditable(false);
    mSpeechBox->setOpaque(false);
    mSpeechBox->setForegroundColorAll(getThemeColor(Theme::BUBBLE_TEXT),
        getThemeColor(Theme::BUBBLE_TEXT_OUTLINE));
}

void SpeechBubble::postInit()
{
    add(mCaption);
    add(mSpeechBox);
}

void SpeechBubble::setCaption(const std::string &name,
                              const gcn::Color *const color1,
                              const gcn::Color *const color2)
{
    mCaption->setCaption(name);
    mCaption->adjustSize();
    mCaption->setForegroundColorAll(*color1, *color2);
}

void SpeechBubble::setText(const std::string &text, const bool showName)
{
    if (text == mText && (mCaption->getWidth() <= mSpeechBox->getMinWidth()))
        return;

    mSpeechBox->setForegroundColorAll(getThemeColor(Theme::BUBBLE_TEXT),
        getThemeColor(Theme::BUBBLE_TEXT_OUTLINE));

    const int pad = mPadding;
    const int pad2 = 2 * pad;
    int width = mCaption->getWidth() + pad2;
    mSpeechBox->setTextWrapped(text, 130 > width ? 130 : width);
    const int speechWidth = mSpeechBox->getMinWidth() + pad2;

    const int fontHeight = getFont()->getHeight();
    const int nameHeight = showName ? mCaption->getHeight() + pad / 2 : 0;
    const int numRows = mSpeechBox->getNumberOfRows();
    const int height = (numRows * fontHeight) + nameHeight + pad;

    if (width < speechWidth)
        width = speechWidth;

    width += pad2;

    setContentSize(width, height);

    const gcn::Rectangle &rect = mDimension;
    const int xPos = ((rect.width - width) / 2);
    const int yPos = ((rect.height - height) / 2) + nameHeight;

    mCaption->setPosition(xPos, pad);
    mSpeechBox->setPosition(xPos, yPos);
}
