/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_POPUPLIST_H
#define GUI_WIDGETS_POPUPLIST_H

#include "enums/simpletypes/modal.h"

#include "gui/widgets/popup.h"

#include "listeners/focuslistener.h"

#include "localconsts.h"

class DropDown;
class ListBox;
class ListModel;
class ScrollArea;

class PopupList final : public Popup,
                        public FocusListener
{
    public:
        PopupList(DropDown *const widget,
                  ListModel *const listModel,
                  const bool extended,
                  const Modal modal);

        ~PopupList() override final;

        A_DELETE_COPY(PopupList)

        void postInit() override final;

        void show(int x, int y);

        void widgetResized(const Event &event) override final;

        void setSelected(const int selected);

        int getSelected() const;

        void setListModel(ListModel *const model);

        ListModel *getListModel() const
        { return mListModel; }

        void adjustSize();

        void focusGained(const Event& event A_UNUSED) override final;

        void focusLost(const Event& event A_UNUSED) override final;

        void mousePressed(MouseEvent& event) override final;

        void mouseReleased(MouseEvent& event) override final;

    private:
        ListModel *mListModel;
        ListBox *mListBox A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        DropDown *mDropDown;
        int mPressedIndex;
        Modal mModal;
};

#endif  // GUI_WIDGETS_POPUPLIST_H
