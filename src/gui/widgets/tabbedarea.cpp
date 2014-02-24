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

#include "gui/widgets/tabbedarea.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/gui.h"

#include "gui/widgets/button.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tabs/tab.h"

#include "gui/base/widgets/container.hpp"

#include "debug.h"

TabbedArea::TabbedArea(const Widget2 *const widget) :
    ActionListener(),
    gcn::BasicContainer(widget),
    KeyListener(),
    MouseListener(),
    WidgetListener(),
    mSelectedTab(nullptr),
    mTabContainer(new gcn::Container(widget)),
    mWidgetContainer(new gcn::Container(widget)),
    mTabsToDelete(),
    mTabs(),
    mTabsWidth(0),
    mVisibleTabsWidth(0),
    mTabScrollIndex(0),
    mRightMargin(0),
    mOpaque(false),
    mEnableScrollButtons(false),
    mFollowDownScroll(false),
    mBlockSwitching(true),
    mResizeHeight(true)
{
    setFocusable(true);
    addKeyListener(this);
    addMouseListener(this);
    mTabContainer->setOpaque(false);

    add(mTabContainer);
    add(mWidgetContainer);

    mWidgetContainer->setOpaque(false);
    addWidgetListener(this);

    mArrowButton[0] = new Button(this, "<", "shift_left", this);
    mArrowButton[1] = new Button(this, ">", "shift_right", this);
}

void TabbedArea::postInit()
{
    widgetResized(Event(nullptr));
}

TabbedArea::~TabbedArea()
{
    if (gui)
        gui->removeDragged(this);

    // +++ virtual method calls
    remove(mTabContainer);
    remove(mWidgetContainer);

    delete mTabContainer;
    mTabContainer = nullptr;
    delete mWidgetContainer;
    mWidgetContainer = nullptr;

    for (size_t i = 0, sz = mTabsToDelete.size(); i < sz; i++)
    {
        delete mTabsToDelete[i];
        mTabsToDelete[i] = nullptr;
    }

    delete mArrowButton[0];
    mArrowButton[0] = nullptr;
    delete mArrowButton[1];
    mArrowButton[1] = nullptr;
}

void TabbedArea::enableScrollButtons(const bool enable)
{
    if (mEnableScrollButtons && !enable)
    {
        if (mArrowButton[0])
            remove(mArrowButton[0]);
        if (mArrowButton[1])
            remove(mArrowButton[1]);
    }
    else if (!mEnableScrollButtons && enable)
    {
        if (mArrowButton[0])
            add(mArrowButton[0]);
        if (mArrowButton[1])
            add(mArrowButton[1]);
    }
    mEnableScrollButtons = enable;
}

int TabbedArea::getNumberOfTabs() const
{
    return static_cast<int>(mTabs.size());
}

Tab *TabbedArea::getTab(const std::string &name) const
{
    TabContainer::const_iterator itr = mTabs.begin();
    const TabContainer::const_iterator itr_end = mTabs.end();
    while (itr != itr_end)
    {
        if ((*itr).first->getCaption() == name)
            return static_cast<Tab*>((*itr).first);

        ++itr;
    }
    return nullptr;
}

void TabbedArea::draw(Graphics *graphics)
{
    BLOCK_START("TabbedArea::draw")
    if (mTabs.empty())
    {
        BLOCK_END("TabbedArea::draw")
        return;
    }

    drawChildren(graphics);
    BLOCK_END("TabbedArea::draw")
}

Widget *TabbedArea::getWidget(const std::string &name) const
{
    TabContainer::const_iterator itr = mTabs.begin();
    const TabContainer::const_iterator itr_end = mTabs.end();
    while (itr != itr_end)
    {
        if ((*itr).first->getCaption() == name)
            return (*itr).second;

        ++itr;
    }

    return nullptr;
}

Widget *TabbedArea::getCurrentWidget() const
{
    const Tab *const tab = getSelectedTab();

    if (tab)
        return getWidget(tab->getCaption());
    else
        return nullptr;
}

void TabbedArea::addTab(Tab *const tab,
                        Widget *const widget)
{
    if (!tab || !widget)
        return;

    tab->setTabbedArea(this);
    tab->addActionListener(this);

    mTabContainer->add(tab);
    mTabs.push_back(std::pair<Tab*, Widget*>(tab, widget));

    if (!mSelectedTab)
        setSelectedTab(tab);

    adjustTabPositions();
    adjustSize();

    const int frameSize = 2 * mFrameSize;
    widget->setSize(getWidth() - frameSize,
        getHeight() - frameSize - mTabContainer->getHeight());

    updateTabsWidth();
    updateArrowEnableState();
}

void TabbedArea::adjustWidget(Widget *const widget) const
{
    const int frameSize = 2 * mFrameSize;
    widget->setSize(getWidth() - frameSize,
        getHeight() - frameSize - mTabContainer->getHeight());
}

