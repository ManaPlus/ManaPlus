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

#ifndef GUI_WIDGETS_TABBEDAREA_H
#define GUI_WIDGETS_TABBEDAREA_H

#include "listeners/keylistener.h"
#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

#include "gui/base/widgets/container.hpp"

#include "listeners/actionlistener.h"

class Button;
class Image;
class Tab;

/**
 * A tabbed area, the same as the guichan tabbed area in 0.8, but extended
 */
class TabbedArea final : public ActionListener,
                         public gcn::BasicContainer,
                         public KeyListener,
                         public MouseListener,
                         public WidgetListener
{
    public:
        /**
         * Constructor.
         */
        explicit TabbedArea(const Widget2 *const widget);

        A_DELETE_COPY(TabbedArea)

        ~TabbedArea();

        void postInit();

        /**
         * Draw the tabbed area.
         */
        void draw(Graphics *graphics) override final;

        /**
         * Return how many tabs have been created.
         *
         * @todo Remove this method when upgrading to Guichan 0.9.0
         */
        int getNumberOfTabs() const A_WARN_UNUSED;

        /**
         * Return tab with specified name as caption.
         */
        Tab *getTab(const std::string &name) const A_WARN_UNUSED;

        Tab *getTabByIndex(const int index) const A_WARN_UNUSED;

        Widget *getWidgetByIndex(const int index) const A_WARN_UNUSED;

        /**
         * Returns the widget with the tab that has specified caption
         */
        Widget *getWidget(const std::string &name) const A_WARN_UNUSED;

        /**
         * Returns the widget for the current tab
         */
        Widget *getCurrentWidget() const A_WARN_UNUSED;

        /**
         * Add a tab. Overridden since it needs to size the widget.
         *
         * @param tab The tab widget for the tab.
         * @param widget The widget to view when the tab is selected.
         */
        void addTab(Tab *const tab, Widget *const widget);

        void addTab(const std::string &caption, Widget *const widget);

        void addTab(Image *const image, Widget *const widget);

        bool isTabSelected(const unsigned int index) const A_WARN_UNUSED;

        bool isTabSelected(const Tab *const tab) const A_WARN_UNUSED;

        bool isTabPresent(const Tab *const tab) const A_WARN_UNUSED;

        /**
         * Overload the remove tab function as it's broken in guichan 0.8.
         */
        void removeTab(Tab *const tab);

        void removeAll(const bool del = true);

        /**
         * Overload the logic function since it's broken in guichan 0.8.
         */
        void logic() override final;

        int getContainerHeight() const A_WARN_UNUSED
        { return mWidgetContainer->getHeight(); }

        void setSelectedTab(Tab *const tab);

        void setSelectedTabByIndex(const unsigned int index);

        int getSelectedTabIndex() const A_WARN_UNUSED;

        Tab* getSelectedTab() const A_WARN_UNUSED
        { return mSelectedTab; }

        void setOpaque(const bool opaque)
        { mOpaque = opaque; }

        bool isOpaque() const A_WARN_UNUSED
        { return mOpaque; }

        void adjustSize();

        void setSelectedTabByName(const std::string &name);

        void widgetResized(const Event &event) override final;

/*
        void moveLeft(Tab *tab);

        void moveRight(Tab *tab);
*/
        void adjustTabPositions();

        void action(const ActionEvent& actionEvent) override final;

        // Inherited from MouseListener

        void mousePressed(MouseEvent &mouseEvent) override final;

        void enableScrollButtons(const bool enable);

        void setRightMargin(const int n)
        { mRightMargin = n; }

        int getRightMargin() const A_WARN_UNUSED
        { return mRightMargin; }

        void setFollowDownScroll(const bool n)
        { mFollowDownScroll = n; }

        bool getFollowDownScroll() const A_WARN_UNUSED
        { return mFollowDownScroll; }

        void keyPressed(KeyEvent& keyEvent) override final;

        void setBlockSwitching(const bool b)
        { mBlockSwitching = b; }

        void setWidth(int width);

        void setHeight(int height);

        void setSize(int width, int height);

        void setDimension(const Rect &dimension);

        void death(const Event &event);

        void setResizeHeight(bool b)
        { mResizeHeight = b; }

        void adjustWidget(Widget *const widget) const;

        void selectNextTab();

        void selectPrevTab();

    private:
        typedef std::vector <std::pair<Tab*, Widget*> > TabContainer;

        /** The tab arrows */
        Button *mArrowButton[2];

        /** Check whether the arrow should be clickable */
        void updateArrowEnableState();

        /**
         * Update the overall width of all tab. Used to know whether the arrows
         * have to be drawn or not.
         */
        void updateTabsWidth();

        Tab* mSelectedTab;
        gcn::Container* mTabContainer;
        gcn::Container* mWidgetContainer;
        std::vector<Tab*> mTabsToDelete;
        TabContainer mTabs;

        /**
         * The overall width of all tab.
         */
        int mTabsWidth;

        /**
         * Update the overall width of visible tab. Used to know whether
         * the arrows have to be enable or not.
         */
        void updateVisibleTabsWidth();

        /**
         * The overall width of visible tab.
         */
        int mVisibleTabsWidth;

        /**
         * The tab scroll index. When scrolling with the arrows, the tabs
         * must be displayed according to the current index.
         * So the first tab displayed may not be the first in the list.
         * @note the index must start at 0.
         */
        unsigned int mTabScrollIndex;
        int mRightMargin;

        bool mOpaque;
        bool mEnableScrollButtons;
        bool mFollowDownScroll;
        bool mBlockSwitching;
        bool mResizeHeight;
};

#endif  // GUI_WIDGETS_TABBEDAREA_H
