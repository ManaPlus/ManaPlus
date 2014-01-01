/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/windows/botcheckerwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/label.h"
#include "gui/widgets/guitable.h"

#include "actormanager.h"
#include "configuration.h"

#include "being/localplayer.h"

#include "utils/gettext.h"

#include <vector>

#include "debug.h"

const int COLUMNS_NR = 5;  // name plus listbox
const int NAME_COLUMN  = 0;
const int TIME_COLUMN  = 1;

const int ROW_HEIGHT  = 12;
// The following column widths really shouldn't be hardcoded but should
// scale with the size of the widget... excep
// that, right now, the widget doesn't exactly scale either.
const int NAME_COLUMN_WIDTH  = 185;
const int TIME_COLUMN_WIDTH  = 70;

#define WIDGET_AT(row, column) (((row) * COLUMNS_NR) + column)

class UsersTableModel final : public TableModel,
                              public Widget2
{
public:
    explicit UsersTableModel(const Widget2 *const widget) :
        TableModel(),
        Widget2(widget),
        mPlayers(0),
        mWidgets()
    {
        playersUpdated();
    }

    A_DELETE_COPY(UsersTableModel)

    ~UsersTableModel()
    {
        freeWidgets();
    }

    int getRows() const
    {
        return static_cast<int>(mPlayers.size());
    }

    int getColumns() const
    {
        return COLUMNS_NR;
    }

    int getRowHeight() const
    {
        return ROW_HEIGHT;
    }

    int getColumnWidth(const int index) const
    {
        if (index == NAME_COLUMN)
            return NAME_COLUMN_WIDTH;
        else
            return TIME_COLUMN_WIDTH;
    }

    void playersUpdated()
    {
        signalBeforeUpdate();

        freeWidgets();
        mPlayers.clear();
        if (actorManager && botCheckerWindow
            && botCheckerWindow->mEnabled)
        {
            std::set<ActorSprite*> beings = actorManager->getAll();
            FOR_EACH (ActorSprites::iterator, i, beings)
            {
                Being *const being = dynamic_cast<Being*>(*i);

                if (being && being->getType() == Being::PLAYER
                    && being != player_node && being->getName() != "")
                {
                    mPlayers.push_back(being);
                }
            }
        }

        const unsigned int curTime = cur_time;
        const unsigned int sz = mPlayers.size();
        // set up widgets
        for (unsigned int r = 0; r < sz; ++r)
        {
            if (!mPlayers.at(r))
                continue;

            const Being *const player = mPlayers.at(r);
            gcn::Widget *widget = new Label(this, player->getName());

            mWidgets.push_back(widget);

            if (player->getAttackTime() != 0)
            {
                widget = new Label(this, toString(curTime
                    - player->getAttackTime()));
            }
            else
            {
                widget = new Label(this, toString(curTime
                    - player->getTestTime()).append("?"));
            }
            mWidgets.push_back(widget);

            if (player->getTalkTime() != 0)
            {
                widget = new Label(this, toString(curTime
                    - player->getTalkTime()));
            }
            else
            {
                widget = new Label(this, toString(curTime
                    - player->getTestTime()).append("?"));
            }
            mWidgets.push_back(widget);

            if (player->getMoveTime() != 0)
            {
                widget = new Label(this, toString(curTime
                    - player->getMoveTime()));
            }
            else
            {
                widget = new Label(this, toString(curTime
                    - player->getTestTime()).append("?"));
            }
            mWidgets.push_back(widget);

            std::string str;
            bool talkBot = false;
            bool moveBot = false;
            bool attackBot = false;
            bool otherBot = false;

            if (curTime - player->getTestTime() > 2 * 60)
            {
                const int attack = curTime - (player->getAttackTime()
                    ? player->getAttackTime()
                    : player->getTestTime());
                const int talk = curTime - (player->getTalkTime()
                    ? player->getTalkTime()
                    : player->getTestTime()) - attack;
                const int move = curTime - (player->getMoveTime()
                    ? player->getMoveTime()
                    : player->getTestTime()) - attack;
                const int other = curTime - (player->getOtherTime()
                    ? player->getMoveTime()
                    : player->getOtherTime()) - attack;

                if (attack < 2 * 60)
                    attackBot = true;

                // attacking but not talking more than 2 minutes
                if (talk > 2 * 60)
                {
                    talkBot = true;
                    str.append(toString((talk) / 60)).append(" ");
                }

                // attacking but not moving more than 2 minutes
                if (move > 2 * 60)
                {
                    moveBot = true;
                    str.append(toString((move) / 60));
                }

                // attacking but not other activity more than 2 minutes
                if (move > 2 * 60 && other > 2 * 60)
                    otherBot = true;
            }

            if (str.length() > 0)
            {
                if (attackBot && talkBot && moveBot && otherBot)
                    str = "bot!! " + str;
                else if (attackBot && talkBot && moveBot)
                    str = "bot! " + str;
                else if (talkBot && moveBot)
                    str = "bot " + str;
                else if (talkBot || moveBot)
                    str = "bot? " + str;
            }
            else
            {
                str = "ok";
            }

            widget = new Label(this, str);
            mWidgets.push_back(widget);
        }

        signalAfterUpdate();
    }

    void updateModelInRow(const int row A_UNUSED) const
    {
    }

    gcn::Widget *getElementAt(const int row, const int column) const
    {
        return mWidgets[WIDGET_AT(row, column)];
    }

    void freeWidgets()
    {
        for (std::vector<gcn::Widget *>::const_iterator it = mWidgets.begin();
             it != mWidgets.end(); ++it)
        {
            delete *it;
        }

        mWidgets.clear();
    }

protected:
    std::vector<Being*> mPlayers;
    std::vector<gcn::Widget*> mWidgets;
};


