/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_TEXTSELECTDIALOG_H
#define GUI_WINDOWS_TEXTSELECTDIALOG_H

#include "gui/widgets/window.h"

#include "enums/simpletypes/allowquit.h"

#include "listeners/actionlistener.h"
#include "listeners/selectionlistener.h"

class Button;
class ScrollArea;
class NamesModel;
class ListBox;

class TextSelectDialog notfinal : public Window,
                                  public ActionListener,
                                  private SelectionListener
{
    public:
        /**
         * Constructor.
         */
        TextSelectDialog(const std::string &name,
                         const std::string &selectButton,
                         const AllowQuit allowQuit);

        A_DELETE_COPY(TextSelectDialog)

        /**
         * Destructor
         */
        ~TextSelectDialog() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Updates labels according to selected item.
         *
         * @see SelectionListener::selectionChanged
         */
        void valueChanged(const SelectionEvent &event) override final;

        /**
         * Sets the visibility of this window.
         */
        void setVisible(Visible visible) override final;

        void postInit() override;

        void addText(const std::string &text);

        std::string getText() const noexcept2 A_WARN_UNUSED
        { return mText; }

        void setTag(const int tag) noexcept2
        { mTag = tag; }

        int getTag() const noexcept2 A_WARN_UNUSED
        { return mTag; }

    protected:
        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        std::string mSelectButtonName;
        std::string mText;
        Button *mSelectButton A_NONNULLPOINTER;
        Button *mQuitButton A_NONNULLPOINTER;
        ListBox *mItemList A_NONNULLPOINTER;
        ScrollArea *mScrollArea A_NONNULLPOINTER;
        NamesModel *mModel A_NONNULLPOINTER;
        AllowQuit mAllowQuit;
        int mTag;
};

#endif  // GUI_WINDOWS_TEXTSELECTDIALOG_H
