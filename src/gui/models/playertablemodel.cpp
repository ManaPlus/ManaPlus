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

#include "gui/models/playertablemodel.h"

#include "being/playerrelations.h"

#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"

#include "gui/models/playerrelationlistmodel.h"

#include "utils/delete2.h"
#include "utils/dtor.h"

#include "debug.h"

static const int COLUMNS_NR = 2;  // name plus listbox
static const int NAME_COLUMN = 0;
static const unsigned int RELATION_CHOICE_COLUMN = 1;

static const unsigned int ROW_HEIGHT = 12;
// The following column widths really shouldn't be hardcoded
// but should scale with the size of the widget... except
// that, right now, the widget doesn't exactly scale either.
static const unsigned int NAME_COLUMN_WIDTH = 230;
static const unsigned int RELATION_CHOICE_COLUMN_WIDTH = 80;

#define WIDGET_AT(row, column) (((row) * COLUMNS_NR) + (column))

PlayerTableModel::PlayerTableModel(const Widget2 *const widget) :
    Widget2(widget),
    TableModel(),
    mPlayers(nullptr),
    mWidgets(),
    mListModel(new PlayerRelationListModel)
{
    playerRelationsUpdated();
}

PlayerTableModel::~PlayerTableModel()
{
    freeWidgets();
    delete2(mListModel)
    delete2(mPlayers)
}

int PlayerTableModel::getRows() const
{
    if (mPlayers != nullptr)
        return CAST_S32(mPlayers->size());
    return 0;
}

int PlayerTableModel::getColumns() const
{
    return COLUMNS_NR;
}

int PlayerTableModel::getRowHeight() const
{
    return ROW_HEIGHT;
}

int PlayerTableModel::getColumnWidth(const int index) const
{
    if (index == NAME_COLUMN)
        return NAME_COLUMN_WIDTH;
    return RELATION_CHOICE_COLUMN_WIDTH;
}

void PlayerTableModel::playerRelationsUpdated()
{
    signalBeforeUpdate();

    freeWidgets();
    StringVect *const player_names = playerRelations.getPlayers();
    delete mPlayers;
    mPlayers = player_names;

    // set up widgets
    for (unsigned int r = 0, fsz = CAST_U32(
         player_names->size()); r < fsz; ++r)
    {
        const std::string name = (*player_names)[r];
        Widget *const widget = new Label(this, name);
        mWidgets.push_back(widget);

        DropDown *const choicebox = new DropDown(this,
            mListModel,
            false,
            Modal_false,
            nullptr,
            std::string());
        choicebox->setSelected(CAST_S32(
            playerRelations.getRelation(name)));
        mWidgets.push_back(choicebox);
    }

    signalAfterUpdate();
}

void PlayerTableModel::updateModelInRow(const int row) const
{
    const DropDown *const choicebox = static_cast<DropDown *>(
        getElementAt(row, RELATION_CHOICE_COLUMN));
    if (choicebox == nullptr)
        return;
    playerRelations.setRelation(getPlayerAt(row),
        static_cast<RelationT>(
        choicebox->getSelected()));
}

Widget *PlayerTableModel::getElementAt(int row, int column) const
{
    return mWidgets[WIDGET_AT(row, column)];
}

void PlayerTableModel::freeWidgets()
{
    delete2(mPlayers)
    delete_all(mWidgets);
    mWidgets.clear();
}

std::string PlayerTableModel::getPlayerAt(const int index) const
{
    if ((mPlayers == nullptr) || index < 0
        || index >= CAST_S32(mPlayers->size()))
    {
        return std::string();
    }
    return (*mPlayers)[index];
}
