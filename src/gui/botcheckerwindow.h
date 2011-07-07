/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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

#ifndef BOTCHECKER_H
#define BOTCHECKER_H

#include "configlistener.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

#include <vector>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

struct BOTCHK
{
    short id;      /**< Index into "botchecker_db" array */
    short lv, sp;
};

class GuiTable;
class ScrollArea;
class UsersTableModel;
class StaticTableModel;

class BotCheckerWindow : public Window, public gcn::ActionListener,
                         public ConfigListener
{
    public:
        friend class UsersTableModel;

        /**
         * Constructor.
         */
        BotCheckerWindow();

        /**
         * Destructor.
         */
        ~BotCheckerWindow();

        void action(const gcn::ActionEvent &event);

        void update();

        void logic();

        void widgetResized(const gcn::Event &event);

        void updateList();

        void reset();

        void optionChanged(const std::string &name);

    private:
        GuiTable *mTable;
        ScrollArea *playersScrollArea;
        UsersTableModel *mTableModel;
        StaticTableModel *mPlayerTableTitleModel;
        GuiTable *mPlayerTitleTable;
        gcn::Button *mIncButton;
        int mLastUpdateTime;
        bool mNeedUpdate;
        bool mEnabled;
};

extern BotCheckerWindow *botCheckerWindow;

#endif
