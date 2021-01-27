/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_WIDGETS_INTTEXTFIELD_H
#define GUI_WIDGETS_INTTEXTFIELD_H

#include "enums/simpletypes/enable.h"

#include "gui/widgets/textfield.h"

/**
 * TextBox which only accepts numbers as input.
 */
class IntTextField final : public TextField
{
    public:
        /**
         * Constructor, sets default value.
         */
        IntTextField(const Widget2 *const widget,
                     const int def,
                     const int min,
                     const int max,
                     const Enable enabled,
                     const int width);

        A_DELETE_COPY(IntTextField)

        /**
         * Sets the minimum and maximum values of the text box.
         */
        void setRange(const int minimum, const int maximum);

        /**
         * Returns the value in the text box.
         */
        int getValue() const A_WARN_UNUSED;

        /**
         * Reset the field to the default value.
         */
        void reset();

        /**
         * Set the value of the text box to the specified value.
         */
        void setValue(const int value);

        /**
         * Set the default value of the text box to the specified value.
         */
        void setDefaultValue(const int value);

        /**
         * Responds to key presses.
         */
        void keyPressed(KeyEvent &event) override final;

    private:
        int mMin;            /**< Minimum value */
        int mMax;            /**< Maximum value */
        int mDefault;        /**< Default value */
        int mValue;          /**< Current value */
};

#endif  // GUI_WIDGETS_INTTEXTFIELD_H
