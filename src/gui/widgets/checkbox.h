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

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <guichan/widgets/checkbox.hpp>

#include "localconsts.h"

class Image;

/**
 * Check box widget. Same as the Guichan check box but with custom look.
 *
 * \ingroup GUI
 */
class CheckBox final : public gcn::CheckBox
{
    public:
        /**
         * Constructor.
         */
        CheckBox(const std::string &caption, bool selected = false,
                 gcn::ActionListener *const listener = nullptr,
                 const std::string &eventId = "");

        /**
         * Destructor.
         */
        ~CheckBox();

        /**
         * Draws the caption, then calls drawBox to draw the check box.
         */
        void draw(gcn::Graphics *const graphics);

        /**
         * Update the alpha value to the checkbox components.
         */
        void updateAlpha();

        /**
         * Draws the check box, not the caption.
         */
        void drawBox(gcn::Graphics* graphics);

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(gcn::MouseEvent& event);

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(gcn::MouseEvent& event);

        void keyPressed(gcn::KeyEvent& keyEvent);

    private:
        static int instances;
        static float mAlpha;
        bool mHasMouse;
        static Image *checkBoxNormal;
        static Image *checkBoxChecked;
        static Image *checkBoxDisabled;
        static Image *checkBoxDisabledChecked;
        static Image *checkBoxNormalHi;
        static Image *checkBoxCheckedHi;
};

#endif
