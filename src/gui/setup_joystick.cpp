/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "gui/setup_joystick.h"

#include "configuration.h"
#include "joystick.h"
#include "logger.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "utils/gettext.h"

#include <guichan/listmodel.hpp>

#include "debug.h"

extern Joystick *joystick;

class NamesModel : public gcn::ListModel
{
    public:
        NamesModel()
        { }

        virtual ~NamesModel()
        { }

        virtual int getNumberOfElements()
        {
            return static_cast<int>(mNames.size());
        }

        virtual std::string getElementAt(int i)
        {
            if (i >= getNumberOfElements() || i < 0)
                return _("???");

            return mNames[i];
        }

        std::vector<std::string> mNames;
};


Setup_Joystick::Setup_Joystick():
    mCalibrateLabel(new Label(_("Press the button to start calibration"))),
    mCalibrateButton(new Button(_("Calibrate"), "calibrate", this)),
    mJoystickEnabled(new CheckBox(_("Enable joystick"))),
    mNamesModel(new NamesModel()),
    mNamesDropDown(new DropDown(mNamesModel)),
    mUseInactiveCheckBox(new CheckBox(_("Use joystick if client "
        "window inactive"), config.getBoolValue("useInactiveJoystick")))
{
    setName(_("Joystick"));

    Joystick::getNames(mNamesModel->mNames);

    mOriginalJoystickEnabled = config.getBoolValue("joystickEnabled");
    mJoystickEnabled->setSelected(mOriginalJoystickEnabled);
    mJoystickEnabled->setActionEventId("joystick");
    mJoystickEnabled->addActionListener(this);
    mCalibrateButton->setEnabled(mOriginalJoystickEnabled);

    mNamesDropDown->setActionEventId("name");
    mNamesDropDown->addActionListener(this);

    if (joystick)
    {
        mNamesDropDown->setSelected(joystick->getNumber());
    }
    else
    {
        unsigned sel = config.getIntValue("selectedJoystick");
        if (sel >= mNamesModel->mNames.size())
            sel = 0;
        mNamesDropDown->setSelected(sel);
    }

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mJoystickEnabled);
    place(0, 1, mNamesDropDown);
    place(0, 2, mUseInactiveCheckBox);
    place(0, 3, mCalibrateLabel);
    place(0, 4, mCalibrateButton);

    setDimension(gcn::Rectangle(0, 0, 365, 75));
}

Setup_Joystick::~Setup_Joystick()
{
    delete mNamesModel;
    mNamesModel = nullptr;
}

void Setup_Joystick::action(const gcn::ActionEvent &event)
{
    if (event.getSource() == mJoystickEnabled)
    {
        setTempEnabled(mJoystickEnabled->isSelected());
    }
    else if (event.getSource() == mNamesDropDown)
    {
        if (joystick)
            joystick->setNumber(mNamesDropDown->getSelected());
    }
    else
    {
        if (!joystick)
            return;

        if (joystick->isCalibrating())
        {
            mCalibrateButton->setCaption(_("Calibrate"));
            mCalibrateLabel->setCaption
                (_("Press the button to start calibration"));
            joystick->finishCalibration();
        }
        else
        {
            mCalibrateButton->setCaption(_("Stop"));
            mCalibrateLabel->setCaption(
                _("Rotate the stick and dont press buttons"));
            joystick->startCalibration();
        }
    }
}

void Setup_Joystick::setTempEnabled(bool sel)
{
    Joystick::setEnabled(sel);
    mCalibrateButton->setEnabled(sel);
    if (joystick)
    {
        if (sel)
            joystick->open();
        else
            joystick->close();
    }
}

void Setup_Joystick::cancel()
{
    if (joystick)
        joystick->setEnabled(mOriginalJoystickEnabled);

    if (mOriginalJoystickEnabled != mJoystickEnabled->isSelected())
        setTempEnabled(mOriginalJoystickEnabled);

    mJoystickEnabled->setSelected(mOriginalJoystickEnabled);
}

void Setup_Joystick::apply()
{
    if (!joystick)
        return;

    config.setValue("joystickEnabled",
        joystick ? joystick->isEnabled() : false);

    config.setValue("useInactiveJoystick", mUseInactiveCheckBox->isSelected());
    if (joystick)
        joystick->setUseInactive(mUseInactiveCheckBox->isSelected());
}
