/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/windows/clanwindow.h"

#include "being/localclan.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/createwidget.h"
#include "gui/widgets/tabbedarea.h"

#include "gui/widgets/tabs/clanwindowtabs.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

ClanWindow *clanWindow = nullptr;

ClanWindow::ClanWindow() :
    // TRANSLATORS: clan window name
    Window(_("Clan"), Modal_false, nullptr, "clan.xml"),
    mTabs(CREATEWIDGETR(TabbedArea, this)),
    mInfoWidget(new InfoClanTab(this)),
    mStatsWidget(new StatsClanTab(this)),
    mAllyWidget(new RelationClanTab(this)),
    mAntagonistWidget(new RelationClanTab(this))
{
    setWindowName("clan");
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
    // TRANSLATORS: clan window tab
    mTabs->addTab(std::string(_("Info")), mInfoWidget);
    // TRANSLATORS: clan window tab
    mTabs->addTab(std::string(_("Stats")), mStatsWidget);
    // TRANSLATORS: clan window tab
    mTabs->addTab(std::string(_("Ally")), mAllyWidget);
    // TRANSLATORS: clan window tab
    mTabs->addTab(std::string(_("Antagonist")), mAntagonistWidget);

    mTabs->setDimension(Rect(0, 0, 600, 300));

    const int w = mDimension.width;
    const int h = mDimension.height;
    mInfoWidget->setDimension(Rect(0, 0, w, h));
    mStatsWidget->setDimension(Rect(0, 0, w, h));
    mAllyWidget->setDimension(Rect(0, 0, w, h));
    mAntagonistWidget->setDimension(Rect(0, 0, w, h));
    loadWindowState();
    enableVisibleSound(true);
    resetClan();
}

ClanWindow::~ClanWindow()
{
    delete2(mInfoWidget);
    delete2(mStatsWidget);
    delete2(mAllyWidget);
    delete2(mAntagonistWidget);
}

void ClanWindow::postInit()
{
    Window::postInit();
    add(mTabs);
}

void ClanWindow::slowLogic()
{
    BLOCK_START("ClanWindow::slowLogic")
    if (!isWindowVisible() || (mTabs == nullptr))
    {
        BLOCK_END("ClanWindow::slowLogic")
        return;
    }

    switch (mTabs->getSelectedTabIndex())
    {
        default:
        case 0:
            mInfoWidget->logic();
            break;
        case 1:
            mStatsWidget->logic();
            break;
        case 2:
            mAllyWidget->logic();
            break;
        case 3:
            mAntagonistWidget->logic();
            break;
    }

    BLOCK_END("ClanWindow::slowLogic")
}

void ClanWindow::widgetResized(const Event &event)
{
    Window::widgetResized(event);

    mTabs->setDimension(Rect(0, 0,
        mDimension.width, mDimension.height));
}

void ClanWindow::updateClan()
{
    mInfoWidget->updateClan();
    mStatsWidget->updateClan();
    mAllyWidget->updateClan(localClan.allyClans);
    mAntagonistWidget->updateClan(localClan.antagonistClans);
}

void ClanWindow::resetClan()
{
    mInfoWidget->resetClan();
    mStatsWidget->resetClan();
    mAllyWidget->resetClan();
    mAntagonistWidget->resetClan();
}

void ClanWindow::updateClanMembers()
{
}

#ifdef USE_PROFILER
void ClanWindow::logicChildren()
{
    BLOCK_START("ClanWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("ClanWindow::logicChildren")
}
#endif  // USE_PROFILER
