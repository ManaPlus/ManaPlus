/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/checkbox.h"

#include "client.h"

#include "input/keydata.h"

#include "resources/image.h"

#include "gui/font.h"
#include "gui/gui.h"

#include "debug.h"

int CheckBox::instances = 0;
Skin *CheckBox::mSkin = nullptr;
float CheckBox::mAlpha = 1.0;

CheckBox::CheckBox(const Widget2 *const widget,
                   const std::string &restrict caption,
                   const bool selected,
                   ActionListener *const listener,
                   const std::string &restrict eventId) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    mSelected(selected),
    mCaption(),
    mPadding(0),
    mImagePadding(0),
    mImageSize(9),
    mSpacing(2),
    mHasMouse(false),
    mDrawBox(true)
{
    setCaption(caption);

    setFocusable(true);
    addMouseListener(this);
    addKeyListener(this);

    mForegroundColor2 = getThemeColor(Theme::CHECKBOX_OUTLINE);
    if (instances == 0)
    {
        if (Theme::instance())
        {
            mSkin = Theme::instance()->load("checkbox.xml", "");
            updateAlpha();
        }
    }

    instances++;

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);

    mForegroundColor = getThemeColor(Theme::CHECKBOX);
    if (mSkin)
    {
        mPadding = mSkin->getPadding();
        mImagePadding = mSkin->getOption("imagePadding");
        mImageSize = mSkin->getOption("imageSize");
        mSpacing = mSkin->getOption("spacing");
        mDrawBox = mSkin->getOption("drawBox", 1);
    }
    adjustSize();
}

CheckBox::~CheckBox()
{
    if (gui)
        gui->removeDragged(this);

    instances--;

    if (instances == 0)
    {
        if (Theme::instance())
            Theme::instance()->unload(mSkin);
    }
}

void CheckBox::draw(Graphics *const graphics)
{
    BLOCK_START("CheckBox::draw")
    drawBox(graphics);

    Font *const font = getFont();
    graphics->setColorAll(mForegroundColor, mForegroundColor2);
    font->drawString(graphics, mCaption, mPadding + mImageSize + mSpacing,
        mPadding);
    BLOCK_END("CheckBox::draw")
}

void CheckBox::updateAlpha()
{
    const float alpha = std::max(client->getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        if (mSkin)
        {
            const ImageRect &rect = mSkin->getBorder();
            for (int a = 0; a < 6; a ++)
            {
                Image *const image = rect.grid[a];
                if (image)
                    image->setAlpha(mAlpha);
            }
        }
    }
}

void CheckBox::drawBox(Graphics *const graphics)
{
    if (!mSkin || !mDrawBox)
        return;

    const ImageRect &rect = mSkin->getBorder();
    int index = 0;

    if (mEnabled && isVisible())
    {
        if (mSelected)
        {
            if (mHasMouse)
                index = 5;
            else
                index = 1;
        }
        else
        {
            if (mHasMouse)
                index = 4;
            else
                index = 0;
        }
    }
    else
    {
        if (mSelected)
            index = 3;
        else
            index = 2;
    }
    const Image *const box = rect.grid[index];

    updateAlpha();

    if (box)
    {
        graphics->drawImage(box,
            mImagePadding,
            (getHeight() - mImageSize) / 2);
    }
}

void CheckBox::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void CheckBox::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void CheckBox::keyPressed(KeyEvent& keyEvent)
{
    const int action = keyEvent.getActionId();

    if (action == Input::KEY_GUI_SELECT)
    {
        toggleSelected();
        keyEvent.consume();
    }
}

void CheckBox::adjustSize()
{
    setHeight(getFont()->getHeight() + 2 * mPadding);
    setWidth(mImagePadding + mImageSize + mSpacing
        + getFont()->getWidth(mCaption) + mPadding);
}

void CheckBox::mouseClicked(MouseEvent& mouseEvent)
{
    if (mouseEvent.getButton() == MouseEvent::LEFT)
        toggleSelected();
}

void CheckBox::mouseDragged(MouseEvent& mouseEvent)
{
    mouseEvent.consume();
}

void CheckBox::toggleSelected()
{
    mSelected = !mSelected;
    distributeActionEvent();
}
