/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SETUPTOUCHITEM_H
#define GUI_WIDGETS_SETUPTOUCHITEM_H

#include "gui/widgets/namesmodel.h"
#include "gui/widgets/setupitem.h"

class TouchActionsModel final : public NamesModel
{
    public:
        TouchActionsModel();

        A_DELETE_COPY(TouchActionsModel)

        ~TouchActionsModel()
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
        SetupActionDropDown(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            TouchActionsModel *restrict const model,
                            const int width, const bool mainConfig = true);

        SetupActionDropDown(const std::string &restrict text,
                            const std::string &restrict description,
                            const std::string &restrict keyName,
                            SetupTabScroll *restrict const parent,
                            const std::string &restrict eventName,
                            TouchActionsModel *restrict const model,
                            const int width, const std::string &def,
                            const bool mainConfig = true);

        A_DELETE_COPY(SetupActionDropDown)

        ~SetupActionDropDown();

        void createControls();

        void fromWidget() override final;

        void toWidget() override final;

    protected:
        HorizontContainer *mHorizont;
        Label *mLabel;
        TouchActionsModel *mModel;
        DropDown *mDropDown;
        int mWidth;
};

#endif  // GUI_WIDGETS_SETUPTOUCHITEM_H
