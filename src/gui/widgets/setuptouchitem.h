/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef SETUPTOUCHITEM_H
#define SETUPTOUCHITEM_H

#include "gui/setupactiondata.h"

#include "gui/widgets/namesmodel.h"
#include "gui/widgets/setupitem.h"

#include <algorithm>

class TouchActionsModel final : public NamesModel
{
    public:
        TouchActionsModel();

        A_DELETE_COPY(TouchActionsModel)

        virtual ~TouchActionsModel()
        { }

        int getActionFromSelection(const int sel) const;

        int getSelectionFromAction(const int action) const;

    private:
        std::vector<int> mActionId;
        std::map<int, int> mActionToSelection;
};

class SetupActionDropDown final : public SetupItem
{
    public:
        SetupActionDropDown(std::string text, std::string description,
                            std::string keyName, SetupTabScroll *const parent,
                            std::string eventName,
                            TouchActionsModel *const model,
                            int width, const bool mainConfig = true);

        SetupActionDropDown(std::string text, std::string description,
                            std::string keyName, SetupTabScroll *const parent,
                            std::string eventName,
                            TouchActionsModel *const model,
                            int width, std::string def,
                            const bool mainConfig = true);

        A_DELETE_COPY(SetupActionDropDown)

        ~SetupActionDropDown();

        void createControls();

        void fromWidget() override;

        void toWidget() override;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        TouchActionsModel *mModel;
        DropDown *mDropDown;
        int mWidth;
};

#endif