void TabbedArea::addTab(const std::string &caption, Widget *const widget)
{
    Tab *const tab = new Tab(this);
    tab->setCaption(caption);
    mTabsToDelete.push_back(tab);

    addTab(tab, widget);
}

void TabbedArea::addTab(Image *const image, Widget *const widget)
{
    Tab *const tab = new Tab(this);
    tab->setImage(image);
    mTabsToDelete.push_back(tab);

    addTab(tab, widget);
}

bool TabbedArea::isTabSelected(const unsigned int index) const
{
    if (index >= mTabs.size())
        return false;

    return mSelectedTab == mTabs[index].first;
}

bool TabbedArea::isTabPresent(const Tab *const tab) const
{
    FOR_EACH (TabContainer::const_iterator, it, mTabs)
    {
        if ((*it).first == tab || (*it).second == tab)
            return true;
    }
    return false;
}

bool TabbedArea::isTabSelected(const Tab *const tab) const
{
    return mSelectedTab == tab;
}

void TabbedArea::setSelectedTabByIndex(const unsigned int index)
{
    if (index >= mTabs.size())
        return;

    setSelectedTab(mTabs[index].first);
}

void TabbedArea::removeTab(Tab *const tab)
{
    int tabIndexToBeSelected = -1;

    if (tab == mSelectedTab)
    {
        const int index = getSelectedTabIndex();
        const size_t sz = mTabs.size();
        if (index == static_cast<int>(sz) - 1 && sz == 1)
            tabIndexToBeSelected = -1;
        else
            tabIndexToBeSelected = index - 1;
    }

    for (TabContainer::iterator iter = mTabs.begin();
         iter != mTabs.end(); ++iter)
    {
        if (iter->first == tab)
        {
            mTabContainer->remove(tab);
            mTabs.erase(iter);
            break;
        }
    }

    for (std::vector<Tab*>::iterator iter2 = mTabsToDelete.begin();
         iter2 != mTabsToDelete.end(); ++iter2)
    {
        if (*iter2 == tab)
        {
            mTabsToDelete.erase(iter2);
            delete tab;
            break;
        }
    }

    const int tabsSize = static_cast<int>(mTabs.size());
    if (tabIndexToBeSelected >= tabsSize)
        tabIndexToBeSelected = tabsSize - 1;
    if (tabIndexToBeSelected < -1)
        tabIndexToBeSelected = -1;

    if (tabIndexToBeSelected == -1)
    {
        mSelectedTab = nullptr;
        mWidgetContainer->clear();
    }
    else
    {
        setSelectedTabByIndex(tabIndexToBeSelected);
    }

    adjustSize();
    updateTabsWidth();
    adjustTabPositions();
}

void TabbedArea::logic()
{
    BLOCK_START("TabbedArea::logic")
    logicChildren();
    BLOCK_END("TabbedArea::logic")
}

void TabbedArea::mousePressed(MouseEvent &mouseEvent)
{
    if (mouseEvent.isConsumed())
        return;

    if (mouseEvent.getButton() == MouseEvent::LEFT)
    {
        Widget *const widget = mTabContainer->getWidgetAt(
            mouseEvent.getX(), mouseEvent.getY());
        Tab *const tab = dynamic_cast<Tab *const>(widget);

        if (tab)
        {
            setSelectedTab(tab);
            requestFocus();
        }
    }
}

void TabbedArea::setSelectedTab(Tab *const tab)
{
    unsigned int i;
    for (i = 0; i < mTabs.size(); i++)
    {
        if (mTabs[i].first == mSelectedTab)
            mWidgetContainer->remove(mTabs[i].second);
    }

    for (i = 0; i < mTabs.size(); i++)
    {
        if (mTabs[i].first == tab)
        {
            mSelectedTab = tab;
            mWidgetContainer->add(mTabs[i].second);
        }
    }

    Tab *const newTab = dynamic_cast<Tab *const>(tab);

    if (newTab)
        newTab->setCurrent();

    widgetResized(Event(nullptr));
}

int TabbedArea::getSelectedTabIndex() const
{
    for (unsigned int i = 0, sz = static_cast<unsigned int>(mTabs.size());
         i < sz; i++)
    {
        if (mTabs[i].first == mSelectedTab)
            return i;
    }

    return -1;
}

void TabbedArea::setSelectedTabByName(const std::string &name)
{
    FOR_EACH (TabContainer::const_iterator, itr, mTabs)
    {
        if ((*itr).first && (*itr).first->getCaption() == name)
        {
            setSelectedTab((*itr).first);
            return;
        }
    }
}

