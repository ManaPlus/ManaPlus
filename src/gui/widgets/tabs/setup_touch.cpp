/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/tabs/setup_touch.h"

#include "gui/models/touchactionmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/setuptouchitem.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

static const int sizeListSize = 4;

static const char *const sizeList[] =
{
    // TRANSLATORS: onscreen button size
    N_("Small"),
    // TRANSLATORS: onscreen button size
    N_("Normal"),
    // TRANSLATORS: onscreen button size
    N_("Medium"),
    // TRANSLATORS: onscreen button size
    N_("Large")
};

static const int formatListSize = 6;

static const char *const formatList[] =
{
    "2x1",
    "2x2",
    "3x3",
    "4x2",
    "4x3",
    "3x2"
};

Setup_Touch::Setup_Touch(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mSizeList(new NamesModel),
    mFormatList(new NamesModel),
    mActionsList(new TouchActionsModel)
{
    // TRANSLATORS: touch settings tab
    setName(_("Touch"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);
    mSizeList->fillFromArray(&sizeList[0], sizeListSize);
    mFormatList->fillFromArray(&formatList[0], formatListSize);

    // TRANSLATORS: settings option
    new SetupItemLabel(_("Onscreen keyboard"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show onscreen keyboard icon"), "",
        "showScreenKeyboard", this, "showScreenKeyboardEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupActionDropDown(_("Keyboard icon action"), "",
        "screenActionKeyboard", this, "screenActionKeyboardEvent",
        mActionsList, 250, MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Onscreen joystick"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show onscreen joystick"), "",
        "showScreenJoystick", this, "showScreenJoystickEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Joystick size"), "", "screenJoystickSize", this,
        "screenJoystickEvent", mSizeList, 100,
        MainConfig_true);


    // TRANSLATORS: settings group
    new SetupItemLabel(_("Onscreen buttons"), "", this,
        Separator_true);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show onscreen buttons"), "",
        "showScreenButtons", this, "showScreenButtonsEvent",
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Buttons format"), "", "screenButtonsFormat", this,
        "screenButtonsFormatEvent", mFormatList, 100,
        MainConfig_true);

    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Buttons size"), "", "screenButtonsSize", this,
        "screenButtonsSizeEvent", mSizeList, 100,
        MainConfig_true);

    for (unsigned int f = 0; f < 12; f ++)
    {
        std::string key = strprintf("screenActionButton%u", f);
        std::string event = strprintf("screenActionButton%uEvent", f);
        // TRANSLATORS: settings option
        new SetupActionDropDown(strprintf(_("Button %u action"), f + 1), "",
            key, this, event, mActionsList, 250, MainConfig_true);
    }

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Touch::~Setup_Touch()
{
    delete2(mSizeList)
    delete2(mFormatList)
    delete2(mActionsList)
}
