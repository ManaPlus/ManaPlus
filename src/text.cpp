/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "textrenderer.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"

#include "resources/resourcemanager.h"
#include "resources/image.h"

#include <guichan/font.hpp>

#include "debug.h"

int Text::mInstances = 0;
ImageRect Text::mBubble;
Image *Text::mBubbleArrow;

Text::Text(const std::string &text, int x, int y,
           gcn::Graphics::Alignment alignment,
           const gcn::Color* color, bool isSpeech,
           gcn::Font *font) :
    mText(text),
    mColor(color),
    mIsSpeech(isSpeech)
{
    if (!font)
        mFont = gui->getFont();
    else
        mFont = font;

    if (textManager == 0)
    {
        textManager = new TextManager;
        Image *sbImage = Theme::getImageFromTheme("bubble.png|W:#"
            + config.getStringValue("speechBubblecolor"));
        if (sbImage)
        {
            mBubble.grid[0] = sbImage->getSubImage(0, 0, 5, 5);
            mBubble.grid[1] = sbImage->getSubImage(5, 0, 5, 5);
            mBubble.grid[2] = sbImage->getSubImage(10, 0, 5, 5);
            mBubble.grid[3] = sbImage->getSubImage(0, 5, 5, 5);
            mBubble.grid[4] = sbImage->getSubImage(5, 5, 5, 5);
            mBubble.grid[5] = sbImage->getSubImage(10, 5, 5, 5);
            mBubble.grid[6] = sbImage->getSubImage(0, 10, 5, 5);
            mBubble.grid[7] = sbImage->getSubImage(5, 10, 5, 5);
            mBubble.grid[8] = sbImage->getSubImage(10, 10, 5, 5);
            mBubbleArrow = sbImage->getSubImage(0, 15, 15, 10);
        }
        else
        {
            for (int f = 0; f < 9; f ++)
                mBubble.grid[f] = 0;
            mBubbleArrow = 0;
        }
        const float bubbleAlpha = config.getFloatValue("speechBubbleAlpha");
        for (int i = 0; i < 9; i++)
        {
            if (mBubble.grid[i])
                mBubble.grid[i]->setAlpha(bubbleAlpha);
        }
        if (mBubbleArrow)
            mBubbleArrow->setAlpha(bubbleAlpha);
        if (sbImage)
            sbImage->decRef();
    }
    ++mInstances;
    mHeight = mFont->getHeight();
    mWidth = mFont->getWidth(text);

    switch (alignment)
    {
        case gcn::Graphics::LEFT:
            mXOffset = 0;
            break;
        case gcn::Graphics::CENTER:
            mXOffset = mWidth / 2;
            break;
        case gcn::Graphics::RIGHT:
            mXOffset = mWidth;
            break;
        default:
            break;
    }
    mX = x - mXOffset;
    mY = y;
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
        textManager = 0;
        delete mBubble.grid[0];
        mBubble.grid[0] = 0;
        delete mBubble.grid[1];
        mBubble.grid[1] = 0;
        delete mBubble.grid[2];
        mBubble.grid[2] = 0;
        delete mBubble.grid[3];
        mBubble.grid[3] = 0;
        delete mBubble.grid[4];
        mBubble.grid[4] = 0;
        delete mBubble.grid[5];
        mBubble.grid[5] = 0;
        delete mBubble.grid[6];
        mBubble.grid[6] = 0;
        delete mBubble.grid[7];
        mBubble.grid[7] = 0;
        delete mBubble.grid[8];
        mBubble.grid[8] = 0;
        delete mBubbleArrow;
        mBubbleArrow = 0;
    }
}

void Text::setColor(const gcn::Color *color)
{
    mColor = color;
}

void Text::adviseXY(int x, int y)
{
    if (textManager)
        textManager->moveText(this, x - mXOffset, y);
}

void Text::draw(gcn::Graphics *graphics, int xOff, int yOff)
{
    if (mIsSpeech)
    {
        static_cast<Graphics*>(graphics)->drawImageRect(
                mX - xOff - 5, mY - yOff - 5, mWidth + 10, mHeight + 10,
                mBubble);
        /*
        if (mWidth >= 15)
        {
            static_cast<Graphics*>(graphics)->drawImage(
                    mBubbleArrow, mX - xOff - 7 + mWidth / 2,
                    mY - yOff + mHeight + 4);
        }
        */
    }

    TextRenderer::renderText(graphics, mText,
            mX - xOff, mY - yOff, gcn::Graphics::LEFT,
            *mColor, mFont, !mIsSpeech, true);
}

FlashText::FlashText(const std::string &text, int x, int y,
                     gcn::Graphics::Alignment alignment,
                     const gcn::Color *color, gcn::Font *font) :
    Text(text, x, y, alignment, color, false, font),
    mTime(0)
{
}

void FlashText::draw(gcn::Graphics *graphics, int xOff, int yOff)
{
    if (mTime)
    {
        if ((--mTime & 4) == 0)
            return;
    }
    Text::draw(graphics, xOff, yOff);
}
