/*
 *  The ManaPlus Client
 *  Copyright (c) 2009  Aethyra Development Team 
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

#ifndef GUI_WIDGETS_LABEL_H
#define GUI_WIDGETS_LABEL_H

#include "gui/widgets/widget2.h"

#include <guichan/widgets/label.hpp>

#include "localconsts.h"

class Skin;

/**
 * Label widget. Same as the Guichan label but modified to use the palette
 * system.
 *
 * \ingroup GUI
 */
class Label final : public gcn::Label, public Widget2
{
    public:
        /**
         * Constructor.
         */
        explicit Label(const Widget2 *const widget);

        /**
         * Constructor. This version of the constructor sets the label with an
         * inintialization string.
         */
        Label(const Widget2 *const widget, const std::string &caption);

        A_DELETE_COPY(Label)

        ~Label();

        void init();

        /**
         * Draws the label.
         */
        void draw(gcn::Graphics *graphics) override final;

        void adjustSize();

        void setForegroundColor(const gcn::Color &color);

        void setForegroundColorAll(const gcn::Color &color1,
                                   const gcn::Color &color2);

        void resizeTo(const int maxSize, const int minSize);

        static Skin *mSkin;

        static int mInstances;

    private:
        int mPadding;
};

#endif  // GUI_WIDGETS_LABEL_H
