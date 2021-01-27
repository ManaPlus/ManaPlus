/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "gui/windows/debugwindow.h"

#include "being/localplayer.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/tabs/mapdebugtab.h"
#include "gui/widgets/tabs/netdebugtab.h"
#include "gui/widgets/tabs/statdebugtab.h"
#include "gui/widgets/tabs/targetdebugtab.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

DebugWindow *debugWindow = nullptr;

DebugWindow::DebugWindow(const std::string &name) :
    // TRANSLATORS: debug window name
    Window(_("Debug"), Modal_false, nullptr, "debug.xml"),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    mMapWidget(new MapDebugTab(this)),
    mTargetWidget(new TargetDebugTab(this)),
    mNetWidget(new NetDebugTab(this)),
    mStatWidget(new StatDebugTab(this))
{
    setWindowName(name);
    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    setDefaultSize(400, 300, ImagePosition::CENTER, 0, 0);

    mTabs->setSelectable(false);
    mTabs->getWidgetContainer()->setSelectable(false);
    mTabs->getTabContainer()->setSelectable(false);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Map")), mMapWidget);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Target")), mTargetWidget);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Net")), mNetWidget);
    // TRANSLATORS: debug window tab
    mTabs->addTab(std::string(_("Stat")), mStatWidget);

    mTabs->setDimension(Rect(0, 0, 600, 300));

    const int w = mDimension.width;
    const int h = mDimension.height;
    mMapWidget->resize(w, h);
    mTargetWidget->resize(w, h);
    mNetWidget->resize(w, h);
    mStatWidget->resize(w, h);
    loadWindowState();
    enableVisibleSound(true);
}

DebugWindow::~DebugWindow()
{
    delete2(mMapWidget)
    delete2(mTargetWidget)
    delete2(mNetWidget)
    delete2(mStatWidget)
}

void DebugWindow::postInit()
{
    Window::postInit();
    add(mTabs);
}

void DebugWindow::slowLogic()
{
    BLOCK_START("DebugWindow::slowLogic")
    if (!isWindowVisible() || (mTabs == nullptr))
    {
        BLOCK_END("DebugWindow::slowLogic")
        return;
    }

    switch (mTabs->getSelectedTabIndex())
    {
        default:
        case 0:
            mMapWidget->logic();
            break;
        case 1:
            mTargetWidget->logic();
            break;
        case 2:
            mNetWidget->logic();
            break;
        case 3:
            mStatWidget->logic();
            break;
    }

    if (localPlayer != nullptr)
        localPlayer->tryPingRequest();
    BLOCK_END("DebugWindow::slowLogic")
}

void DebugWindow::draw(Graphics *const g)
{
    BLOCK_START("DebugWindow::draw")
    Window::draw(g);

    if (localPlayer != nullptr)
    {
        const Being *const target = localPlayer->getTarget();
        if (target != nullptr)
        {
            target->draw(g, -target->getPixelX() + mapTileSize / 2
                + mDimension.width / 2, -target->getPixelY() + mapTileSize
                + mDimension.height / 2);
        }
    }
    BLOCK_END("DebugWindow::draw")
}

void DebugWindow::safeDraw(Graphics *const g)
{
    BLOCK_START("DebugWindow::draw")
    Window::safeDraw(g);

    if (localPlayer != nullptr)
    {
        const Being *const target = localPlayer->getTarget();
        if (target != nullptr)
        {
            target->draw(g, -target->getPixelX() + mapTileSize / 2
                + mDimension.width / 2, -target->getPixelY() + mapTileSize
                + mDimension.height / 2);
        }
    }
    BLOCK_END("DebugWindow::draw")
}

void DebugWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    mTabs->setDimension(Rect(0, 0,
        mDimension.width, mDimension.height));
}

#ifdef USE_PROFILER
void DebugWindow::logicChildren()
{
    BLOCK_START("DebugWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("DebugWindow::logicChildren")
}
#endif  // USE_PROFILER
