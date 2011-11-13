/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "botcheckerwindow.h"

#include <SDL.h>
#include <SDL_thread.h>
#include <vector>
#include <algorithm>

#include "gui/chatwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/guitable.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "localplayer.h"
#include "main.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

#define COLUMNS_NR 5 // name plus listbox
#define NAME_COLUMN 0
#define TIME_COLUMN 1

#define ROW_HEIGHT 12
// The following column widths really shouldn't be hardcoded but should scale with the size of the widget... excep
// that, right now, the widget doesn't exactly scale either.
#define NAME_COLUMN_WIDTH 185
#define TIME_COLUMN_WIDTH 70

#define WIDGET_AT(row, column) (((row) * COLUMNS_NR) + column)

class UsersTableModel : public TableModel
{
public:
    UsersTableModel() :
        mPlayers(0)
    {
        playersUpdated();
    }

    virtual ~UsersTableModel()
    {
        freeWidgets();
    }

    virtual int getRows() const
    {
        return static_cast<int>(mPlayers.size());
    }

    virtual int getColumns() const
    {
        return COLUMNS_NR;
    }

    virtual int getRowHeight() const
    {
        return ROW_HEIGHT;
    }

    virtual int getColumnWidth(int index) const
    {
        if (index == NAME_COLUMN)
            return NAME_COLUMN_WIDTH;
        else
            return TIME_COLUMN_WIDTH;
    }

