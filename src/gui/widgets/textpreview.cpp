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
#include "textrenderer.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include <typeinfo>

#include "debug.h"

float TextPreview::mAlpha = 1.0;

TextPreview::TextPreview(const std::string &text):
    mFont(gui->getFont()),
    mText(text),
    mTextColor(&Theme::getThemeColor(Theme::TEXT)),
    mBGColor(&Theme::getThemeColor(Theme::BACKGROUND)),
    mTextBGColor(nullptr),
    mTextAlpha(false),
    mOpaque(false),
    mShadow(false),
    mOutline(false)
{
}

void TextPreview::draw(gcn::Graphics* graphics)
{
    if (Client::getGuiAlpha() != mAlpha)
        mAlpha = Client::getGuiAlpha();

    int alpha = static_cast<int>(mAlpha * 255.0f);

    if (!mTextAlpha)
        alpha = 255;

    if (mOpaque)
    {
        graphics->setColor(gcn::Color(static_cast<int>(mBGColor->r),
                                      static_cast<int>(mBGColor->g),
                                      static_cast<int>(mBGColor->b),
                                      static_cast<int>(mAlpha * 255.0f)));
        graphics->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
    }

    if (mTextBGColor && typeid(*mFont) == typeid(SDLFont))
    {
        SDLFont *font = static_cast<SDLFont*>(mFont);
        int x = font->getWidth(mText) + 1 + 2 * ((mOutline || mShadow) ? 1 :0);
        int y = font->getHeight() + 1 + 2 * ((mOutline || mShadow) ? 1 : 0);
        graphics->setColor(gcn::Color(static_cast<int>(mTextBGColor->r),
                                      static_cast<int>(mTextBGColor->g),
                                      static_cast<int>(mTextBGColor->b),
                                      static_cast<int>(mAlpha * 255.0f)));
        graphics->fillRectangle(gcn::Rectangle(1, 1, x, y));
    }

    TextRenderer::renderText(graphics, mText, 2, 2,  gcn::Graphics::LEFT,
                             gcn::Color(mTextColor->r, mTextColor->g,
                                        mTextColor->b, alpha),
                             mFont, mOutline, mShadow);
}
