/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_MODELS_PLAYERTABLEMODEL_H
#define GUI_MODELS_PLAYERTABLEMODEL_H

#include "gui/widgets/widget2.h"

#include "gui/models/tablemodel.h"

#include "localconsts.h"

class PlayerRelationListModel;

class PlayerTableModel final : public Widget2,
                               public TableModel
{
    public:
        explicit PlayerTableModel(const Widget2 *const widget);

        A_DELETE_COPY(PlayerTableModel)

        ~PlayerTableModel() override final;

        int getRows() const override final;

        int getColumns() const override final A_CONST;

        int getRowHeight() const override final A_CONST;

        int getColumnWidth(const int index) const override final A_CONST;

        void playerRelationsUpdated();

        void updateModelInRow(const int row) const;

        Widget *getElementAt(int row, int column) const override final;

        void freeWidgets();

        std::string getPlayerAt(const int index) const;

    protected:
        StringVect *mPlayers;
        STD_VECTOR<Widget *> mWidgets;
        PlayerRelationListModel *mListModel;
};

#endif  // GUI_MODELS_PLAYERTABLEMODEL_H
