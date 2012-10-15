/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef GUI_SETUP_RELATIONS_H
#define GUI_SETUP_RELATIONS_H

#include "playerrelations.h"

#include "gui/widgets/setuptab.h"

#include <guichan/actionlistener.hpp>

class Button;
class EditDialog;
class GuiTable;
class PlayerTableModel;
class StaticTableModel;

namespace gcn
{
    class CheckBox;
    class DropDown;
    class ListModel;
    class ScrollArea;
}

class Setup_Relations final : public SetupTab,
                              public PlayerRelationsListener
{
public:
    Setup_Relations();

    A_DELETE_COPY(Setup_Relations)

    virtual ~Setup_Relations();

    void apply();

    void cancel();

    void reset();

    void action(const gcn::ActionEvent &event) override;

    virtual void updatedPlayer(const std::string &name);

    virtual void updateAll();

    virtual void externalUpdated();

private:
    StaticTableModel *mPlayerTableTitleModel;
    PlayerTableModel *mPlayerTableModel;
    GuiTable *mPlayerTable;
    GuiTable *mPlayerTitleTable;
    gcn::ScrollArea *mPlayerScrollArea;

    gcn::CheckBox *mDefaultTrading;
    gcn::CheckBox *mDefaultWhisper;

    Button *mDeleteButton;

    gcn::ListModel *mIgnoreActionChoicesModel;
    gcn::DropDown *mIgnoreActionChoicesBox;
};

#endif
