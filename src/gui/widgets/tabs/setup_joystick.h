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

#ifndef GUI_WIDGETS_TABS_SETUP_JOYSTICK_H
#define GUI_WIDGETS_TABS_SETUP_JOYSTICK_H

#include "gui/widgets/tabs/setuptab.h"

class Button;
class CheckBox;
class DropDown;
class Label;
class NamesModel;
class Slider;

class Setup_Joystick final : public SetupTab
{
    public:
        explicit Setup_Joystick(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Joystick)

        ~Setup_Joystick() override final;

        void apply() override final;

        void cancel() override final;

        void action(const ActionEvent &event) override final;

        void setTempEnabled(const bool sel);

    private:
        Button *mDetectButton A_NONNULLPOINTER;
        CheckBox *mJoystickEnabled A_NONNULLPOINTER;
        NamesModel *mNamesModel A_NONNULLPOINTER;
        DropDown *mNamesDropDown A_NONNULLPOINTER;
        Label *mToleranceLabel A_NONNULLPOINTER;
        Slider *mToleranceSlider A_NONNULLPOINTER;
        CheckBox *mUseInactiveCheckBox A_NONNULLPOINTER;
        bool mOriginalJoystickEnabled A_NONNULLPOINTER;
};

#endif  // GUI_WIDGETS_TABS_SETUP_JOYSTICK_H
