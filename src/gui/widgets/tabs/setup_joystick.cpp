/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/tabs/setup_joystick.h"

#include "configuration.h"

#include "input/joystick.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/slider.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

Setup_Joystick::Setup_Joystick(const Widget2 *const widget) :
    SetupTab(widget),
    // TRANSLATORS: joystick settings tab button
    mDetectButton(new Button(this, _("Detect joysticks"), "detect",
        BUTTON_SKIN, this)),
    // TRANSLATORS: joystick settings tab checkbox
    mJoystickEnabled(new CheckBox(this, _("Enable joystick"),
        false, nullptr, std::string())),
    mNamesModel(new NamesModel),
    mNamesDropDown(new DropDown(this, mNamesModel,
        false, Modal_false, nullptr, std::string())),
    mToleranceLabel(new Label(this)),
    mToleranceSlider(new Slider(this, 0.01, 1, 0.01)),
    mUseHatForMovementCheckBox(new CheckBox(this,
        // TRANSLATORS: joystick settings tab checkbox
        _("Use joystick hat (d-pad) for movement"),
        config.getBoolValue("useHatForMovement"),
        nullptr, std::string())),
    mUseInactiveCheckBox(new CheckBox(this,
        // TRANSLATORS: joystick settings tab checkbox
        _("Use joystick if client window inactive"),
        config.getBoolValue("useInactiveJoystick"),
        nullptr, std::string())),
    mOriginalJoystickEnabled(config.getBoolValue("joystickEnabled"))
{
    // TRANSLATORS: joystick settings tab name
    setName(_("Joystick"));

    Joystick::getNames(mNamesModel->getNames());

    mJoystickEnabled->setSelected(mOriginalJoystickEnabled);
    mJoystickEnabled->setActionEventId("joystick");
    mJoystickEnabled->addActionListener(this);

    float tolerance = config.getFloatValue("joystickTolerance");
    mToleranceSlider->setValue(tolerance);
    // TRANSLATORS: joystick settings tab label
    mToleranceLabel->setCaption(_("Axis tolerance: ") + strprintf("%.2f", tolerance));
    mToleranceLabel->setWidth(150);
    mToleranceLabel->setHeight(20);

    mNamesDropDown->setActionEventId("name");
    mNamesDropDown->addActionListener(this);
    mToleranceSlider->setActionEventId("toleranceslider");
    mToleranceSlider->addActionListener(this);

    if (joystick != nullptr)
    {
        mNamesDropDown->setSelected(joystick->getNumber());
    }
    else
    {
        unsigned int sel = config.getIntValue("selectedJoystick");
        if (sel >= CAST_U32(mNamesModel->size()))
            sel = 0;
        mNamesDropDown->setSelected(sel);
    }

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mJoystickEnabled, 1, 1);
    place(0, 1, mNamesDropDown, 1, 1);

    place(0, 2, mToleranceSlider, 1, 1);
    place(1, 2, mToleranceLabel, 1, 1).setPadding(3);

    place(0, 3, mUseHatForMovementCheckBox, 1, 1);
    place(0, 4, mUseInactiveCheckBox, 1, 1);
    place(0, 5, mDetectButton, 1, 1);

    setDimension(Rect(0, 0, 365, 75));
}

Setup_Joystick::~Setup_Joystick()
{
    delete2(mNamesModel)
}

void Setup_Joystick::action(const ActionEvent &event)
{
    const Widget *const source = event.getSource();
    if (source == mJoystickEnabled)
    {
        setTempEnabled(mJoystickEnabled->isSelected());
    }
    else if (source == mNamesDropDown)
    {
        if (joystick != nullptr)
            joystick->setNumber(mNamesDropDown->getSelected());
    }
    else if (source == mToleranceSlider)
    {
        float tolerance = mToleranceSlider->getValue();
        // TRANSLATORS: joystick settings tab label
        mToleranceLabel->setCaption(_("Axis tolerance: ") + strprintf("%.2f", tolerance));
    }
    else if (source == mDetectButton)
    {
        if (joystick != nullptr)
        {
            joystick->reload();
            Joystick::getNames(mNamesModel->getNames());
            mNamesDropDown->setSelected(joystick->getNumber());
        }
    }
}

void Setup_Joystick::setTempEnabled(const bool sel)
{
    Joystick::setEnabled(sel);
    if (joystick != nullptr)
    {
        if (sel)
            joystick->open();
        else
            joystick->close();
    }
}

void Setup_Joystick::cancel()
{
    Joystick::setEnabled(mOriginalJoystickEnabled);

    if (mOriginalJoystickEnabled != mJoystickEnabled->isSelected())
        setTempEnabled(mOriginalJoystickEnabled);

    mJoystickEnabled->setSelected(mOriginalJoystickEnabled);
}

void Setup_Joystick::apply()
{
    if (joystick == nullptr)
        return;

    config.setValue("joystickEnabled", Joystick::isEnabled());

    config.setValue("useHatForMovement", mUseHatForMovementCheckBox->isSelected());
    joystick->setUseHatForMovement(mUseHatForMovementCheckBox->isSelected());
    config.setValue("useInactiveJoystick", mUseInactiveCheckBox->isSelected());
    joystick->setUseInactive(mUseInactiveCheckBox->isSelected());

    float tolerance = mToleranceSlider->getValue();
    config.setValue("joystickTolerance", tolerance);
    joystick->setTolerance(tolerance);
}
