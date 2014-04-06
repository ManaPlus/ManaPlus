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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GUI_WIDGETS_SCROLLAREA_H
#define GUI_WIDGETS_SCROLLAREA_H

#include "gui/widgets/basiccontainer.h"

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "localconsts.h"

class Image;
class ImageCollection;

/**
 * A scroll area.
 *
 * Contrary to Guichan's scroll area, this scroll area takes ownership over its
 * content. However, it won't delete a previously set content widget when
 * setContent is called!
 *
 * \ingroup GUI
 */
class ScrollArea final : public BasicContainer,
                         public MouseListener,
                         public WidgetListener
{
    public:
        /**
         * Scrollpolicies for the horizontal and vertical scrollbar.
         * The policies are:
         *
         * SHOW_ALWAYS - Always show the scrollbars no matter what.
         * SHOW_NEVER  - Never show the scrollbars no matter waht.
         * SHOW_AUTO   - Show the scrollbars only when needed. That is if the
         *               content grows larger then the ScrollArea.
         */
        enum ScrollPolicy
        {
            SHOW_ALWAYS = 0,
            SHOW_NEVER,
            SHOW_AUTO
        };

        /**
         * Constructor.
         *
         * @param content the initial content to show in the scroll area
         */
        ScrollArea(Widget2 *const widget2,
                   Widget *const widget,
                   const bool opaque = true,
                   const std::string &skin = "");

        A_DELETE_COPY(ScrollArea)

        /**
         * Destructor. Also deletes the content.
         */
        ~ScrollArea();

        /**
         * Logic function optionally adapts width or height of contents. This
         * depends on the scrollbar settings.
         */
        void logic() override final;

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the scroll area.
         */
        void draw(Graphics *graphics) override final;

        /**
         * Draws the background and border of the scroll area.
         */
        void drawFrame(Graphics *graphics) override final;

        /**
         * Sets whether the widget should draw its background or not.
         */
        void setOpaque(bool opaque);

        /**
         * Returns whether the widget draws its background or not.
         */
        bool isOpaque() const A_WARN_UNUSED
        { return mOpaque; }

        /**
         * Called when the mouse moves in the widget area.
         */
        void mouseMoved(MouseEvent& event) override final;

        /**
         * Called when the mouse enteres the widget area.
         */
        void mouseEntered(MouseEvent& event) override final;

        /**
         * Called when the mouse leaves the widget area.
         */
        void mouseExited(MouseEvent& event) override final;

        void mousePressed(MouseEvent& event) override final;

        void mouseReleased(MouseEvent& event) override final;

        void mouseDragged(MouseEvent &event) override final;

        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event &event) override final;

        Rect getVerticalBarDimension() const;

        Rect getHorizontalBarDimension() const;

        Rect getVerticalMarkerDimension();

        Rect getHorizontalMarkerDimension();

        Rect getUpButtonDimension() const;

        Rect getDownButtonDimension() const;

        Rect getLeftButtonDimension() const;

        Rect getRightButtonDimension() const;

        /**
         * Sets the content.
         *
         * @param widget The content of the scroll area.
         */
        void setContent(Widget* widget);

        /**
         * Gets the content.
         *
         * @return The content of the scroll area.
         */
        Widget* getContent();

        /**
         * Sets the horizontal scrollbar policy. See enum with policies.
         *
         * @param hPolicy The policy for the horizontal scrollbar.
         * @see getHorizontalScrollPolicy
         */
        void setHorizontalScrollPolicy(const ScrollPolicy hPolicy);

        /**
         * Gets the horizontal scrollbar policy. See enum with policies.
         *
         * @return The policy for the horizontal scrollbar policy.
         * @see setHorizontalScrollPolicy, setScrollPolicy
         */
        ScrollPolicy getHorizontalScrollPolicy() const
        { return mHPolicy; }

        /**
         * Sets the vertical scrollbar policy. See enum with policies.
         *
         * @param vPolicy The policy for the vertical scrollbar.
         * @see getVerticalScrollPolicy
         */
        void setVerticalScrollPolicy(const ScrollPolicy vPolicy);

        /**
         * Gets the vertical scrollbar policy. See enum with policies.
         *
         * @return The policy for the vertical scrollbar.
         * @see setVerticalScrollPolicy, setScrollPolicy
         */
        ScrollPolicy getVerticalScrollPolicy() const
        { return mVPolicy; }

        /**
         * Sets the horizontal and vertical scrollbar policy.
         *
         * @param hPolicy The policy for the horizontal scrollbar.
         * @param vPolicy The policy for the vertical scrollbar.
         * @see getVerticalScrollPolicy, getHorizontalScrollPolicy
         */
        void setScrollPolicy(const ScrollPolicy hPolicy,
                             const ScrollPolicy vPolicy);

        /**
         * Sets the amount to scroll vertically.
         *
         * @param vScroll The amount to scroll.
         * @see getVerticalScrollAmount
         */
        void setVerticalScrollAmount(const int vScroll);

        /**
         * Gets the amount that is scrolled vertically.
         *
         * @return The scroll amount on vertical scroll.
         * @see setVerticalScrollAmount, setScrollAmount
         */
        int getVerticalScrollAmount() const
        { return mVScroll; }

        /**
         * Sets the amount to scroll horizontally.
         *
         * @param hScroll The amount to scroll.
         * @see getHorizontalScrollAmount
         */
        void setHorizontalScrollAmount(int hScroll);

        /**
         * Gets the amount that is scrolled horizontally.
         *
         * @return The scroll amount on horizontal scroll.
         * @see setHorizontalScrollAmount, setScrollAmount
         */
        int getHorizontalScrollAmount() const
        { return mHScroll; }

        /**
         * Sets the amount to scroll horizontally and vertically.
         *
         * @param hScroll The amount to scroll on horizontal scroll.
         * @param vScroll The amount to scroll on vertical scroll.
         * @see getHorizontalScrollAmount, getVerticalScrollAmount
         */
        void setScrollAmount(const int hScroll, const int vScroll);

        /**
         * Gets the maximum amount of horizontal scroll.
         *
         * @return The horizontal max scroll.
         */
        int getHorizontalMaxScroll();

        /**
         * Gets the maximum amount of vertical scroll.
         *
         * @return The vertical max scroll.
         */
        int getVerticalMaxScroll();

        /**
         * Sets the width of the scroll bars.
         *
         * @param width The width of the scroll bars.
         * @see getScrollbarWidth
         */
        void setScrollbarWidth(const int width);

        /**
         * Gets the width of the scroll bars.
         *
         * @return the width of the ScrollBar.
         * @see setScrollbarWidth
         */
        int getScrollbarWidth() const
        { return mScrollbarWidth; }

        /**
         * Sets the amount to scroll in pixels when the left scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels. 
         * @see getLeftButtonScrollAmount
         */
        void setLeftButtonScrollAmount(const int amount)
        { mLeftButtonScrollAmount = amount; }

        /**
         * Sets the amount to scroll in pixels when the right scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getRightButtonScrollAmount
         */
        void setRightButtonScrollAmount(const int amount)
        { mRightButtonScrollAmount = amount; }

        /**
         * Sets the amount to scroll in pixels when the up scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getUpButtonScrollAmount
         */
        void setUpButtonScrollAmount(const int amount)
        { mUpButtonScrollAmount = amount; }

        /**
         * Sets the amount to scroll in pixels when the down scroll button is
         * pushed.
         *
         * @param amount The amount to scroll in pixels.
         * @see getDownButtonScrollAmount
         */
        void setDownButtonScrollAmount(const int amount)
        { mDownButtonScrollAmount = amount; }

        /**
         * Gets the amount to scroll in pixels when the left scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setLeftButtonScrollAmount
         */
        int getLeftButtonScrollAmount() const
        { return mLeftButtonScrollAmount; }

        /**
         * Gets the amount to scroll in pixels when the right scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setRightButtonScrollAmount
         */
        int getRightButtonScrollAmount() const
        { return mRightButtonScrollAmount; }

        /**
         * Gets the amount to scroll in pixels when the up scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setUpButtonScrollAmount
         */
        int getUpButtonScrollAmount() const
        { return mUpButtonScrollAmount; }

        /**
         * Gets the amount to scroll in pixels when the down scroll button is
         * pushed.
         *
         * @return The amount to scroll in pixels.
         * @see setDownButtonScrollAmount
         */
        int getDownButtonScrollAmount() const
        { return mDownButtonScrollAmount; }

        void showWidgetPart(Widget *const widget, Rect area) override final;

        Rect getChildrenArea() override final;

        Widget *getWidgetAt(int x, int y) override final;

        void setWidth(int width);

        void setHeight(int height);

        void setDimension(const Rect& dimension);

        void mouseWheelMovedUp(MouseEvent& event) override final;

        void mouseWheelMovedDown(MouseEvent& event) override final;

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

        /**
         * Checks the policies for the scroll bars.
         */
        void checkPolicies();

        void drawButton(Graphics *const graphics, const BUTTON_DIR dir);
        void calcButton(Graphics *const graphics, const BUTTON_DIR dir);
        void drawVBar(Graphics *const graphics) const;
        void drawHBar(Graphics *const graphics) const;
        void drawVMarker(Graphics *const graphics);
        void drawHMarker(Graphics *const graphics);

        void calcVBar(Graphics *const graphics);
        void calcHBar(Graphics *const graphics);
        void calcVMarker(Graphics *const graphics);
        void calcHMarker(Graphics *const graphics);

        Image *getImageByState(Rect &dim, const BUTTON_DIR dir);

        void updateCalcFlag(Graphics *const graphics);

        static int instances;
        static float mAlpha;
        static bool mShowButtons;
        static int mMarkerSize;
        static int mScrollbarSize;
        static ImageRect background;
        static ImageRect vMarker;
        static ImageRect vMarkerHi;
        static ImageRect vBackground;
        static ImageRect hBackground;
        static Image *buttons[4][2];

        ImageCollection *mVertexes;
        ImageCollection *mVertexes2;

        /**
         * Holds the horizontal scroll bar policy.
         */
        ScrollPolicy mHPolicy;

        /**
         * Holds the vertical scroll bar policy.
         */
        ScrollPolicy mVPolicy;

        /**
         * Holds the vertical scroll amount.
         */
        int mVScroll;

        /**
         * Holds the horizontal scroll amount.
         */
        int mHScroll;

        /**
         * Holds the width of the scroll bars.
         */
        int mScrollbarWidth;

        /**
         * Holds the up button scroll amount.
         */
        int mUpButtonScrollAmount;

        /**
         * Holds the down button scroll amount.
         */
        int mDownButtonScrollAmount;

        /**
         * Holds the left button scroll amount.
         */
        int mLeftButtonScrollAmount;

        /**
         * Holds the right button scroll amount.
         */
        int mRightButtonScrollAmount;

        /**
         * Holds the horizontal markers drag offset.
         */
        int mHorizontalMarkerDragOffset;

        /**
         * Holds the vertical markers drag offset.
         */
        int mVerticalMarkerDragOffset;

        int mX;
        int mY;
        int mClickX;
        int mClickY;
        int mXOffset;
        int mYOffset;
        int mDrawWidth;
        int mDrawHeight;

        /**
         * True if the vertical scroll bar is visible, false otherwise.
         */
        bool mVBarVisible;

        /**
         * True if the horizontal scroll bar is visible, false otherwise.
         */
        bool mHBarVisible;

        /**
         * True if the up button is pressed, false otherwise.
         */
        bool mUpButtonPressed;

        /**
         * True if the down button is pressed, false otherwise.
         */
        bool mDownButtonPressed;

        /**
         * True if the left button is pressed, false otherwise.
         */
        bool mLeftButtonPressed;

        /**
         * True if the right button is pressed, false otherwise.
         */
        bool mRightButtonPressed;

        /**
         * True if the vertical marked is dragged.
         */
        bool mIsVerticalMarkerDragged;

        /**
         * True if the horizontal marked is dragged.
         */
        bool mIsHorizontalMarkerDragged;

        /**
         * True if the scroll area should be opaque (that is
         * display its background), false otherwise.
         */
        bool mOpaque;

        bool mHasMouse;
};

#endif  // GUI_WIDGETS_SCROLLAREA_H
