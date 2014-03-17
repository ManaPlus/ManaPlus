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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gui/widgets/label.h"

#include "gui/font.h"
#include "gui/gui.h"

#include "debug.h"

Skin *Label::mSkin = nullptr;
int Label::mInstances = 0;

Label::Label(const Widget2 *const widget) :
    Widget(widget),
    mCaption(),
    mAlignment(Graphics::LEFT),
    mPadding(0)
{
    init();
}

Label::Label(const Widget2 *const widget,
             const std::string &caption) :
    Widget(widget),
    mCaption(caption),
    mAlignment(Graphics::LEFT),
    mPadding(0)
{
    const Font *const font = getFont();
    if (font)
    {
        setWidth(font->getWidth(caption));
        setHeight(font->getHeight());
    }
    init();
}

Label::~Label()
{
    if (gui)
        gui->removeDragged(this);

    mInstances --;
    if (mInstances == 0)
    {
        if (theme)
            theme->unload(mSkin);
    }
}

void Label::init()
{
    mAllowLogic = false;
    mForegroundColor = getThemeColor(Theme::LABEL);
    mForegroundColor2 = getThemeColor(Theme::LABEL_OUTLINE);
    if (mInstances == 0)
    {
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
