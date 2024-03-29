/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_POPUPS_SPEECHBUBBLE_H
#define GUI_POPUPS_SPEECHBUBBLE_H

#include "gui/widgets/popup.h"

class Label;
class StaticBrowserBox;

class SpeechBubble final : public Popup
{
    public:
        /**
         * Constructor. Initializes the speech bubble.
         */
        SpeechBubble();

        A_DELETE_COPY(SpeechBubble)

        void postInit() override final;

        /**
         * Sets the name displayed for the speech bubble, and in what color.
         */
        void setCaption(const std::string &name,
                        const Color *const color1,
                        const Color *const color2);

        /**
         * Sets the text to be displayed.
         */
        void setText(const std::string &text,
                     const bool showName);

        void requestMoveToBackground();

    private:
        std::string mText;
        int mSpacing;
        Label *mCaption A_NONNULLPOINTER;
        StaticBrowserBox *mSpeechBox A_NONNULLPOINTER;
};

#endif  // GUI_POPUPS_SPEECHBUBBLE_H
