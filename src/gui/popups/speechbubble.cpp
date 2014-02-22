/*
 *  Speech bubbles
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/popups/speechbubble.h"

#include "gui/font.h"
#include "gui/gui.h"
#include "gui/viewport.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/label.h"

#include "debug.h"

SpeechBubble::SpeechBubble() :
    Popup("Speech", "speechbubble.xml"),
    mText(),
    mSpacing(mSkin ? mSkin->getOption("spacing") : 2),
    mCaption(new Label(this)),
    mSpeechBox(new BrowserBox(this, BrowserBox::AUTO_SIZE, true,
        "speechbrowserbox.xml"))
{
    setContentSize(140, 46);
    setMinWidth(8);
    setMinHeight(8);

    mCaption->setFont(boldFont);
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
                              const Color *const color1,
                              const Color *const color2)
{
    mCaption->setCaption(name);
    mCaption->adjustSize();
    mCaption->setForegroundColorAll(*color1, *color2);
}

void SpeechBubble::setText(const std::string &text, const bool showName)
{
    if (text == mText && (mCaption->getWidth() <= mSpeechBox->getWidth()))
        return;

    mSpeechBox->setForegroundColorAll(getThemeColor(Theme::BUBBLE_TEXT),
        getThemeColor(Theme::BUBBLE_TEXT_OUTLINE));

    int width = mCaption->getWidth();
    mSpeechBox->clearRows();
    mSpeechBox->addRow(text);
    mSpeechBox->setWidth(mSpeechBox->getDataWidth());

    const int speechWidth = mSpeechBox->getWidth();
    const int nameHeight = showName ? mCaption->getHeight() + mSpacing : 0;

    if (width < speechWidth)
        width = speechWidth;

    setContentSize(width, getFont()->getHeight() + nameHeight);
    mCaption->setPosition(0, 0);
    mSpeechBox->setPosition(0, nameHeight);
}

void SpeechBubble::requestMoveToBackground()
{
    windowContainer->moveWidgetAfter(viewport, this);
}
