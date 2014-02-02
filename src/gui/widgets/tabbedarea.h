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

#ifndef GUI_WIDGETS_TABBEDAREA_H
#define GUI_WIDGETS_TABBEDAREA_H

#include "gui/widgets/widget2.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/mouselistener.hpp>
#include <guichan/widgetlistener.hpp>
#include <guichan/widgets/container.hpp>

class Button;
class Image;
class Tab;

/**
 * A tabbed area, the same as the guichan tabbed area in 0.8, but extended
 */
class TabbedArea final : public Widget2,
                         public gcn::ActionListener,
                         public gcn::BasicContainer,
                         public gcn::KeyListener,
                         public gcn::MouseListener,
                         public gcn::WidgetListener
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
        void draw(gcn::Graphics *graphics) override final;

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

        gcn::Widget *getWidgetByIndex(const int index) const A_WARN_UNUSED;

        /**
         * Returns the widget with the tab that has specified caption
         */
        gcn::Widget *getWidget(const std::string &name) const A_WARN_UNUSED;

        /**
         * Returns the widget for the current tab
         */
        gcn::Widget *getCurrentWidget() const A_WARN_UNUSED;

        /**
         * Add a tab. Overridden since it needs to size the widget.
         *
         * @param tab The tab widget for the tab.
         * @param widget The widget to view when the tab is selected.
         */
        void addTab(Tab *const tab, gcn::Widget *const widget);

        void addTab(const std::string &caption, gcn::Widget *const widget);

        void addTab(Image *const image, gcn::Widget *const widget);

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

        void widgetResized(const gcn::Event &event) override final;

/*
        void moveLeft(Tab *tab);

        void moveRight(Tab *tab);
*/
        void adjustTabPositions();

        void action(const gcn::ActionEvent& actionEvent) override final;

        // Inherited from MouseListener

        void mousePressed(gcn::MouseEvent &mouseEvent) override final;

        void enableScrollButtons(const bool enable);

        void setRightMargin(const int n)
        { mRightMargin = n; }

        int getRightMargin() const A_WARN_UNUSED
        { return mRightMargin; }

        void setFollowDownScroll(const bool n)
        { mFollowDownScroll = n; }

        bool getFollowDownScroll() const A_WARN_UNUSED
        { return mFollowDownScroll; }

        void keyPressed(gcn::KeyEvent& keyEvent) override final;

        void setBlockSwitching(const bool b)
        { mBlockSwitching = b; }

        void setWidth(int width);

        void setHeight(int height);

        void setSize(int width, int height);

        void setDimension(const gcn::Rectangle &dimension);

        void death(const gcn::Event &event);

        void setResizeHeight(bool b)
        { mResizeHeight = b; }

        void adjustWidget(gcn::Widget *const widget) const;

        void selectNextTab();

        void selectPrevTab();

    private:
        typedef std::vector <std::pair<Tab*, gcn::Widget*> > TabContainer;

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
