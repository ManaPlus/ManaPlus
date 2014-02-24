/*
 *  The ManaPlus Client
 *  Copyright (c) 2009  Aethyra Development Team 
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

#include "gui/widgets/label.h"

#include "gui/font.h"
#include "gui/gui.h"

#include "debug.h"

Skin *Label::mSkin = nullptr;
int Label::mInstances = 0;

Label::Label(const Widget2 *const widget) :
    gcn::Label(widget),
    mPadding(0)
{
    init();
}

Label::Label(const Widget2 *const widget,
             const std::string &caption) :
    gcn::Label(widget, caption),
    mPadding(0)
{
    init();
}

Label::~Label()
{
    if (gui)
        gui->removeDragged(this);

    mInstances --;
    if (mInstances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
            theme->unload(mSkin);
    }
}

void Label::init()
{
    mForegroundColor = getThemeColor(Theme::LABEL);
    mForegroundColor2 = getThemeColor(Theme::LABEL_OUTLINE);
    if (mInstances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
            mSkin = theme->load("label.xml", "");
    }
    mInstances ++;

    if (mSkin)
        mPadding = mSkin->getPadding();
    else
        mPadding = 0;
}

void Label::draw(Graphics* graphics)
{
    BLOCK_START("Label::draw")
    int textX;
    const Rect &rect = mDimension;
    const int textY = rect.height / 2 - getFont()->getHeight() / 2;
    Font *const font = getFont();

    switch (mAlignment)
    {
        case Graphics::LEFT:
        default:
            textX = mPadding;
            break;
        case Graphics::CENTER:
            textX = (rect.width - font->getWidth(mCaption)) / 2;
            break;
        case Graphics::RIGHT:
            if (rect.width > mPadding)
                textX = rect.width - mPadding - font->getWidth(mCaption);
            else
                textX = 0;
            break;
    }

    graphics->setColorAll(mForegroundColor, mForegroundColor2);
    font->drawString(graphics, mCaption, textX, textY);
    BLOCK_END("Label::draw")
}

void Label::adjustSize()
{
    const Font *const font = getFont();
    const int pad2 = 2 * mPadding;
    setWidth(font->getWidth(mCaption) + pad2);
    setHeight(font->getHeight() + pad2);
}

void Label::setForegroundColor(const Color &color)
{
    mForegroundColor = color;
    mForegroundColor2 = color;
}

void Label::setForegroundColorAll(const Color &color1,
                                  const Color &color2)
{
    mForegroundColor = color1;
    mForegroundColor2 = color2;
}

void Label::resizeTo(const int maxSize, const int minSize)
{
    const Font *const font = getFont();
    const int pad2 = 2 * mPadding;
    setHeight(font->getHeight() + pad2);

    if (font->getWidth(mCaption) + pad2 > maxSize)
    {
        const int dots = font->getWidth("...");
        if (dots > maxSize)
        {
            setWidth(maxSize);
            return;
        }
        const size_t szChars = mCaption.size();
        for (size_t f = 1; f < szChars - 1; f ++)
        {
            const std::string text = mCaption.substr(0, szChars - f);
            const int width = font->getWidth(text) + dots + pad2;
            if (width <= maxSize)
            {
                setCaption(text + "...");
                setWidth(width);
                return;
            }
        }
        setWidth(maxSize);
    }
    else
    {
        int sz = font->getWidth(mCaption) + pad2;
        if (sz < minSize)
            sz = minSize;
        setWidth(sz);
    }
}
