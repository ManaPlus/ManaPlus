/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "gui/widgets/tabbedarea.h"

#include "keydata.h"
#include "keyevent.h"

#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tab.h"

#include "logger.h"

#include <guichan/widgets/container.hpp>

#include "debug.h"

TabbedArea::TabbedArea() :
    gcn::TabbedArea(),
    mTabsWidth(0),
    mVisibleTabsWidth(0),
    mTabScrollIndex(0),
    mEnableScrollButtons(false),
    mRightMargin(0),
    mFollowDownScroll(false),
    mBlockSwitching(true)
{
    mWidgetContainer->setOpaque(false);
    addWidgetListener(this);

    mArrowButton[0] = new Button("<", "shift_left", this);
    mArrowButton[1] = new Button(">", "shift_right", this);

    widgetResized(nullptr);
}

TabbedArea::~TabbedArea()
{
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
            add(mArrowButton[0]);
        if (mArrowButton[1])
            add(mArrowButton[1]);
    }
    else if (!mEnableScrollButtons && enable)
    {
        if (mArrowButton[0])
            add(mArrowButton[0]);
        if (mArrowButton[1])
            add(mArrowButton[1]);
    }
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

void TabbedArea::draw(gcn::Graphics *graphics)
{
    if (mTabs.empty())
        return;

    drawChildren(graphics);
}

gcn::Widget *TabbedArea::getWidget(const std::string &name) const
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

gcn::Widget *TabbedArea::getCurrentWidget()
{
    const gcn::Tab *const tab = getSelectedTab();

    if (tab)
        return getWidget(tab->getCaption());
    else
        return nullptr;
}

void TabbedArea::addTab(gcn::Tab* tab, gcn::Widget* widget)
{
    if (!tab || !widget)
        return;

    gcn::TabbedArea::addTab(tab, widget);

    const int width = getWidth() - 2 * getFrameSize();
    const int height = getHeight() - 2 * getFrameSize()
        - mTabContainer->getHeight();
    widget->setSize(width, height);

    updateTabsWidth();
    updateArrowEnableState();
}

void TabbedArea::addTab(const std::string &caption, gcn::Widget *const widget)
{
    Tab *const tab = new Tab;
    tab->setCaption(caption);
    mTabsToDelete.push_back(tab);

    addTab(tab, widget);
}

void TabbedArea::removeTab(gcn::Tab *tab)
{
    int tabIndexToBeSelected = -1;

    if (tab == mSelectedTab)
    {
        const int index = getSelectedTabIndex();

        if (index == static_cast<int>(mTabs.size()) - 1 && mTabs.size() == 1)
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

    for (std::vector<gcn::Tab*>::iterator iter2 = mTabsToDelete.begin();
         iter2 != mTabsToDelete.end(); ++iter2)
    {
        if (*iter2 == tab)
        {
            mTabsToDelete.erase(iter2);
            delete tab;
            break;
        }
    }

    if (tabIndexToBeSelected >= static_cast<signed>(mTabs.size()))
        tabIndexToBeSelected = static_cast<int>(mTabs.size()) - 1;
    if (tabIndexToBeSelected < -1)
        tabIndexToBeSelected = -1;

    if (tabIndexToBeSelected == -1)
    {
        mSelectedTab = nullptr;
        mWidgetContainer->clear();
    }
    else
    {
        setSelectedTabByPos(tabIndexToBeSelected);
    }

    adjustSize();
    updateTabsWidth();
    adjustTabPositions();
}

void TabbedArea::logic()
{
    logicChildren();
}

void TabbedArea::mousePressed(gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.isConsumed())
        return;

    if (mouseEvent.getButton() == gcn::MouseEvent::LEFT)
    {
        gcn::Widget *const widget = mTabContainer->getWidgetAt(
            mouseEvent.getX(), mouseEvent.getY());
        gcn::Tab *const tab = dynamic_cast<gcn::Tab *const>(widget);

        if (tab)
        {
            setSelectedTab(tab);
            requestFocus();
        }
    }
}

void TabbedArea::setSelectedTab(gcn::Tab *tab)
{
    gcn::TabbedArea::setSelectedTab(tab);

    Tab *const newTab = dynamic_cast<Tab *const>(tab);

    if (newTab)
        newTab->setCurrent();

    widgetResized(nullptr);
}

