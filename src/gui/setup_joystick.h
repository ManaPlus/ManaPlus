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

#ifndef GUI_SETUP_JOYSTICK_H
#define GUI_SETUP_JOYSTICK_H

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class DropDown;
class NamesModel;

namespace gcn
{
    class Button;
    class CheckBox;
    class Label;
}

class Setup_Joystick final : public SetupTab
{
    public:
        Setup_Joystick();

        ~Setup_Joystick();

        void apply();

        void cancel();

        void action(const gcn::ActionEvent &event);

        void setTempEnabled(const bool sel);

    private:
        gcn::Label *mCalibrateLabel;
        gcn::Button *mCalibrateButton;
        bool mOriginalJoystickEnabled;
        gcn::CheckBox *mJoystickEnabled;
        NamesModel *mNamesModel;
        DropDown *mNamesDropDown;
        gcn::CheckBox *mUseInactiveCheckBox;
};

#endif
