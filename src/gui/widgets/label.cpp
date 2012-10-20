/*
 *  The ManaPlus Client
 *  Copyright (c) 2009  Aethyra Development Team 
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

#include "gui/widgets/label.h"

#include <guichan/font.hpp>

#include "debug.h"

Skin *Label::mSkin = nullptr;
int Label::mInstances = 0;

Label::Label(const Widget2 *const widget) :
    gcn::Label(),
    Widget2(widget),
    mPadding(0)
{
    init();
}

Label::Label(const Widget2 *const widget, const std::string &caption) :
    gcn::Label(caption),
    Widget2(widget),
    mPadding(0)
{
    init();
}

Label::~Label()
{
    mInstances --;
    if (mInstances == 0 && Theme::instance())
        Theme::instance()->unload(mSkin);
}

void Label::init()
{
    mForegroundColor = getThemeColor(Theme::LABEL);
    if (mInstances == 0)
    {
        if (Theme::instance())
            mSkin = Theme::instance()->load("label.xml", "");
    }
    mInstances ++;

    if (mSkin)
        mPadding = mSkin->getPadding();
    else
        mPadding = 0;
}

void Label::draw(gcn::Graphics* graphics)
{
    int textX;
    const int textY = getHeight() / 2 - getFont()->getHeight() / 2;

    switch (getAlignment())
    {
        case Graphics::LEFT:
        default:
            textX = mPadding;
            break;
        case Graphics::CENTER:
            textX = getWidth() / 2;
            break;
        case Graphics::RIGHT:
            if (getWidth() > mPadding)
                textX = getWidth() - mPadding;
            else
                textX = 0;
            break;
    }

    graphics->setFont(getFont());
    graphics->setColor(mForegroundColor);
    graphics->drawText(getCaption(), textX, textY, getAlignment());
}

void Label::adjustSize()
{
    setWidth(getFont()->getWidth(getCaption()) + 2 * mPadding);
    setHeight(getFont()->getHeight() + 2 * mPadding);
}
