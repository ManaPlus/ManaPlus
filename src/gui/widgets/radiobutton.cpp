/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/radiobutton.h"

#include "settings.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/fonts/font.h"

#include "render/graphics.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "debug.h"

int RadioButton::instances = 0;
Skin *RadioButton::mSkin = nullptr;
float RadioButton::mAlpha = 1.0;

RadioButton::GroupMap RadioButton::mGroupMap;

RadioButton::RadioButton(const Widget2 *const widget,
                         const std::string &restrict caption,
                         const std::string &restrict group,
                         const bool marked) :
    Widget(widget),
    MouseListener(),
    KeyListener(),
    WidgetListener(),
    mSelected(false),
    mCaption(),
    mGroup(),
    mTextChunk(),
    mPadding(0),
    mImagePadding(0),
    mImageSize(9),
    mSpacing(2),
    mTextX(0 + 9 + 2),
    mHasMouse(false),
    mTextChanged(true)
{
    mAllowLogic = false;
    setCaption(caption);
    setGroup(group);
    setSelected(marked);

    setFocusable(true);
    addMouseListener(this);
    addKeyListener(this);

    mForegroundColor = getThemeColor(ThemeColorId::RADIOBUTTON, 255U);
    mForegroundColor2 = getThemeColor(ThemeColorId::RADIOBUTTON_OUTLINE, 255U);
    if (instances == 0)
    {
        if (theme != nullptr)
        {
            mSkin = theme->load("radio.xml",
                "",
                true,
                theme->getThemePath());
            updateAlpha();
        }
    }

    instances++;

    if (mSkin != nullptr)
    {
        mPadding = mSkin->getPadding();
        mImagePadding = mSkin->getOption("imagePadding");
        mImageSize = mSkin->getOption("imageSize");
        mSpacing = mSkin->getOption("spacing");
        mTextX = mPadding + mImageSize + mSpacing;
    }

    adjustSize();
}

RadioButton::~RadioButton()
{
    if (mWindow != nullptr)
        mWindow->removeWidgetListener(this);

    setGroup(std::string());

    if (gui != nullptr)
        gui->removeDragged(this);

    instances--;

    if (instances == 0)
    {
        if (theme != nullptr)
            theme->unload(mSkin);
    }
}

void RadioButton::updateAlpha()
{
    const float alpha = std::max(settings.guiAlpha,
        theme->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        if (mSkin != nullptr)
        {
            const ImageRect &rect = mSkin->getBorder();
            for (int a = 0; a < 4; a ++)
            {
                Image *const image = rect.grid[a];
                if (image != nullptr)
                    image->setAlpha(mAlpha);
            }
        }
    }
}

void RadioButton::drawBox(Graphics *const graphics)
{
    if (mSkin == nullptr)
        return;

    const ImageRect &rect = mSkin->getBorder();
    int index = 0;

    if (mEnabled && mVisible == Visible_true)
    {
        if (mSelected)
        {
            if (mHasMouse)
                index = 1;
            else
                index = 0;
        }
        else
        {
            if (mHasMouse)
                index = 3;
            else
                index = 2;
        }
    }
    else
    {
        if (mSelected)
            index = 0;
        else
            index = 2;
    }

    const Image *const box = rect.grid[index];

    updateAlpha();

    if (box != nullptr)
    {
        graphics->drawImage(box,
            mImagePadding,
            (getHeight() - mImageSize) / 2);
    }
}

void RadioButton::draw(Graphics *const graphics)
{
    BLOCK_START("RadioButton::draw")
    drawBox(graphics);

    Font *const font = getFont();

    if (mTextChanged)
    {
        mTextChunk.textFont = font;
        mTextChunk.deleteImage();
        mTextChunk.text = mCaption;
        mTextChunk.color = mForegroundColor;
        mTextChunk.color2 = mForegroundColor2;
        font->generate(mTextChunk);
        mTextChanged = false;
    }

    const Image *const image = mTextChunk.img;
    if (image != nullptr)
        graphics->drawImage(image, mTextX, mPadding);

    BLOCK_END("RadioButton::draw")
}

void RadioButton::safeDraw(Graphics *const graphics)
{
    RadioButton::draw(graphics);
}

void RadioButton::mouseEntered(MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void RadioButton::mouseExited(MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void RadioButton::keyPressed(KeyEvent& event)
{
    const InputActionT action = event.getActionId();
    if (action == InputAction::GUI_SELECT)
    {
        setSelected(true);
        distributeActionEvent();
        event.consume();
    }
}

void RadioButton::adjustSize()
{
    Font *const font = getFont();
    setHeight(font->getHeight() + 2 * mPadding);
    setWidth(mImagePadding + mImageSize + mSpacing
        + font->getWidth(mCaption) + mPadding);
}

void RadioButton::setSelected(const bool selected)
{
    if (selected && !mGroup.empty())
    {
        for (GroupIterator iter = mGroupMap.lower_bound(mGroup),
             iterEnd = mGroupMap.upper_bound(mGroup);
             iter != iterEnd;
             ++ iter)
        {
            if ((iter->second != nullptr) && iter->second->isSelected())
                iter->second->setSelected(false);
        }
    }

    mSelected = selected;
}

void RadioButton::mouseClicked(MouseEvent& event)
{
    if (event.getButton() == MouseButton::LEFT)
    {
        setSelected(true);
        event.consume();
        distributeActionEvent();
    }
}

void RadioButton::mouseDragged(MouseEvent& event)
{
    event.consume();
}

void RadioButton::setGroup(const std::string &group)
{
    if (!mGroup.empty())
    {
        for (GroupIterator iter = mGroupMap.lower_bound(mGroup),
             iterEnd = mGroupMap.upper_bound(mGroup);
             iter != iterEnd;
             ++ iter)
        {
            if (iter->second == this)
            {
                mGroupMap.erase(iter);
                break;
            }
        }
    }

    if (!group.empty())
        mGroupMap.insert(std::pair<std::string, RadioButton *>(group, this));

    mGroup = group;
}

void RadioButton::setCaption(const std::string& caption)
{
    if (caption != mCaption)
        mTextChanged = true;
    mCaption = caption;
}

void RadioButton::setParent(Widget *widget)
{
    if (mWindow != nullptr)
        mWindow->addWidgetListener(this);
    Widget::setParent(widget);
}

void RadioButton::widgetHidden(const Event &event A_UNUSED)
{
    mTextChanged = true;
    mTextChunk.deleteImage();
}

void RadioButton::setWindow(Widget *const widget)
{
    if ((widget == nullptr) && (mWindow != nullptr))
    {
        mWindow->removeWidgetListener(this);
        mWindow = nullptr;
    }
    else
    {
        Widget2::setWindow(widget);
    }
}
