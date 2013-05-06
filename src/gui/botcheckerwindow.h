/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#ifndef BOTCHECKER_H
#define BOTCHECKER_H

#include "configlistener.h"

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

#include <vector>

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
                               public gcn::ActionListener,
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

        void action(const gcn::ActionEvent &event) override;

        void update();

        void slowLogic();

        void updateList();

        void reset();

        void optionChanged(const std::string &name) override;

    private:
        UsersTableModel *mTableModel;
        GuiTable *mTable;
        ScrollArea *playersScrollArea;
        StaticTableModel *mPlayerTableTitleModel;
        GuiTable *mPlayerTitleTable;
        Button *mIncButton;
        int mLastUpdateTime;
        bool mNeedUpdate;
        bool mEnabled;
};

extern BotCheckerWindow *botCheckerWindow;

#endif
