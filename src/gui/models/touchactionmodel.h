/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2018  The ManaPlus Developers
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

#ifndef GUI_MODELS_TOUCHACTIONMODEL_H
#define GUI_MODELS_TOUCHACTIONMODEL_H

#include "enums/input/inputaction.h"

#include "gui/models/namesmodel.h"

#include <map>

class TouchActionsModel final : public NamesModel
{
    public:
        TouchActionsModel();

        A_DELETE_COPY(TouchActionsModel)

        InputActionT getActionFromSelection(const int sel) const;

        int getSelectionFromAction(const InputActionT action) const;

    private:
        STD_VECTOR<InputActionT> mActionId;
        std::map<InputActionT, int> mActionToSelection;
};

#endif  // GUI_MODELS_TOUCHACTIONMODEL_H
