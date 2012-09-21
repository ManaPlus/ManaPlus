/*
 *  Desktop widget
 *  Copyright (c) 2009-2010  The Mana World Development Team
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

#ifndef DESKTOP_H
#define DESKTOP_H

#include "gui/widgets/container.h"

#include <guichan/widgetlistener.hpp>

class Image;

namespace gcn
{
    class Label;
}

/**
 * Desktop widget, for drawing a background image and color.
 *
 * It picks the best fitting background image. If the image doesn't fit, a
 * background color is drawn and the image is centered.
 *
 * When the desktop widget is resized, the background image is automatically
 * updated.
 *
 * The desktop also displays the client version in the top-right corner.
 *
 * \ingroup GUI
 */
class Desktop final : public Container, private gcn::WidgetListener
{
    public:
        Desktop();
        ~Desktop();

        /**
         * Has to be called after updates have been loaded.
         */
        void reloadWallpaper();

        void widgetResized(const gcn::Event &event);

        void draw(gcn::Graphics *graphics);

    private:
        void setBestFittingWallpaper();

        Image *mWallpaper;
        gcn::Label *mVersionLabel;
        gcn::Color mBackgroundColor;
        gcn::Color mBackgroundGrayColor;
};

#endif // DESKTOP_H
