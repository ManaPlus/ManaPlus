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

#ifndef GUI_SETUP_JOYSTICK_H
#define GUI_SETUP_JOYSTICK_H

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class Button;
class CheckBox;
class DropDown;
class Label;
class NamesModel;

class Setup_Joystick final : public SetupTab
{
    public:
        explicit Setup_Joystick(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Joystick)

        ~Setup_Joystick();

        void apply() override;

        void cancel() override;

        void action(const gcn::ActionEvent &event) override;

        void setTempEnabled(const bool sel);

    private:
        Label *mCalibrateLabel;
        Button *mCalibrateButton;
        Button *mDetectButton;
        bool mOriginalJoystickEnabled;
        CheckBox *mJoystickEnabled;
        NamesModel *mNamesModel;
        DropDown *mNamesDropDown;
        CheckBox *mUseInactiveCheckBox;
};

#endif
