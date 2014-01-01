/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TEXTBOX_H
#define GUI_WIDGETS_TEXTBOX_H

#include "gui/widgets/widget2.h"

#include <guichan/widgets/textbox.hpp>

#include "localconsts.h"

/**
 * A text box, meant to be used inside a scroll area. Same as the Guichan text
 * box except this one doesn't have a background or border, instead completely
 * relying on the scroll area.
 *
 * \ingroup GUI
 */
class TextBox final : public gcn::TextBox,
                      public Widget2
{
    public:
        /**
         * Constructor.
         */
        explicit TextBox(const Widget2 *const widget);

        A_DELETE_COPY(TextBox)

        ~TextBox();

        /**
         * Sets the text after wrapping it to the current width of the widget.
         */
        void setTextWrapped(const std::string &text, const int minDimension);

        /**
         * Get the minimum text width for the text box.
         */
        int getMinWidth() const A_WARN_UNUSED
        { return mMinWidth; }

        void keyPressed(gcn::KeyEvent& keyEvent) override final;

        void draw(gcn::Graphics* graphics) override final;

        void setForegroundColor(const gcn::Color &color);

        void setForegroundColorAll(const gcn::Color &color1,
                                   const gcn::Color &color2);

    private:
        int mMinWidth;
};

#endif  // GUI_WIDGETS_TEXTBOX_H
