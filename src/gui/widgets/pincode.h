/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_PINCODE_H
#define GUI_WIDGETS_PINCODE_H

#include "gui/widgets/container.h"

#include "listeners/actionlistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

class Button;
class Skin;
class TextField;

class Pincode final : public Container,
                      public ActionListener,
                      public WidgetListener
{
    public:
        Pincode(const Widget2 *const widget,
                TextField *const textField);

        A_DELETE_COPY(Pincode)

        ~Pincode() override final;

        void adjustSize();

        void action(const ActionEvent &event) override final;

        void shuffle(uint32_t seed) const;

        static Skin *mSkin;

        static int mInstances;

        static void finalCleanup();

    protected:
        void addButtons();

    private:
        Button *mButtons[11];
        std::string mText;
        TextField *mTextField;
        int mPadding;
        int mSpacing;
        int mButtonWidth;
        int mButtonHeight;
};

#endif  // GUI_WIDGETS_PINCODE_H
