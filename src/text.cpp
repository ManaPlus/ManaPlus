/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/gui.h"
#include "gui/theme.h"

#include "gui/fonts/font.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "utils/delete2.h"

#include "debug.h"

int Text::mInstances = 0;
ImageRect Text::mBubble;

Text::Text(const std::string &text,
           const int x, const int y,
           const Graphics::Alignment alignment,
           const Color *const color,
           const Speech isSpeech,
           Font *const font) :
    mFont(font != nullptr ?
        font : (gui != nullptr ? gui->getFont() : nullptr)),
    mTextChunk(),
    mX(x),
    mY(y),
    mWidth(mFont != nullptr ? mFont->getWidth(text) : 1),
    mHeight(mFont != nullptr ? mFont->getHeight() : 1),
    mXOffset(0),
    mText(text),
    mColor(color),
    mOutlineColor(color != nullptr ? (isSpeech == Speech_true ?
        *color : theme->getColor(ThemeColorId::OUTLINE, 255)) : Color()),
    mIsSpeech(isSpeech),
    mTextChanged(true)
{
    if (textManager == nullptr)
    {
        textManager = new TextManager;
        if (theme != nullptr)
        {
            theme->loadRect(mBubble,
                "bubble.xml",
                "",
                0,
                8);
        }
        else
        {
            for (int f = 0; f < 9; f ++)
                mBubble.grid[f] = nullptr;
        }

        const float bubbleAlpha = config.getFloatValue("speechBubbleAlpha");
        for (int i = 0; i < 9; i++)
        {
            if (mBubble.grid[i] != nullptr)
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
    if (textManager != nullptr)
        textManager->addText(this);
}

Text::~Text()
{
    if (textManager != nullptr)
        textManager->removeText(this);
    if (--mInstances == 0)
    {
        delete2(textManager);
        for (int f = 0; f < 9; f ++)
        {
            if (mBubble.grid[f] != nullptr)
            {
                mBubble.grid[f]->decRef();
                mBubble.grid[f] = nullptr;
            }
        }
    }
}

void Text::setColor(const Color *const color)
{
    mTextChanged = true;
    if (mIsSpeech == Speech_true)
    {
        mColor = color;
        mOutlineColor = *color;
    }
    else
    {
        mColor = color;
    }
}

void Text::adviseXY(const int x, const int y,
                    const Move move)
{
    if ((textManager != nullptr) && move == Move_true)
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
    if (mIsSpeech == Speech_true)
    {
        graphics->drawImageRect(mX - xOff - 5,
            mY - yOff - 5,
            mWidth + 10,
            mHeight + 10,
            mBubble);
    }

    if (mTextChanged)
    {
        mTextChunk.textFont = mFont;
        mTextChunk.deleteImage();
        mTextChunk.text = mText;
        mTextChunk.color = *mColor;
        mTextChunk.color2 = mOutlineColor;
        mFont->generate(mTextChunk);
        mTextChanged = false;
    }

    const Image *const image = mTextChunk.img;
    if (image != nullptr)
        graphics->drawImage(image, mX - xOff, mY - yOff);

    BLOCK_END("Text::draw")
}

FlashText::FlashText(const std::string &text,
                     const int x, const int y,
                     const Graphics::Alignment alignment,
                     const Color *const color,
                     Font *const font) :
    Text(text, x, y, alignment, color, Speech_false, font),
    mTime(0)
{
}

void FlashText::draw(Graphics *const graphics, const int xOff, const int yOff)
{
    BLOCK_START("FlashText::draw")
    if (mTime != 0)
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
