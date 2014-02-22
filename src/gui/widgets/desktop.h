/*
 *  Desktop widget
 *  Copyright (c) 2009-2010  The Mana World Development Team
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

#ifndef GUI_WIDGETS_DESKTOP_H
#define GUI_WIDGETS_DESKTOP_H

#include "gui/widgets/container.h"

#include "listeners/widgetlistener.h"

#include "localconsts.h"

class Image;
class Label;
class Skin;

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
class Desktop final : public Container,
                      private WidgetListener
{
    public:
        explicit Desktop(const Widget2 *const widget);

        A_DELETE_COPY(Desktop)

        ~Desktop();

        /**
         * Has to be called after updates have been loaded.
         */
        void reloadWallpaper();

        void widgetResized(const Event &event) override final;

        void draw(Graphics *graphics) override final;

        void postInit();

    private:
        void setBestFittingWallpaper();

        Image *mWallpaper;
        Label *mVersionLabel;
        Skin *mSkin;
        Color mBackgroundColor;
        Color mBackgroundGrayColor;
        bool mShowBackground;
};

#endif  // GUI_WIDGETS_DESKTOP_H
