/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_TABS_SETUP_RELATIONS_H
#define GUI_WIDGETS_TABS_SETUP_RELATIONS_H

#include "being/playerrelations.h"

#include "gui/widgets/tabs/setuptab.h"

class Button;
class CheckBox;
class DropDown;
class GuiTable;
class PlayerTableModel;
class ScrollArea;
class StaticTableModel;

namespace gcn
{
    class ListModel;
}

class Setup_Relations final : public SetupTab,
                              public PlayerRelationsListener
{
public:
    explicit Setup_Relations(const Widget2 *const widget);

    A_DELETE_COPY(Setup_Relations)

    ~Setup_Relations();

    void apply() override final;

    void cancel() override final;

    void reset();

    void action(const gcn::ActionEvent &event) override final;

    void updatedPlayer(const std::string &name);

    void updateAll();

    void externalUpdated() override final;

private:
    StaticTableModel *mPlayerTableTitleModel;
    PlayerTableModel *mPlayerTableModel;
    GuiTable *mPlayerTable;
    GuiTable *mPlayerTitleTable;
    ScrollArea *mPlayerScrollArea;

    CheckBox *mDefaultTrading;
    CheckBox *mDefaultWhisper;

    Button *mDeleteButton;

    gcn::ListModel *mIgnoreActionChoicesModel;
    DropDown *mIgnoreActionChoicesBox;
};

#endif  // GUI_WIDGETS_TABS_SETUP_RELATIONS_H