void TabbedArea::widgetResized(const Event &event A_UNUSED)
{
    adjustSize();

    const int frameSize = 2 * mFrameSize;
    const int widgetFrameSize = 2 * mWidgetContainer->getFrameSize();
    const int w1 = mDimension.width;
    const int h1 = mDimension.height;
    const int width = w1 - frameSize - widgetFrameSize;
    const int height = h1 - frameSize
        - mWidgetContainer->getY() - widgetFrameSize;

    Widget *const w = getCurrentWidget();
    if (w)
    {
        ScrollArea *const scr = dynamic_cast<ScrollArea *const>(w);
        if (scr)
        {
            if (mFollowDownScroll && height != 0)
            {
                const Rect &rect = w->getDimension();
                if (rect.height != 0 && rect.height > height + 2)
                {
                    if (scr->getVerticalScrollAmount()
                        >= scr->getVerticalMaxScroll() - 2
                        && scr->getVerticalScrollAmount()
                        <= scr->getVerticalMaxScroll() + 2)
                    {
                        const int newScroll = scr->getVerticalScrollAmount()
                            + rect.height - height;
                        w->setSize(mWidgetContainer->getWidth() - frameSize,
                            mWidgetContainer->getHeight() - frameSize);
                        if (newScroll)
                            scr->setVerticalScrollAmount(newScroll);
                    }
                }
            }
        }
    }

    if (mArrowButton[1])
    {
        // Check whether there is room to show more tabs now.
        int innerWidth = w1 - 4 - mArrowButton[0]->getWidth()
            - mArrowButton[1]->getWidth() - mRightMargin;
        if (innerWidth < 0)
            innerWidth = 0;

        int newWidth = mVisibleTabsWidth;
        while (mTabScrollIndex && newWidth < innerWidth)
        {
            newWidth += mTabs[mTabScrollIndex - 1].first->getWidth();
            if (newWidth < innerWidth)
                --mTabScrollIndex;
        }

        if (mArrowButton[1])
        {
            // Move the right arrow to fit the windows content.
            newWidth = width - mArrowButton[1]->getWidth() - mRightMargin;
            if (newWidth < 0)
                newWidth = 0;
            mArrowButton[1]->setPosition(newWidth, 0);
        }
    }

    updateArrowEnableState();
    adjustTabPositions();
}

void TabbedArea::updateTabsWidth()
{
    mTabsWidth = 0;
    FOR_EACH (TabContainer::const_iterator, itr, mTabs)
    {
        if ((*itr).first)
            mTabsWidth += (*itr).first->getWidth();
    }
    updateVisibleTabsWidth();
}

void TabbedArea::updateVisibleTabsWidth()
{
    mVisibleTabsWidth = 0;
    for (size_t i = mTabScrollIndex, sz = mTabs.size(); i < sz; ++i)
    {
        if (mTabs[i].first)
            mVisibleTabsWidth += mTabs[i].first->getWidth();
    }
}

void TabbedArea::adjustSize()
{
    int maxTabHeight = 0;

    const int width = mDimension.width;
    const int height = mDimension.height;

    for (size_t i = 0, sz = mTabs.size(); i < sz; i++)
    {
        if (mTabs[i].first->getHeight() > maxTabHeight)
            maxTabHeight = mTabs[i].first->getHeight();
    }

    mTabContainer->setSize(width - mRightMargin, maxTabHeight);

    mWidgetContainer->setPosition(0, maxTabHeight);
    mWidgetContainer->setSize(width, height - maxTabHeight);
    Widget *const w = getCurrentWidget();
    if (w)
    {
        const int wFrameSize = w->getFrameSize();
        const int frame2 = 2 * wFrameSize;

        w->setPosition(wFrameSize, wFrameSize);
        if (mResizeHeight)
        {
            w->setSize(mWidgetContainer->getWidth() - frame2,
                mWidgetContainer->getHeight() - frame2);
        }
        else
        {
            w->setSize(mWidgetContainer->getWidth() - frame2,
                w->getHeight());
        }
    }
}

void TabbedArea::adjustTabPositions()
{
    int maxTabHeight = 0;
    const size_t sz = mTabs.size();
    for (size_t i = 0; i < sz; ++i)
    {
        const Tab *const tab = mTabs[i].first;
        if (tab && tab->getHeight() > maxTabHeight)
            maxTabHeight = tab->getHeight();
    }

    int x = (mEnableScrollButtons && mArrowButton[0]->isVisible())
        ? mArrowButton[0]->getWidth() : 0;
    for (size_t i = mTabScrollIndex; i < sz; ++i)
    {
        Tab *const tab = mTabs[i].first;
        if (!tab)
            continue;
        tab->setPosition(x, maxTabHeight - tab->getHeight());
        x += tab->getWidth();
    }

    // If the tabs are scrolled, we hide them away.
    if (mTabScrollIndex > 0)
    {
        x = 0;
        for (unsigned i = 0; i < mTabScrollIndex; ++i)
        {
            Tab *const tab = mTabs[i].first;
            if (tab)
            {
                x -= tab->getWidth();
                tab->setPosition(x, maxTabHeight - tab->getHeight());
            }
        }
    }
}

