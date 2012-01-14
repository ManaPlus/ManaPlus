/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/shortcutwindow.h"

#include "configuration.h"

#include "gui/setup.h"

#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shortcutcontainer.h"
#include "gui/widgets/tab.h"
#include "gui/widgets/tabbedarea.h"

#include "debug.h"

static const int SCROLL_PADDING = 0;

int ShortcutWindow::mBoxesWidth = 0;

class ShortcutTab : public Tab
{
    public:
        ShortcutTab(std::string name, ShortcutContainer* content)
        {
            setCaption(name);
            mContent = content;
        }

        ShortcutContainer* mContent;
};

ShortcutWindow::ShortcutWindow(const std::string &title,
                               ShortcutContainer *content,
                               std::string skinFile,
                               int width, int height) :
    Window("Window", false, nullptr, skinFile)
{
    setWindowName(title);
    // no title presented, title bar is padding so window can be moved.
    gcn::Window::setTitleBarHeight(gcn::Window::getPadding() + 1);
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);

    setupWindow->registerWindowForReset(this);

    mTabs = nullptr;
    mItems = content;

    const int border = SCROLL_PADDING * 2 + getPadding() * 2;
    setMinWidth(mItems->getBoxWidth() + border);
    setMinHeight(mItems->getBoxHeight() + border);
    setMaxWidth(mItems->getBoxWidth() * mItems->getMaxItems() + border);
    setMaxHeight(mItems->getBoxHeight() * mItems->getMaxItems() + border);

    if (width == 0)
        width = mItems->getBoxWidth() + border;
    if (height == 0)
        height = (mItems->getBoxHeight() * mItems->getMaxItems()) + border;

    setDefaultSize(width, height, ImageRect::LOWER_RIGHT);

    mBoxesWidth += mItems->getBoxWidth() + border;

    mScrollArea = new ScrollArea(mItems);
    mScrollArea->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mScrollArea->setOpaque(false);

    place(0, 0, mScrollArea, 5, 5).setPadding(0);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
    layout.setMargin(0);

    loadWindowState();
}

ShortcutWindow::ShortcutWindow(const std::string &title, std::string skinFile,
                               int width, int height) :
    Window("Window", false, nullptr, skinFile)
{
    setWindowName(title);
    // no title presented, title bar is padding so window can be moved.
    gcn::Window::setTitleBarHeight(gcn::Window::getPadding() + 1);
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);

    setupWindow->registerWindowForReset(this);

    mTabs = new TabbedArea;

    mItems = nullptr;

    const int border = SCROLL_PADDING * 2 + getPadding() * 2;

    if (width && height)
        setDefaultSize(width, height, ImageRect::LOWER_RIGHT);

    setMinWidth(32 + border);
    setMinHeight(32 + border);

    place(0, 0, mTabs, 5, 5);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
    layout.setMargin(0);

    loadWindowState();
}

ShortcutWindow::~ShortcutWindow()
{
    if (mTabs)
        mTabs->removeAll();
    delete mTabs;
    mTabs = nullptr;
    delete mItems;
    mItems = nullptr;
}

void ShortcutWindow::addTab(std::string name, ShortcutContainer *content)
{
    ScrollArea *scroll = new ScrollArea(content);
    scroll->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    scroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    scroll->setOpaque(false);
    Tab *tab = new ShortcutTab(name, content);
    mTabs->addTab(tab, scroll);
}

int ShortcutWindow::getTabIndex()
{
    if (!mTabs)
        return 0;
    return mTabs->getSelectedTabIndex();
}

void ShortcutWindow::widgetHidden(const gcn::Event &event)
{
    if (mItems)
        mItems->widgetHidden(event);
    if (mTabs)
    {
        ScrollArea *scroll = static_cast<ScrollArea*>(
            mTabs->getCurrentWidget());
        if (scroll)
        {
            ShortcutContainer *content = static_cast<ShortcutContainer*>(
                scroll->getContent());

            if (content)
                content->widgetHidden(event);
        }
    }
}
