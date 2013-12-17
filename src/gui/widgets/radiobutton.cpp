/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/widgets/radiobutton.h"

#include "client.h"

#include "input/keydata.h"
#include "input/keyevent.h"

#include "resources/image.h"

#include <guichan/font.hpp>

#include "debug.h"

int RadioButton::instances = 0;
Skin *RadioButton::mSkin = nullptr;
float RadioButton::mAlpha = 1.0;

RadioButton::RadioButton(const Widget2 *const widget,
                         const std::string &restrict caption,
                         const std::string &restrict group,
                         const bool marked):
    gcn::RadioButton(caption, group, marked),
    Widget2(widget),
    mPadding(0),
    mImagePadding(0),
    mImageSize(9),
    mSpacing(2),
    mHasMouse(false)
{
    mForegroundColor = getThemeColor(Theme::RADIOBUTTON);
    mForegroundColor2 = getThemeColor(Theme::RADIOBUTTON_OUTLINE);
    if (instances == 0)
    {
        Theme *const theme = Theme::instance();
        if (theme)
        {
            mSkin = theme->load("radio.xml", "");
            updateAlpha();
        }
    }

    instances++;

    if (mSkin)
    {
        mPadding = mSkin->getPadding();
        mImagePadding = mSkin->getOption("imagePadding");
        mImageSize = mSkin->getOption("imageSize");
        mSpacing = mSkin->getOption("spacing");
    }

    adjustSize();
}

RadioButton::~RadioButton()
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

void RadioButton::updateAlpha()
{
    const float alpha = std::max(client->getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        if (mSkin)
        {
            const ImageRect &rect = mSkin->getBorder();
            for (int a = 0; a < 4; a ++)
            {
                Image *const image = rect.grid[a];
                if (image)
                    image->setAlpha(mAlpha);
            }
        }
    }
}

void RadioButton::drawBox(gcn::Graphics* graphics)
{
    if (!mSkin)
        return;

    const ImageRect &rect = mSkin->getBorder();
    int index = 0;

    if (mEnabled && isVisible())
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

    if (box)
    {
        DRAW_IMAGE(static_cast<Graphics*>(graphics),
            box, mImagePadding, (getHeight() - mImageSize) / 2);
    }
}

void RadioButton::draw(gcn::Graphics* graphics)
{
    BLOCK_START("RadioButton::draw")
    drawBox(graphics);

    gcn::Font *const font = getFont();
    static_cast<Graphics *const>(graphics)->setColorAll(
        mForegroundColor, mForegroundColor2);

    font->drawString(graphics, mCaption, mPadding + mImageSize + mSpacing,
        mPadding);
    BLOCK_END("RadioButton::draw")
}

void RadioButton::mouseEntered(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void RadioButton::mouseExited(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void RadioButton::keyPressed(gcn::KeyEvent& keyEvent)
{
    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();
    if (action == Input::KEY_GUI_SELECT)
    {
        setSelected(true);
        distributeActionEvent();
        keyEvent.consume();
    }
}

void RadioButton::adjustSize()
{
    gcn::Font *const font = getFont();
    setHeight(font->getHeight() + 2 * mPadding);
    setWidth(mImagePadding + mImageSize + mSpacing
        + font->getWidth(mCaption) + mPadding);
}
