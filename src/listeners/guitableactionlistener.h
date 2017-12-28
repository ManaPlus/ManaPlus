/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef LISTENERS_GUITABLEACTIONLISTENER_H
#define LISTENERS_GUITABLEACTIONLISTENER_H

#include "listeners/actionlistener.h"

#include "localconsts.h"

class GuiTable;

class GuiTableActionListener final : public ActionListener
{
    public:
        GuiTableActionListener(GuiTable *const restrict table,
                               Widget *const restrict widget,
                               const int row,
                               const int column);

        A_DELETE_COPY(GuiTableActionListener)

        ~GuiTableActionListener() override final;

        void action(const ActionEvent& actionEvent) override final;

    protected:
        GuiTable *mTable;
        int mRow;
        int mColumn;
        Widget *mWidget;
};

#endif  // LISTENERS_GUITABLEACTIONLISTENER_H
