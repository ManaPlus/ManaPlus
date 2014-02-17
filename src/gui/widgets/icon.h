/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_ICON_H
#define GUI_WIDGETS_ICON_H

#include "gui/widgets/widget2.h"

#include "gui/base/widget.hpp"

#include "localconsts.h"

class Image;

/**
 * An icon.
 *
 * \ingroup GUI
 */
class Icon final : public gcn::Widget
{
    public:
        /**
         * Constructor.
         */
        Icon(const Widget2 *const widget,
             const std::string &filename);

        /**
         * Constructor, uses an existing Image.
         */
        Icon(const Widget2 *const widget,
             Image *const image);

        A_DELETE_COPY(Icon)

        ~Icon();

        /** 
         * Gets the current Image.
         */
        Image *getImage() const A_WARN_UNUSED
        { return mImage; }

        /**
         * Sets the image to display.
         */
        void setImage(Image *const image);

        /**
         * Draws the Icon.
         */
        void draw(Graphics *g) override final;

    private:
        Image *mImage;
};

#endif  // GUI_WIDGETS_ICON_H
