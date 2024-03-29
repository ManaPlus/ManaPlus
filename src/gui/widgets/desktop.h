/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana World Development Team
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

#ifndef GUI_WIDGETS_DESKTOP_H
#define GUI_WIDGETS_DESKTOP_H

#include "gui/widgets/container.h"

#include "gui/widgets/linkhandler.h"

#include "listeners/widgetlistener.h"

#include "localconsts.h"

class StaticBrowserBox;
class Image;
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
                      public LinkHandler,
                      public WidgetListener
{
    public:
        explicit Desktop(const Widget2 *const widget);

        A_DELETE_COPY(Desktop)

        ~Desktop() override final;

        /**
         * Has to be called after updates have been loaded.
         */
        void reloadWallpaper();

        void widgetResized(const Event &event) override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void postInit() override final;

        void handleLink(const std::string &link,
                        MouseEvent *event) override final;
    private:
        void setBestFittingWallpaper();

        Image *mWallpaper;
        StaticBrowserBox *mVersionLabel;
        Skin *mSkin;
        Color mBackgroundGrayColor;
        bool mShowBackground;
};

extern Desktop *desktop;

#endif  // GUI_WIDGETS_DESKTOP_H
