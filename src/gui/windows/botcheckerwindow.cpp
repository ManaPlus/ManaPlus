/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "gui/windows/botcheckerwindow.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/guitable.h"

#include "gui/models/userstablemodel.h"

#include "client.h"
#include "configuration.h"

#include "utils/gettext.h"

#include "debug.h"

BotCheckerWindow *botCheckerWindow = nullptr;

BotCheckerWindow::BotCheckerWindow() :
    // TRANSLATORS: bot checker window header
    Window(_("Bot Checker"), false, nullptr, "botchecker.xml"),
    ActionListener(),
    mTableModel(new UsersTableModel(this)),
    mTable(new GuiTable(this, mTableModel)),
    playersScrollArea(new ScrollArea(this, mTable, true,
        "bochecker_background.xml")),
    mPlayerTableTitleModel(new StaticTableModel(1, COLUMNS_NR)),
    mPlayerTitleTable(new GuiTable(this, mPlayerTableTitleModel)),
    // TRANSLATORS: bot checker window button
    mIncButton(new Button(this, _("Reset"), "reset", this)),
    mLastUpdateTime(0),
    mNeedUpdate(false),
    mBotcheckerEnabled(false)
{
    const int w = 500;
    const int h = 250;

    setSaveVisible(true);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    mTable->setOpaque(false);
    mTable->setLinewiseSelection(true);
    mTable->setWrappingEnabled(true);
    mTable->setActionEventId("skill");
    mTable->addActionListener(this);

    mPlayerTableTitleModel->fixColumnWidth(NAME_COLUMN, NAME_COLUMN_WIDTH);

    for (int f = 0; f < 4; f++)
    {
        mPlayerTableTitleModel->fixColumnWidth(
            TIME_COLUMN + f, TIME_COLUMN_WIDTH);
    }

    mPlayerTitleTable->setHeight(1);
    mPlayerTitleTable->setBackgroundColor(getThemeColor(
        Theme::TABLE_BACKGROUND));
    mPlayerTitleTable->setSelectable(false);

    // TRANSLATORS: bot checker window table header
    mPlayerTableTitleModel->set(0, 0, new Label(this, _("Name")));
    // TRANSLATORS: bot checker window table header
    mPlayerTableTitleModel->set(0, 1, new Label(this, _("Attack")));
    // TRANSLATORS: bot checker window table header
    mPlayerTableTitleModel->set(0, 2, new Label(this, _("Talk")));
    // TRANSLATORS: bot checker window table header
    mPlayerTableTitleModel->set(0, 3, new Label(this, _("Move")));
    // TRANSLATORS: bot checker window table header
    mPlayerTableTitleModel->set(0, 4, new Label(this, _("Result")));

    mPlayerTitleTable->setLinewiseSelection(true);

    setWindowName("BotCheckerWindow");
    setCloseButton(true);
    setStickyButtonLock(true);
    setDefaultSize(w, h, ImageRect::CENTER);

    playersScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mIncButton->adjustSize();

    const int pad4 = mPadding * 4;
    int y = mPadding;
    mPlayerTitleTable->setPosition(mPadding, y);
    mPlayerTitleTable->setWidth(w - pad4);
    mPlayerTitleTable->setHeight(20);

    y += 20 + mPadding;
    playersScrollArea->setPosition(mPadding, y);
    playersScrollArea->setWidth(w - pad4);
    const int scrollHeight = h - y - mTitleBarHeight - mPadding * 3
        - mIncButton->getHeight();
    playersScrollArea->setHeight(scrollHeight);
    y += scrollHeight + mPadding;

    mIncButton->setPosition(mPadding, y);

    mLastHost += 0x1234;

    add(mPlayerTitleTable);
    add(playersScrollArea);
    add(mIncButton);

    center();

    setWidth(w);
    setHeight(h);
    loadWindowState();
    enableVisibleSound(true);

    config.addListener("enableBotCheker", this);
    mBotcheckerEnabled = config.getBoolValue("enableBotCheker");
}

BotCheckerWindow::~BotCheckerWindow()
{
    config.removeListener("enableBotCheker", this);
    CHECKLISTENERS
}

void BotCheckerWindow::slowLogic()
{
    BLOCK_START("BotCheckerWindow::slowLogic")
    if (mBotcheckerEnabled && mTableModel)
    {
        const unsigned int nowTime = cur_time;
        if (nowTime - mLastUpdateTime > 5 && mNeedUpdate)
        {
            mTableModel->playersUpdated();
            mNeedUpdate = false;
            mLastUpdateTime = nowTime;
        }
        else if (nowTime - mLastUpdateTime > 15)
        {
            mTableModel->playersUpdated();
            mNeedUpdate = false;
            mLastUpdateTime = nowTime;
        }
    }
    BLOCK_END("BotCheckerWindow::slowLogic")
}

void BotCheckerWindow::action(const ActionEvent &event)
{
    if (event.getId() == "reset")
    {
        reset();
        mNeedUpdate = true;
    }
}

void BotCheckerWindow::update()
{
}

void BotCheckerWindow::updateList()
{
    if (mTableModel)
        mNeedUpdate = true;
}

void BotCheckerWindow::reset()
{
    if (actorManager)
    {
        std::set<ActorSprite*> beings = actorManager->getAll();
        FOR_EACH (ActorSprites::iterator, i, beings)
        {
            if ((*i)->getType() == ActorType::Player)
            {
                Being *const being = static_cast<Being*>(*i);
                if (being != localPlayer && !being->getName().empty())
                    being->resetCounters();
            }
        }
    }
}

void BotCheckerWindow::optionChanged(const std::string &name)
{
    if (name == "enableBotCheker")
        mBotcheckerEnabled = config.getBoolValue("enableBotCheker");
}

#ifdef USE_PROFILER
void BotCheckerWindow::logicChildren()
{
    BLOCK_START("BotCheckerWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("BotCheckerWindow::logicChildren")
}
#endif
