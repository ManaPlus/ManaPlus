/*
 *  The ManaPlus Client
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

#ifndef GUI_WINDOWS_MAILEDITWINDOW_H
#define GUI_WINDOWS_MAILEDITWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class IntTextField;
class Inventory;
class Item;
class ItemContainer;
class Label;
class ScrollArea;
class TextField;

class MailEditWindow final : public Window,
                             public ActionListener
{
    public:
        MailEditWindow();

        A_DELETE_COPY(MailEditWindow)

        ~MailEditWindow();

        void action(const ActionEvent &event) override final;

        void addItem(const Item *const item, const int amount);

        void setSubject(const std::string &str);

        void setTo(const std::string &str);

        void setMessage(const std::string &str);

        void close() override final;

    private:
        Button *mSendButton;
        Button *mCloseButton;
        Button *mAddButton;
        Label *mToLabel;
        Label *mSubjectLabel;
        Label *mMoneyLabel;
        Label *mItemLabel;
        Label *mMessageLabel;
        TextField *mToField;
        TextField *mSubjectField;
        IntTextField *mMoneyField;
        TextField *mMessageField;
        Inventory *mInventory;
        ItemContainer *mItemContainer;
        ScrollArea *mItemScrollArea;
};

extern MailEditWindow *mailEditWindow;

#endif  // GUI_WINDOWS_MAILEDITWINDOW_H
