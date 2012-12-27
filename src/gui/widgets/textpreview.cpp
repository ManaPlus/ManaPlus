/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "configuration.h"

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
    mBGColor(&getThemeColor(Theme::BACKGROUND)),
    mTextBGColor(nullptr),
    mTextAlpha(false),
    mOpaque(false),
    mShadow(false),
    mOutline(false),
    mPadding(0)
{
    if (instances == 0)
    {
        if (Theme::instance())
            mSkin = Theme::instance()->load("textpreview.xml", "");
    }

    instances++;

    if (mSkin)
        mPadding = mSkin->getOption("padding", 0);

    adjustSize();
}

TextPreview::~TextPreview()
{
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
    if (Client::getGuiAlpha() != mAlpha)
        mAlpha = Client::getGuiAlpha();

    const int alpha = mTextAlpha ? static_cast<int>(mAlpha * 255.0f) : 255;

    if (mOpaque)
    {
        graphics->setColor(gcn::Color(static_cast<int>(mBGColor->r),
                                      static_cast<int>(mBGColor->g),
                                      static_cast<int>(mBGColor->b),
                                      static_cast<int>(mAlpha * 255.0f)));
        graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
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
            graphics->setColor(gcn::Color(static_cast<int>(mTextBGColor->r),
                                          static_cast<int>(mTextBGColor->g),
                                          static_cast<int>(mTextBGColor->b),
                                          static_cast<int>(mAlpha * 255.0f)));
            graphics->fillRectangle(gcn::Rectangle(mPadding, mPadding, x, y));
        }
    }

    graphics->setColor(gcn::Color(mTextColor->r, mTextColor->g,
        mTextColor->b, alpha));

    if (mOutline)
    {
        Graphics *const g = static_cast<Graphics *const>(graphics);
        g->setColor2(Theme::getThemeColor(Theme::OUTLINE));
    }

    mFont->drawString(graphics, mText, mPadding + 1, mPadding + 1);
    BLOCK_END("TextPreview::draw")
}

void TextPreview::adjustSize()
{
    setHeight(getFont()->getHeight() + 2 * mPadding);
}
