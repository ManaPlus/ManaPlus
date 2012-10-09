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

#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <guichan/widgets/scrollarea.hpp>
#include <guichan/widgetlistener.hpp>

class GraphicsVertexes;
class Image;
class ImageRect;

/**
 * A scroll area.
 *
 * Contrary to Guichan's scroll area, this scroll area takes ownership over its
 * content. However, it won't delete a previously set content widget when
 * setContent is called!
 *
 * \ingroup GUI
 */
class ScrollArea final : public gcn::ScrollArea, public gcn::WidgetListener
{
    public:
        /**
         * Constructor that takes no content. Needed for use with the DropDown
         * class.
         */
        ScrollArea(const bool opaque = true, const std::string &skin = "");

        /**
         * Constructor.
         *
         * @param content the initial content to show in the scroll area
         */
        ScrollArea(gcn::Widget *const widget, const bool opaque = true,
                   const std::string &skin = "");

        A_DELETE_COPY(ScrollArea);

        /**
         * Destructor. Also deletes the content.
         */
        ~ScrollArea();

        /**
         * Logic function optionally adapts width or height of contents. This
         * depends on the scrollbar settings.
         */
        void logic() override;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the scroll area.
         */
        void draw(gcn::Graphics *graphics) override;

        /**
         * Draws the background and border of the scroll area.
         */
        void drawFrame(gcn::Graphics *graphics) override;

        /**
         * Sets whether the widget should draw its background or not.
         */
        void setOpaque(bool opaque);

        /**
         * Returns whether the widget draws its background or not.
         */
        bool isOpaque() const
        { return mOpaque; }

        /**
         * Called when the mouse moves in the widget area.
         */
        void mouseMoved(gcn::MouseEvent& event) override;

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(gcn::MouseEvent& event) override;

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(gcn::MouseEvent& event) override;

        void mousePressed(gcn::MouseEvent& event) override;

        void mouseReleased(gcn::MouseEvent& event) override;

        void widgetResized(const gcn::Event &event) override;

        void widgetMoved(const gcn::Event &event) override;

    protected:
        enum BUTTON_DIR
        {
            UP = 0,
            DOWN,
            LEFT,
            RIGHT,
            BUTTONS_DIR
        };

        /**
         * Initializes the scroll area.
         */
        void init(std::string skinName);

        void drawButton(gcn::Graphics *const graphics, const BUTTON_DIR dir);
        void drawUpButton(gcn::Graphics *const graphics) override;
        void drawDownButton(gcn::Graphics *const graphics) override;
        void drawLeftButton(gcn::Graphics *const graphics) override;
        void drawRightButton(gcn::Graphics *const graphics) override;
        void drawVBar(gcn::Graphics *const graphics) override;
        void drawHBar(gcn::Graphics *const graphics) override;
        void drawVMarker(gcn::Graphics *const graphics) override;
        void drawHMarker(gcn::Graphics *const graphics) override;

        static int instances;
        static float mAlpha;
        static ImageRect background;
        static ImageRect vMarker;
        static ImageRect vMarkerHi;
        static ImageRect vBackground;
        static ImageRect hBackground;
        static Image *buttons[4][2];

        int mX, mY;
        int mClickX, mClickY;
        bool mHasMouse;
        bool mOpaque;
        GraphicsVertexes *mVertexes;
        bool mRedraw;
        int mXOffset;
        int mYOffset;
        int mDrawWidth;
        int mDrawHeight;
};

#endif