void TabbedArea::setSelectedTabByName(const std::string &name)
{
    for (TabContainer::const_iterator itr = mTabs.begin(),
         itr_end = mTabs.end(); itr != itr_end; ++itr)
    {
        if ((*itr).first && (*itr).first->getCaption() == name)
        {
            setSelectedTab((*itr).first);
            return;
        }
    }
}

void TabbedArea::setSelectedTabByPos(int tab)
{
    gcn::TabbedArea::setSelectedTab(tab);
}

void TabbedArea::widgetResized(const gcn::Event &event A_UNUSED)
{
    const int width = getWidth() - 2 * getFrameSize()
        - 2 * mWidgetContainer->getFrameSize();
    const int height = getHeight() - 2 * getFrameSize()
        - mWidgetContainer->getY() - 2 * mWidgetContainer->getFrameSize();
    mWidgetContainer->setSize(width, height);

    gcn::Widget *const w = getCurrentWidget();
    if (w)
    {
        int newScroll = 0;
        ScrollArea* scr = nullptr;
        if (mFollowDownScroll && height != 0)
        {
            const gcn::Rectangle rect = w->getDimension();
            if (rect.height != 0 && rect.height > height + 2)
            {
                scr = dynamic_cast<ScrollArea*>(w);
                if (scr && scr->getVerticalScrollAmount()
                    >= scr->getVerticalMaxScroll() - 2
                    && scr->getVerticalScrollAmount()
                    <= scr->getVerticalMaxScroll() + 2)
                {
                    newScroll = scr->getVerticalScrollAmount()
                        + rect.height - height;
                }
            }
        }
        w->setSize(width, height);
        if (scr && newScroll)
            scr->setVerticalScrollAmount(newScroll);
    }

    if (mArrowButton[1])
    {
        // Check whether there is room to show more tabs now.
        int innerWidth = getWidth() - 4 - mArrowButton[0]->getWidth()
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
    for (TabContainer::const_iterator itr = mTabs.begin(),
         itr_end = mTabs.end(); itr != itr_end; ++itr)
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

void TabbedArea::adjustTabPositions()
{
    int maxTabHeight = 0;
    size_t sz = mTabs.size();
    for (size_t i = 0; i < sz; ++i)
    {
        if (mTabs[i].first && mTabs[i].first->getHeight() > maxTabHeight)
            maxTabHeight = mTabs[i].first->getHeight();
    }

    int x = mArrowButton[0]->isVisible() ? mArrowButton[0]->getWidth() : 0;
    for (size_t i = mTabScrollIndex; i < sz; ++i)
    {
        gcn::Tab *const tab = mTabs[i].first;
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
            gcn::Tab *const tab = mTabs[i].first;
            if (tab)
            {
                x -= tab->getWidth();
                tab->setPosition(x, maxTabHeight - tab->getHeight());
            }
        }
    }
}

void TabbedArea::action(const gcn::ActionEvent& actionEvent)
{
    Widget *const source = actionEvent.getSource();
    Tab *const tab = dynamic_cast<Tab *const>(source);

    if (tab)
    {
        setSelectedTab(tab);
    }
    else
    {
        if (actionEvent.getId() == "shift_left")
        {
            if (mTabScrollIndex)
                --mTabScrollIndex;
        }
        else if (actionEvent.getId() == "shift_right")
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

    if (mTabsWidth > getWidth() - 4
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
    if (mVisibleTabsWidth < getWidth() - 4
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

gcn::Widget *TabbedArea::getWidgetByIndex(const int index) const
{
    if (index < 0 || index >= static_cast<int>(mTabs.size()))
        return nullptr;
    return mTabs[index].second;
}

void TabbedArea::removeAll()
{
    if (getSelectedTabIndex() != -1)
    {
        setSelectedTabByPos(static_cast<unsigned int>(0));
    }
    while (getNumberOfTabs() > 0)
    {
        const int idx = getNumberOfTabs() - 1;
        gcn::Tab *tab = mTabs[idx].first;
        Widget *widget = mTabs[idx].second;
        removeTab(tab);
        delete tab;
        delete widget;
    }
}

void TabbedArea::keyPressed(gcn::KeyEvent& keyEvent)
{
    if (mBlockSwitching || keyEvent.isConsumed() || !isFocused())
        return;

    const int actionId = static_cast<KeyEvent*>(&keyEvent)->getActionId();

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
