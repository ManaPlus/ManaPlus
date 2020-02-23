/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/windows/shortcutwindow.h"

#include "enums/gui/layouttype.h"

#include "gui/windows/setupwindow.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/shortcutcontainer.h"

#include "gui/widgets/tabs/shortcuttab.h"

#include "utils/delete2.h"
#include "utils/foreach.h"

#include "debug.h"

ShortcutWindow *dropShortcutWindow = nullptr;
ShortcutWindow *emoteShortcutWindow = nullptr;
ShortcutWindow *itemShortcutWindow = nullptr;
ShortcutWindow *spellShortcutWindow = nullptr;
static const int SCROLL_PADDING = 0;

int ShortcutWindow::mBoxesWidth = 0;

ShortcutWindow::ShortcutWindow(const std::string &restrict title,
                               ShortcutContainer *restrict const content,
                               const std::string &restrict skinFile,
                               int width, int height) :
    Window("Window", Modal_false, nullptr, skinFile),
    mItems(content),
    mScrollArea(new ScrollArea(this, mItems, Opaque_false, std::string())),
    mTabs(nullptr),
    mPages(),
    mButtonIndex(0)
{
    setWindowName(title);
    setTitleBarHeight(getPadding() + getTitlePadding());

    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);
    setAllowClose(true);

    mDragOffsetX = 0;
    mDragOffsetY = 0;

    if (content != nullptr)
        content->setSkin(this, mSkin);
    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    setMinWidth(32);
    setMinHeight(32);
    if (mItems != nullptr)
    {
        const int border = SCROLL_PADDING * 2 + getPadding() * 2;
        const int bw = mItems->getBoxWidth();
        const int bh = mItems->getBoxHeight();
        const int maxItems = mItems->getMaxItems();
        setMaxWidth(bw * maxItems + border);
        setMaxHeight(bh * maxItems + border);

        if (width == 0)
            width = bw + border;
        if (height == 0)
            height = bh * maxItems + border;

        setDefaultSize(width, height, ImagePosition::LOWER_RIGHT, 0, 0);

        mBoxesWidth += bw + border;
    }

    mScrollArea->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    mScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    place(0, 0, mScrollArea, 5, 5).setPadding(0);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);
    layout.setMargin(0);

    loadWindowState();
    enableVisibleSound(true);
}

ShortcutWindow::ShortcutWindow(const std::string &restrict title,
                               const std::string &restrict skinFile,
                               const int width, const int height) :
    Window("Window", Modal_false, nullptr, skinFile),
    mItems(nullptr),
    mScrollArea(nullptr),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    mPages(),
    mButtonIndex(0)
{
    setWindowName(title);
    setTitleBarHeight(getPadding() + getTitlePadding());
    setShowTitle(false);
    setResizable(true);
    setDefaultVisible(false);
    setSaveVisible(true);
    setAllowClose(true);

    mDragOffsetX = 0;
    mDragOffsetY = 0;

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    if ((width != 0) && (height != 0))
        setDefaultSize(width, height, ImagePosition::LOWER_RIGHT, 0, 0);

    setMinWidth(32);
    setMinHeight(32);

    place(0, 0, mTabs, 5, 5);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);
    layout.setMargin(0);

    loadWindowState();
    enableVisibleSound(true);
}

ShortcutWindow::~ShortcutWindow()
{
    if (mTabs != nullptr)
        mTabs->removeAll(true);
    delete2(mTabs)
    delete2(mItems)
}

void ShortcutWindow::addButton(const std::string &text,
                               const std::string &eventName,
                               ActionListener *const listener)
{
    place(mButtonIndex++,
        5,
        new Button(this, text, eventName, BUTTON_SKIN, listener),
        1,
        1);
    Window::widgetResized(Event(nullptr));
}

void ShortcutWindow::addTab(const std::string &name,
                            ShortcutContainer *const content)
{
    if ((content == nullptr) || (mTabs == nullptr))
        return;
    ScrollArea *const scroll = new ScrollArea(this,
        content,
        Opaque_false,
        std::string());
    scroll->setPosition(SCROLL_PADDING, SCROLL_PADDING);
    scroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    content->setSkin(this, mSkin);
    Tab *const tab = new ShortcutTab(this, name, content);
    mTabs->addTab(tab, scroll);
    mPages.push_back(content);
}

int ShortcutWindow::getTabIndex() const
{
    if (mTabs == nullptr)
        return 0;
    return mTabs->getSelectedTabIndex();
}

void ShortcutWindow::widgetHidden(const Event &event)
{
    Window::widgetHidden(event);
    if (mItems != nullptr)
        mItems->widgetHidden(event);
    if (mTabs != nullptr)
    {
        ScrollArea *const scroll = static_cast<ScrollArea *>(
            mTabs->getCurrentWidget());
        if (scroll != nullptr)
        {
            ShortcutContainer *const content = static_cast<ShortcutContainer*>(
                scroll->getContent());

            if (content != nullptr)
                content->widgetHidden(event);
        }
    }
}

void ShortcutWindow::mousePressed(MouseEvent &event)
{
    Window::mousePressed(event);

    if (event.isConsumed())
        return;

    if (event.getButton() == MouseButton::LEFT)
    {
        mDragOffsetX = event.getX();
        mDragOffsetY = event.getY();
    }
}

void ShortcutWindow::mouseDragged(MouseEvent &event)
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

void ShortcutWindow::widgetMoved(const Event& event)
{
    Window::widgetMoved(event);
    if (mItems != nullptr)
        mItems->setRedraw(true);
    FOR_EACH (STD_VECTOR<ShortcutContainer*>::iterator, it, mPages)
        (*it)->setRedraw(true);
}

void ShortcutWindow::nextTab()
{
    if (mTabs != nullptr)
        mTabs->selectNextTab();
}

void ShortcutWindow::prevTab()
{
    if (mTabs != nullptr)
        mTabs->selectPrevTab();
}

#ifdef USE_PROFILER
void ShortcutWindow::logicChildren()
{
    BLOCK_START("ShortcutWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("ShortcutWindow::logicChildren")
}
#endif  // USE_PROFILER
