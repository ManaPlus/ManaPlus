/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_POPUPS_TEXTBOXPOPUP_H
#define GUI_POPUPS_TEXTBOXPOPUP_H

#include "gui/widgets/popup.h"

class TextBox;

/**
 * A popup that displays information about an item.
 */
class TextBoxPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the item popup.
         */
        TextBoxPopup();

        A_DELETE_COPY(TextBoxPopup)

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~TextBoxPopup() override final;

        void postInit() override final;

        /**
         * Sets the text to be displayed.
         */
        void show(const int x, const int y, const std::string &str);

        void mouseMoved(MouseEvent &event) override final;

    private:
        TextBox *mTextBox A_NONNULLPOINTER;
};

extern TextBoxPopup *textBoxPopup;

#endif  // GUI_POPUPS_TEXTBOXPOPUP_H