BotCheckerWindow::BotCheckerWindow():
    // TRANSLATORS: bot checker window header
    Window(_("Bot Checker"), false, nullptr, "botchecker.xml"),
    gcn::ActionListener(),
    mTableModel(new UsersTableModel(this)),
    mTable(new GuiTable(this, mTableModel)),
    playersScrollArea(new ScrollArea(mTable, true,
        "bochecker_background.xml")),
    mPlayerTableTitleModel(new StaticTableModel(1, COLUMNS_NR)),
    mPlayerTitleTable(new GuiTable(this, mPlayerTableTitleModel)),
    // TRANSLATORS: bot checker window button
    mIncButton(new Button(this, _("Reset"), "reset", this)),
    mLastUpdateTime(0),
    mNeedUpdate(false),
    mEnabled(false)
{
    const int w = 500;
    const int h = 250;

    setSaveVisible(true);

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

    add(mPlayerTitleTable);
    add(playersScrollArea);
    add(mIncButton);

    center();

    setWidth(w);
    setHeight(h);
    loadWindowState();
    enableVisibleSound(true);

    config.addListener("enableBotCheker", this);
    mEnabled = config.getBoolValue("enableBotCheker");
}

BotCheckerWindow::~BotCheckerWindow()
{
    config.removeListener("enableBotCheker", this);
    CHECKLISTENERS
}

void BotCheckerWindow::slowLogic()
{
    BLOCK_START("BotCheckerWindow::slowLogic")
    if (mEnabled && mTableModel)
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

void BotCheckerWindow::action(const gcn::ActionEvent &event)
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
            Being *const being = dynamic_cast<Being*>(*i);

            if (being && being->getType() == Being::PLAYER
                && being != player_node && being->getName() != "")
            {
                being->resetCounters();
            }
        }
    }
}

void BotCheckerWindow::optionChanged(const std::string &name)
{
    if (name == "enableBotCheker")
        mEnabled = config.getBoolValue("enableBotCheker");
}

#ifdef USE_PROFILER
void BotCheckerWindow::logicChildren()
{
    BLOCK_START("BotCheckerWindow::logicChildren")
    BasicContainer::logicChildren();
    BLOCK_END("BotCheckerWindow::logicChildren")
}
#endif
