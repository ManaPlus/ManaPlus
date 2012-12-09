/*
 *  The ManaPlus Client
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

#include "gui/setup_touch.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"

#include "configuration.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Touch::Setup_Touch(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mSizeList(new SetupItemNames)
{
    setName(_("Touch"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemCheckBox(_("Show on screen buttons"), "",
        "showScreenButtons", this, "showScreenButtonsEvent");

    mSizeList->push_back(_("Small"));
    mSizeList->push_back(_("Normal"));
    mSizeList->push_back(_("Medium"));
    mSizeList->push_back(_("Large"));
    new SetupItemSlider2(_("Buttons size"), "", "screenButtonsSize", this,
        "screenButtonsSizeEvent", 0, 3, mSizeList);

    new SetupItemCheckBox(_("Show on screen joystick"), "",
        "showScreenJoystick", this, "showScreenJoystickEvent");

    new SetupItemSlider2(_("Joystick size"), "", "screenJoystickSize", this,
        "screenJoystickEvent", 0, 3, mSizeList);

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

Setup_Touch::~Setup_Touch()
{
    delete mSizeList;
    mSizeList = nullptr;
}
