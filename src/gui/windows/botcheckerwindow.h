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

#ifndef GUI_BOTCHECKERWINDOW_H
#define GUI_BOTCHECKERWINDOW_H

#include "listeners/configlistener.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

struct BOTCHK final
{
    int16_t id;      /**< Index into "botchecker_db" array */
    int16_t lv;
    int16_t sp;
};

class Button;
class GuiTable;
class ScrollArea;
class UsersTableModel;
class StaticTableModel;

class BotCheckerWindow final : public Window,
                               public ActionListener,
                               public ConfigListener
{
    public:
        friend class UsersTableModel;

        /**
         * Constructor.
         */
        BotCheckerWindow();

        A_DELETE_COPY(BotCheckerWindow)

        /**
         * Destructor.
         */
        ~BotCheckerWindow();

        void action(const ActionEvent &event) override final;

        void update();

        void slowLogic();

        void updateList();

        void reset();

        void optionChanged(const std::string &name) override final;

#ifdef USE_PROFILER
        void logicChildren();
#endif

    private:
        UsersTableModel *mTableModel;
        GuiTable *mTable;
        ScrollArea *playersScrollArea;
        StaticTableModel *mPlayerTableTitleModel;
        GuiTable *mPlayerTitleTable;
        Button *mIncButton;
        int mLastUpdateTime;
        bool mNeedUpdate;
        bool mBotcheckerEnabled;
};

extern BotCheckerWindow *botCheckerWindow;

#endif  // GUI_BOTCHECKERWINDOW_H