void TabbedArea::action(const ActionEvent& actionEvent)
{
    Widget *const source = actionEvent.getSource();
    Tab *const tab = dynamic_cast<Tab *const>(source);

    if (tab)
    {
        setSelectedTab(tab);
    }
    else
    {
        const std::string &eventId = actionEvent.getId();
        if (eventId == "shift_left")
        {
            if (mTabScrollIndex)
                --mTabScrollIndex;
        }
        else if (eventId == "shift_right")
        {
            if (mTabScrollIndex < mTabs.size() - 1)
                ++mTabScrollIndex;
        }
        adjustTabPositions();
        updateArrowEnableState();
    }
}

void TabbedArea::updateArrowEnableState()
{
    updateTabsWidth();
    if (!mArrowButton[0] || !mArrowButton[1])
        return;

    const int width = mDimension.width;
    if (mTabsWidth > width - 4
        - mArrowButton[0]->getWidth()
        - mArrowButton[1]->getWidth() - mRightMargin)
    {
        mArrowButton[0]->setVisible(true);
        mArrowButton[1]->setVisible(true);
    }
    else
    {
        mArrowButton[0]->setVisible(false);
        mArrowButton[1]->setVisible(false);
        mTabScrollIndex = 0;
    }

    // Left arrow consistency check
    if (!mTabScrollIndex)
        mArrowButton[0]->setEnabled(false);
    else
        mArrowButton[0]->setEnabled(true);

    // Right arrow consistency check
    if (mVisibleTabsWidth < width - 4
        - mArrowButton[0]->getWidth()
        - mArrowButton[1]->getWidth() - mRightMargin)
    {
        mArrowButton[1]->setEnabled(false);
    }
    else
    {
        mArrowButton[1]->setEnabled(true);
    }
}

Tab *TabbedArea::getTabByIndex(const int index) const
{
    if (index < 0 || index >= static_cast<int>(mTabs.size()))
        return nullptr;
    return static_cast<Tab*>(mTabs[index].first);
}

Widget *TabbedArea::getWidgetByIndex(const int index) const
{
    if (index < 0 || index >= static_cast<int>(mTabs.size()))
        return nullptr;
    return mTabs[index].second;
}

void TabbedArea::removeAll(const bool del)
{
    if (getSelectedTabIndex() != -1)
    {
        setSelectedTabByIndex(static_cast<unsigned int>(0));
    }
    while (getNumberOfTabs() > 0)
    {
        const int idx = getNumberOfTabs() - 1;
        Tab *tab = mTabs[idx].first;
        Widget *widget = mTabs[idx].second;
        removeTab(tab);
        if (del)
        {
            delete tab;
            delete widget;
        }
    }
}

void TabbedArea::setWidth(int width)
{
    Widget::setWidth(width);
    adjustSize();
}

void TabbedArea::setHeight(int height)
{
    Widget::setHeight(height);
    adjustSize();
}

void TabbedArea::setSize(int width, int height)
{
    Widget::setSize(width, height);
    adjustSize();
}

void TabbedArea::setDimension(const Rect &dimension)
{
    Widget::setDimension(dimension);
    adjustSize();
}

void TabbedArea::keyPressed(KeyEvent& keyEvent)
{
    if (mBlockSwitching || keyEvent.isConsumed() || !isFocused())
        return;

    const int actionId = keyEvent.getActionId();

    if (actionId == Input::KEY_GUI_LEFT)
    {
        int index = getSelectedTabIndex();
        index--;

        if (index < 0)
            return;
        else
            setSelectedTab(mTabs[index].first);

        keyEvent.consume();
    }
    else if (actionId == Input::KEY_GUI_RIGHT)
    {
        int index = getSelectedTabIndex();
        index++;

        if (index >= static_cast<int>(mTabs.size()))
            return;
        else
            setSelectedTab(mTabs[index].first);

        keyEvent.consume();
    }
}

void TabbedArea::death(const Event &event)
{
    Tab *const tab = dynamic_cast<Tab*>(event.getSource());

    if (tab)
        removeTab(tab);
    else
        gcn::BasicContainer::death(event);
}

void TabbedArea::selectNextTab()
{
    int tab = getSelectedTabIndex();
    tab++;
    if (tab == static_cast<int>(mTabs.size()))
        tab = 0;
    setSelectedTab(mTabs[tab].first);
}

void TabbedArea::selectPrevTab()
{
    int tab = getSelectedTabIndex();

    if (tab == 0)
        tab = static_cast<int>(mTabs.size());
    tab--;
    setSelectedTab(mTabs[tab].first);
}
