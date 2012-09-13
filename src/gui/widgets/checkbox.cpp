/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/checkbox.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/palette.h"
#include "gui/theme.h"

#include "resources/image.h"

#include <guichan/actionlistener.hpp>

#include "debug.h"

int CheckBox::instances = 0;
float CheckBox::mAlpha = 1.0;
Image *CheckBox::checkBoxNormal;
Image *CheckBox::checkBoxChecked;
Image *CheckBox::checkBoxDisabled;
Image *CheckBox::checkBoxDisabledChecked;
Image *CheckBox::checkBoxNormalHi;
Image *CheckBox::checkBoxCheckedHi;

CheckBox::CheckBox(const std::string &caption, const bool selected,
                   gcn::ActionListener *const listener,
                   const std::string &eventId) :
    gcn::CheckBox(caption, selected),
    mHasMouse(false)
{
    if (instances == 0)
    {
        if (Theme::instance())
        {
            ImageRect rect;
            Theme::instance()->loadRect(rect, "checkbox.xml", "", 0, 5);
            checkBoxNormal = rect.grid[0];
            checkBoxChecked = rect.grid[1];
            checkBoxDisabled = rect.grid[2];
            checkBoxDisabledChecked = rect.grid[3];
            checkBoxNormalHi = rect.grid[4];
            checkBoxCheckedHi = rect.grid[5];

            if (checkBoxNormal)
                checkBoxNormal->setAlpha(mAlpha);
            if (checkBoxChecked)
                checkBoxChecked->setAlpha(mAlpha);
            if (checkBoxDisabled)
                checkBoxDisabled->setAlpha(mAlpha);
            if (checkBoxDisabledChecked)
                checkBoxDisabledChecked->setAlpha(mAlpha);
            if (checkBoxNormalHi)
                checkBoxNormalHi->setAlpha(mAlpha);
            if (checkBoxCheckedHi)
                checkBoxCheckedHi->setAlpha(mAlpha);
        }
        else
        {
            checkBoxNormal = nullptr;
            checkBoxChecked = nullptr;
            checkBoxDisabled = nullptr;
            checkBoxDisabledChecked = nullptr;
            checkBoxNormalHi = nullptr;
            checkBoxCheckedHi = nullptr;
        }
    }

    instances++;

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);

    setForegroundColor(Theme::getThemeColor(Theme::CHECKBOX));
}

CheckBox::~CheckBox()
{
    instances--;

    if (instances == 0)
    {
        if (checkBoxNormal)
        {
            checkBoxNormal->decRef();
            checkBoxNormal = nullptr;
        }
        if (checkBoxChecked)
        {
            checkBoxChecked->decRef();
            checkBoxChecked = nullptr;
        }
        if (checkBoxDisabled)
        {
            checkBoxDisabled->decRef();
            checkBoxDisabled = nullptr;
        }
        if (checkBoxDisabledChecked)
        {
            checkBoxDisabledChecked->decRef();
            checkBoxDisabledChecked = nullptr;
        }
        if (checkBoxNormalHi)
        {
            checkBoxNormalHi->decRef();
            checkBoxNormalHi = nullptr;
        }
        if (checkBoxCheckedHi)
        {
            checkBoxCheckedHi->decRef();
            checkBoxCheckedHi = nullptr;
        }
    }
}

void CheckBox::draw(gcn::Graphics* graphics)
{
    drawBox(graphics);

    graphics->setFont(getFont());
    graphics->setColor(getForegroundColor());

    const int h = getHeight() + getHeight() / 2;

    graphics->drawText(getCaption(), h - 2, 0);
}

void CheckBox::updateAlpha()
{
    const float alpha = std::max(Client::getGuiAlpha(),
        Theme::instance()->getMinimumOpacity());

    if (mAlpha != alpha)
    {
        mAlpha = alpha;
        if (checkBoxNormal)
            checkBoxNormal->setAlpha(mAlpha);
        if (checkBoxChecked)
            checkBoxChecked->setAlpha(mAlpha);
        if (checkBoxDisabled)
            checkBoxDisabled->setAlpha(mAlpha);
        if (checkBoxDisabledChecked)
            checkBoxDisabledChecked->setAlpha(mAlpha);
        if (checkBoxNormal)
            checkBoxNormal->setAlpha(mAlpha);
        if (checkBoxCheckedHi)
            checkBoxCheckedHi->setAlpha(mAlpha);
    }
}

void CheckBox::drawBox(gcn::Graphics *const graphics)
{
    const Image *box;

    if (mEnabled && isVisible())
    {
        if (mSelected)
        {
            if (mHasMouse)
                box = checkBoxCheckedHi;
            else
                box = checkBoxChecked;
        }
        else
        {
            if (mHasMouse)
                box = checkBoxNormalHi;
            else
                box = checkBoxNormal;
        }
    }
    else
    {
        if (mSelected)
            box = checkBoxDisabledChecked;
        else
            box = checkBoxDisabled;
    }

    updateAlpha();

    if (box)
        static_cast<Graphics*>(graphics)->drawImage(box, 2, 2);
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
