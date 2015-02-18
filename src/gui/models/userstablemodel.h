/*
 *  The ManaPlus Client
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

#ifndef GUI_MODELS_USERSTABLEMODEL_H
#define GUI_MODELS_USERSTABLEMODEL_H

#include "gui/windows/botcheckerwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"

#include "gui/models/tablemodel.h"

#include "actormanager.h"

#include "being/localplayer.h"

#include "utils/stringutils.h"

#include "localconsts.h"

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

        int getRows() const override final
        {
            return static_cast<int>(mPlayers.size());
        }

        int getColumns() const override final
        {
            return COLUMNS_NR;
        }

        int getRowHeight() const override final
        {
            return ROW_HEIGHT;
        }

        int getColumnWidth(const int index) const override final
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
                && botCheckerWindow->mBotcheckerEnabled)
            {
                std::set<ActorSprite*> beings = actorManager->getAll();
                FOR_EACH (ActorSprites::iterator, i, beings)
                {
                    Being *const being = dynamic_cast<Being*>(*i);

                    if (being && being->getType() == ActorType::Player
                        && being != localPlayer && !being->getName().empty())
                    {
                        mPlayers.push_back(being);
                    }
                }
            }

            const unsigned int curTime = cur_time;
            const size_t sz = mPlayers.size();
            // set up widgets
            for (size_t r = 0; r < sz; ++r)
            {
                if (!mPlayers.at(r))
                    continue;

                const Being *const player = mPlayers.at(r);
                Widget *widget = new Label(this, player->getName());

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

        Widget *getElementAt(const int row,
                             const int column) const override final
        {
            return mWidgets[WIDGET_AT(row, column)];
        }

        void freeWidgets()
        {
            for (std::vector<Widget *>::const_iterator it = mWidgets.begin();
                 it != mWidgets.end(); ++it)
            {
                delete *it;
            }

            mWidgets.clear();
        }

    protected:
        std::vector<Being*> mPlayers;
        std::vector<Widget*> mWidgets;
};

#endif  // GUI_MODELS_USERSTABLEMODEL_H
