/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "gui/widgets/textpreview.h"

#include "client.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "debug.h"

int TextPreview::instances = 0;
float TextPreview::mAlpha = 1.0;
Skin *TextPreview::mSkin = nullptr;

TextPreview::TextPreview(const Widget2 *const widget,
                         const std::string &text) :
    gcn::Widget(),
    Widget2(widget),
    mFont(gui->getFont()),
    mText(text),
    mTextColor(&getThemeColor(Theme::TEXT)),
    mTextColor2(&getThemeColor(Theme::TEXT_OUTLINE)),
    mBGColor(&getThemeColor(Theme::BACKGROUND)),
    mTextBGColor(nullptr),
    mPadding(0),
    mTextAlpha(false),
    mOpaque(false),
    mShadow(false),
    mOutline(false)
{
    if (instances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
            mSkin = theme->load("textpreview.xml", "");
    }

    instances++;

    if (mSkin)
        mPadding = mSkin->getOption("padding", 0);

    adjustSize();
}

TextPreview::~TextPreview()
{
    if (gui)
        gui->removeDragged(this);

    instances--;

    if (instances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
            theme->unload(mSkin);
    }
}

void TextPreview::draw(gcn::Graphics* graphics)
{
    BLOCK_START("TextPreview::draw")
    if (client->getGuiAlpha() != mAlpha)
        mAlpha = client->getGuiAlpha();
    Graphics *const g = static_cast<Graphics*>(graphics);

    const int intAlpha = static_cast<int>(mAlpha * 255.0F);
    const int alpha = mTextAlpha ? intAlpha : 255;

    if (mOpaque)
    {
        g->setColor(gcn::Color(static_cast<int>(mBGColor->r),
                    static_cast<int>(mBGColor->g),
                    static_cast<int>(mBGColor->b),
                    static_cast<int>(mAlpha * 255.0F)));
        g->fillRectangle(gcn::Rectangle(0, 0,
            mDimension.width, mDimension.height));
    }

    if (mTextBGColor)
    {
        const SDLFont *const font = dynamic_cast<SDLFont*>(mFont);
        if (font)
        {
            const int x = font->getWidth(mText) + 1
                + 2 * ((mOutline || mShadow) ? 1 :0);
            const int y = font->getHeight() + 1
                + 2 * ((mOutline || mShadow) ? 1 : 0);
            g->setColor(gcn::Color(static_cast<int>(mTextBGColor->r),
                static_cast<int>(mTextBGColor->g),
                static_cast<int>(mTextBGColor->b),
                intAlpha));
            g->fillRectangle(gcn::Rectangle(mPadding, mPadding, x, y));
        }
    }

    g->setColorAll(gcn::Color(mTextColor->r, mTextColor->g, mTextColor->b,
        alpha), gcn::Color(mTextColor2->r, mTextColor2->g, mTextColor2->b,
        alpha));

    if (mOutline && mTextColor != mTextColor2)
        g->setColor2(Theme::getThemeColor(Theme::OUTLINE));

    mFont->drawString(graphics, mText, mPadding + 1, mPadding + 1);
    BLOCK_END("TextPreview::draw")
}

void TextPreview::adjustSize()
{
    setHeight(getFont()->getHeight() + 2 * mPadding);
}
