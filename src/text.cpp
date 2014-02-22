/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
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

#include "text.h"

#include "configuration.h"
#include "textmanager.h"

#include "gui/font.h"
#include "gui/gui.h"
#include "gui/theme.h"

#include "resources/image.h"

#include "debug.h"

int Text::mInstances = 0;
ImageRect Text::mBubble;

Text::Text(const std::string &text, const int x, const int y,
           const Graphics::Alignment alignment,
           const Color *const color, const bool isSpeech,
           Font *const font) :
    mFont(font ? font : (gui ? gui->getFont() : nullptr)),
    mX(x),
    mY(y),
    mWidth(mFont ? mFont->getWidth(text) : 1),
    mHeight(mFont ? mFont->getHeight() : 1),
    mXOffset(0),
    mText(text),
    mColor(color),
    mOutlineColor(Theme::getThemeColor(Theme::OUTLINE)),
    mIsSpeech(isSpeech)
{
    if (!textManager)
    {
        textManager = new TextManager;
        if (Theme::instance())
        {
            Theme::instance()->loadRect(mBubble, "bubble.xml", "");
        }
        else
        {
            for (int f = 0; f < 9; f ++)
                mBubble.grid[f] = nullptr;
        }

        const float bubbleAlpha = config.getFloatValue("speechBubbleAlpha");
        for (int i = 0; i < 9; i++)
        {
            if (mBubble.grid[i])
                mBubble.grid[i]->setAlpha(bubbleAlpha);
        }
    }
    ++mInstances;

    switch (alignment)
    {
        case Graphics::LEFT:
            mXOffset = 0;
            break;
        case Graphics::CENTER:
            mXOffset = mWidth / 2;
            break;
        case Graphics::RIGHT:
            mXOffset = mWidth;
            break;
        default:
            break;
    }
    mX = x - mXOffset;
    if (textManager)
        textManager->addText(this);
}

Text::~Text()
{
    if (textManager)
        textManager->removeText(this);
    if (--mInstances == 0)
    {
        delete textManager;
        textManager = nullptr;
        for (int f = 0; f < 9; f ++)
        {
            if (mBubble.grid[f])
            {
                mBubble.grid[f]->decRef();
                mBubble.grid[f] = nullptr;
            }
        }
    }
}

void Text::setColor(const Color *const color)
{
    mColor = color;
}

void Text::adviseXY(const int x, const int y, const bool move)
{
    if (textManager && move)
    {
        textManager->moveText(this, x - mXOffset, y);
    }
    else
    {
        mX = x - mXOffset;
        mY = y;
    }
}

void Text::draw(Graphics *const graphics, const int xOff, const int yOff)
{
    BLOCK_START("Text::draw")
    if (mIsSpeech)
    {
        graphics->drawImageRect(mX - xOff - 5,
            mY - yOff - 5,
            mWidth + 10,
            mHeight + 10,
            mBubble);
    }

    graphics->setColor(*mColor);
    if (!mIsSpeech)
        graphics->setColor2(mOutlineColor);

    mFont->drawString(graphics, mText, mX - xOff, mY - yOff);
    BLOCK_END("Text::draw")
}

FlashText::FlashText(const std::string &text, const int x, const int y,
                     const Graphics::Alignment alignment,
                     const Color *const color, Font *const font) :
    Text(text, x, y, alignment, color, false, font),
    mTime(0)
{
}

void FlashText::draw(Graphics *const graphics, const int xOff, const int yOff)
{
    BLOCK_START("FlashText::draw")
    if (mTime)
    {
        if ((--mTime & 4) == 0)
        {
            BLOCK_END("FlashText::draw")
            return;
        }
    }
    Text::draw(graphics, xOff, yOff);
    BLOCK_END("FlashText::draw")
}
