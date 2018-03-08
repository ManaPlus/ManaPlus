/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/widgets/pincode.h"

#include "gui/gui.h"
#include "gui/skin.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

Skin *Pincode::mSkin = nullptr;
int Pincode::mInstances = 0;

Pincode::Pincode(const Widget2 *const widget,
                 TextField *const textField) :
    Container(widget),
    ActionListener(),
    WidgetListener(),
    mButtons(),
    mText(),
    mTextField(textField),
    mPadding(0),
    mSpacing(2)
{
    mAllowLogic = false;
    setOpaque(Opaque_false);

    if (mInstances == 0)
    {
        if (theme != nullptr)
        {
            mSkin = theme->load("pin.xml",
                "",
                true,
                Theme::getThemePath());
        }
    }
    mInstances ++;

    if (mSkin != nullptr)
    {
        mPadding = mSkin->getPadding();
        mSpacing = mSkin->getOption("spacing", 2);
    }
    setSelectable(false);

    addButtons();
    adjustSize();
}

Pincode::~Pincode()
{
    if (mWindow != nullptr)
        mWindow->removeWidgetListener(this);

    if (gui != nullptr)
        gui->removeDragged(this);

    mInstances --;
    if (mInstances == 0)
    {
        if (theme != nullptr)
            theme->unload(mSkin);
    }
}

void Pincode::addButtons()
{
    int buttonWidth = 0;
    int buttonHeight = 0;
    for (int f = 0; f < 10; f ++)
    {
        const std::string str = toString(f);
        mButtons[f] = new Button(this,
            str,
            str,
            BUTTON_PIN_SKIN,
            this);
        mButtons[f]->adjustSize();
        const Rect &rect = mButtons[f]->getDimension();
        if (rect.width > buttonWidth)
            buttonWidth = rect.width;
        if (rect.height > buttonHeight)
            buttonHeight = rect.height;
        add(mButtons[f]);
    }
    int x = 0;
    int y = 0;
    const int xSize = buttonWidth + mSpacing;
    const int ySize = buttonHeight + mSpacing;
    for (int f = 0; f < 10; f ++)
    {
        mButtons[f]->setPosition(mPadding + x * xSize,
            mPadding + y * ySize);
        x ++;
        if (x > 2)
        {
            x = 0;
            y ++;
        }
    }
    mButtons[10] = new Button(this,
        // TRANSLATORS: clear pin code button
        _("Clear"),
        "clear",
        BUTTON_PIN_SKIN,
        this);
    mButtons[10]->adjustSize();
    add(mButtons[10]);
    mButtons[10]->setPosition(mPadding + xSize,
        mPadding + 3 * ySize);

    mButtonWidth = buttonWidth;
    mButtonHeight = buttonHeight;
}

void Pincode::adjustSize()
{
    const int pad2 = mPadding * 2;
    setSize(pad2 + 3 * mButtonWidth + 2 * mSpacing,
        pad2 + 4 * mButtonHeight + 3 * mSpacing);
}

void Pincode::finalCleanup()
{
    mSkin = nullptr;
}

void Pincode::action(const ActionEvent &event)
{
    if (event.getId() == "clear")
    {
        mText.clear();
        mTextField->setText(mText);
        mTextField->signalEvent();
        return;
    }
    if (mText.size() >= 4)
        return;
    Widget *const eventSrc = event.getSource();
    for (int f = 0; f < 10; f ++)
    {
        if (mButtons[f] == eventSrc)
        {
            const std::string str = toString(f);
            mText.append(str);
            mTextField->setText(mText);
            mTextField->signalEvent();
            return;
        }
    }
}

void Pincode::shuffle(uint32_t seed)
{
    int tab[10];
    const uint32_t multiplier = 0x3498;
    const uint32_t baseSeed = 0x881234;
    int k = 2;

    for (size_t f = 0; f < 10; f ++)
        tab[f] = f;
    for (size_t f = 1; f < 10; f ++)
    {
        seed = baseSeed + seed * multiplier;
        const size_t pos = seed % k;
        if (f != pos)
        {
            const int tmp = tab[f];
            tab[f] = tab[pos];
            tab[pos] = tmp;
        }
        k = k + 1;
    }
    for (size_t f = 0; f < 10; f ++)
    {
        const std::string str = toString(tab[f]);
        mButtons[f]->setCaption(str);
    }
}
