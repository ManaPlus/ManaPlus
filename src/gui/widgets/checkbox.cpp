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

#include "gui/widgets/checkbox.h"

#include "client.h"
#include "configuration.h"
#include "keydata.h"
#include "keyevent.h"

#include "resources/image.h"

#include <guichan/actionlistener.hpp>
#include <guichan/font.hpp>

#include "debug.h"

int CheckBox::instances = 0;
Skin *CheckBox::mSkin = nullptr;
float CheckBox::mAlpha = 1.0;

CheckBox::CheckBox(const Widget2 *const widget,
                   const std::string &caption, const bool selected,
                   gcn::ActionListener *const listener,
                   const std::string &eventId) :
    gcn::CheckBox(caption, selected),
    Widget2(widget),
    mHasMouse(false),
    mPadding(0),
    mImagePadding(0),
    mImageSize(9),
    mSpacing(2),
    mForegroundColor(getThemeColor(Theme::CHECKBOX_OUTLINE))
{
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

void CheckBox::draw(gcn::Graphics* graphics)
{
    BLOCK_START("CheckBox::draw")
    drawBox(graphics);

    gcn::Font *const font = getFont();
    static_cast<Graphics *const>(graphics)->setColorAll(
        mForegroundColor, mForegroundColor2);

    font->drawString(graphics, mCaption, mPadding + mImageSize + mSpacing,
        mPadding);
    BLOCK_END("CheckBox::draw")
}

void CheckBox::updateAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
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

void CheckBox::drawBox(gcn::Graphics *const graphics)
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
        static_cast<Graphics*>(graphics)->drawImage(
            box, mImagePadding, (getHeight() - mImageSize) / 2);
    }
}

void CheckBox::mouseEntered(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = true;
}

void CheckBox::mouseExited(gcn::MouseEvent& event A_UNUSED)
{
    mHasMouse = false;
}

void CheckBox::keyPressed(gcn::KeyEvent& keyEvent)
{
    const int action = static_cast<KeyEvent*>(&keyEvent)->getActionId();

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
