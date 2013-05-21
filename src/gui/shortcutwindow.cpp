/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "debug.h"

static const int SCROLL_PADDING = 0;

int ShortcutWindow::mBoxesWidth = 0;

class ShortcutTab final : public Tab
{
    public:
        ShortcutTab(const Widget2 *const widget,
                    std::string name, ShortcutContainer *const content) :
            Tab(widget),
            mContent(content)
        {
            setCaption(name);
        }

        A_DELETE_COPY(ShortcutTab)

        ShortcutContainer* mContent;
};

ShortcutWindow::ShortcutWindow(const std::string &title,
                               ShortcutContainer *const content,
                               const std::string &skinFile,
                               int width, int height) :
    Window("Window", false, nullptr, skinFile),
    mItems(content),
    mScrollArea(new ScrollArea(mItems, false)),
    mTabs(nullptr),
    mPages()
{
    setWindowName(title);
    setTitleBarHeight(getPadding() + getTitlePadding());

    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);

    mDragOffsetX = 0;
    mDragOffsetY = 0;

    content->setWidget2(this);
    setupWindow->registerWindowForReset(this);

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

    mScrollArea->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    place(0, 0, mScrollArea, 5, 5).setPadding(0);

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);
    layout.setMargin(0);

    loadWindowState();
    enableVisibleSound(true);
}

ShortcutWindow::ShortcutWindow(const std::string &title,
                               const std::string &skinFile,
                               const int width, const int height) :
    Window("Window", false, nullptr, skinFile),
    mItems(nullptr),
    mScrollArea(nullptr),
    mTabs(new TabbedArea(this)),
    mPages()
{
    setWindowName(title);
    setTitleBarHeight(getPadding() + getTitlePadding());
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);

    mDragOffsetX = 0;
    mDragOffsetY = 0;

    setupWindow->registerWindowForReset(this);

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
    enableVisibleSound(true);
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

void ShortcutWindow::addTab(const std::string &name,
                            ShortcutContainer *const content)
{
    ScrollArea *const scroll = new ScrollArea(content, false);
    scroll->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    scroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    content->setWidget2(this);
    Tab *const tab = new ShortcutTab(this, name, content);
    mTabs->addTab(tab, scroll);
    mPages.push_back(content);
}

int ShortcutWindow::getTabIndex() const
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
        ScrollArea *const scroll = static_cast<ScrollArea *const>(
            mTabs->getCurrentWidget());
        if (scroll)
        {
            ShortcutContainer *const content = static_cast<ShortcutContainer*>(
                scroll->getContent());

            if (content)
                content->widgetHidden(event);
        }
    }
}

void ShortcutWindow::mousePressed(gcn::MouseEvent &event)
{
    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        mDragOffsetX = event.getX();
        mDragOffsetY = event.getY();
    }
}

void ShortcutWindow::mouseDragged(gcn::MouseEvent &event)
{
    Window::mouseDragged(event);

    if (event.isConsumed())
        return;

    if (canMove() && isMovable() && mMoved)
    {
        int newX = std::max(0, getX() + event.getX() - mDragOffsetX);
        int newY = std::max(0, getY() + event.getY() - mDragOffsetY);
        newX = std::min(mainGraphics->mWidth - getWidth(), newX);
        newY = std::min(mainGraphics->mHeight - getHeight(), newY);
        setPosition(newX, newY);
    }
}

void ShortcutWindow::widgetMoved(const gcn::Event& event)
{
    Window::widgetMoved(event);
    if (mItems)
        mItems->setRedraw(true);
    FOR_EACH (std::vector<ShortcutContainer*>::iterator, it, mPages)
        (*it)->setRedraw(true);
}