    virtual void playersUpdated()
    {
        signalBeforeUpdate();

        freeWidgets();
        mPlayers.clear();
        if (actorSpriteManager && botCheckerWindow
            && botCheckerWindow->mEnabled)
        {
            std::set<ActorSprite*> beings = actorSpriteManager->getAll();
            for (ActorSprites::const_iterator i = beings.begin();
                 i != beings.end(); ++i)
            {
                Being *being = dynamic_cast<Being*>(*i);

                if (being && being->getType() == Being::PLAYER
                    && being != player_node && being->getName() != "")
                {
                    mPlayers.push_back(being);
                }
            }
        }

        unsigned int curTime = cur_time;
        // set up widgets
        for (unsigned int r = 0; r < mPlayers.size(); ++r)
        {
            if (!mPlayers.at(r))
                continue;

            std::string name = mPlayers.at(r)->getName();
            gcn::Widget *widget = new Label(name);

            mWidgets.push_back(widget);

            if (mPlayers.at(r)->getAttackTime() != 0)
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getAttackTime()));
            }
            else
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getTestTime()) + "?");
            }
            mWidgets.push_back(widget);

            if (mPlayers.at(r)->getTalkTime() != 0)
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getTalkTime()));
            }
            else
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getTestTime()) + "?");
            }
            mWidgets.push_back(widget);

            if (mPlayers.at(r)->getMoveTime() != 0)
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getMoveTime()));
            }
            else
            {
                widget = new Label(toString(curTime
                    - mPlayers.at(r)->getTestTime()) + "?");
            }
            mWidgets.push_back(widget);

            std::string str;
            bool talkBot = false;
            bool moveBot = false;
            bool attackBot = false;
            bool otherBot = false;

            if (curTime - mPlayers.at(r)->getTestTime() > 2 * 60)
            {
                int attack = curTime - (mPlayers.at(r)->getAttackTime()
                    ? mPlayers.at(r)->getAttackTime()
                    : mPlayers.at(r)->getTestTime());
                int talk = curTime - (mPlayers.at(r)->getTalkTime()
                    ? mPlayers.at(r)->getTalkTime()
                    : mPlayers.at(r)->getTestTime()) - attack;
                int move = curTime - (mPlayers.at(r)->getMoveTime()
                    ? mPlayers.at(r)->getMoveTime()
                    : mPlayers.at(r)->getTestTime()) - attack;
                int other = curTime - (mPlayers.at(r)->getOtherTime()
                    ? mPlayers.at(r)->getMoveTime()
                    : mPlayers.at(r)->getOtherTime()) - attack;

                if (attack < 2 * 60)
                    attackBot = true;

                // attacking but not talking more than 2 minutes
                if (talk > 2 * 60 && talk > 2 * 60)
                {
                    talkBot = true;
                    str += toString((talk) / 60) + " ";
                }

                // attacking but not moving more than 2 minutes
                if (move > 2 * 60 && move > 2 * 60)
                {
                    moveBot = true;
                    str += toString((move) / 60);
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

            widget = new Label(str);
            mWidgets.push_back(widget);

        }

        signalAfterUpdate();
    }

    virtual void updateModelInRow(int row A_UNUSED)
    {
    }


    virtual gcn::Widget *getElementAt(int row, int column) const
    {
        return mWidgets[WIDGET_AT(row, column)];
    }

    virtual void freeWidgets()
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
    Window(_("Bot Checker"), false, nullptr, "botchecker.xml"),
    mEnabled(false)
{
    int w = 500;
    int h = 250;

    setSaveVisible(true);
    mLastUpdateTime = 0;
    mNeedUpdate = false;

    mTableModel = new UsersTableModel();
    mTable = new GuiTable(mTableModel);
    mTable->setOpaque(false);
    mTable->setLinewiseSelection(true);
    mTable->setWrappingEnabled(true);
    mTable->setActionEventId("skill");
    mTable->addActionListener(this);

    mPlayerTableTitleModel = new StaticTableModel(1, COLUMNS_NR);
    mPlayerTableTitleModel->fixColumnWidth(NAME_COLUMN, NAME_COLUMN_WIDTH);

    for (int f = 0; f < 4; f++)
    {
        mPlayerTableTitleModel->fixColumnWidth(TIME_COLUMN + f,
                                               TIME_COLUMN_WIDTH);
    }

    mPlayerTitleTable = new GuiTable(mPlayerTableTitleModel);
    mPlayerTitleTable->setHeight(1);

    mPlayerTableTitleModel->set(0, 0, new Label(_("Name")));
    mPlayerTableTitleModel->set(0, 1, new Label(_("Attack")));
    mPlayerTableTitleModel->set(0, 2, new Label(_("Talk")));
    mPlayerTableTitleModel->set(0, 3, new Label(_("Move")));
    mPlayerTableTitleModel->set(0, 4, new Label(_("Result")));

    mPlayerTitleTable->setLinewiseSelection(true);

    setWindowName("BotCheckerWindow");
    setCloseButton(true);
    setStickyButtonLock(true);
    setDefaultSize(w, h, ImageRect::CENTER);

    playersScrollArea = new ScrollArea(mTable);

    mIncButton = new Button(_("Reset"), "reset", this);
    playersScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mPlayerTitleTable->setPosition(getPadding(), getPadding());
    mPlayerTitleTable->setWidth(w - 10);
    mPlayerTitleTable->setHeight(20);

    playersScrollArea->setPosition(getPadding(), 20 + 2*getPadding());
    playersScrollArea->setWidth(w - 15);
    playersScrollArea->setHeight(h - 80);

    mIncButton->setPosition(getPadding(), 190 + 3*getPadding());
    mIncButton->setWidth(80);
    mIncButton->setHeight(20);

    add(mPlayerTitleTable);
    add(playersScrollArea);
    add(mIncButton);

    center();

    setWidth(w);
    setHeight(h);
    loadWindowState();

    config.addListener("enableBotCheker", this);
    mEnabled = config.getBoolValue("enableBotCheker");
}

BotCheckerWindow::~BotCheckerWindow()
{
    config.removeListener("enableBotCheker", this);
}

void BotCheckerWindow::logic()
{
    if (mEnabled && mTableModel)
    {
        unsigned int nowTime = cur_time;
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

    Window::logic();
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
    if (actorSpriteManager)
    {
        std::set<ActorSprite*> beings = actorSpriteManager->getAll();
        for (ActorSprites::const_iterator i = beings.begin();
             i != beings.end(); ++i)
        {
            Being *being = dynamic_cast<Being*>(*i);

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
