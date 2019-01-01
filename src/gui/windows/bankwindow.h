/*
 *  The ManaPlus Client
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

#ifndef GUI_WINDOWS_BANKWINDOW_H
#define GUI_WINDOWS_BANKWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/banklistener.h"

class Button;
class IntTextField;
class Label;

/**
 * A dialog to choose between buying or selling at a shop.
 *
 * \ingroup Interface
 */
class BankWindow final : public Window,
                         public ActionListener,
                         public BankListener
{
    public:
        BankWindow();

        A_DELETE_COPY(BankWindow)

        ~BankWindow() override final;

        void action(const ActionEvent &event) override final;

        void widgetShown(const Event &event) override final;

        void bankMoneyChanged(const int money) override final;

    private:
        Label *mBankMoneyLabel;
        IntTextField *mInputMoneyTextField;
        Button *mWithdrawButton;
        Button *mDepositButton;
};

extern BankWindow *bankWindow;

#endif  // GUI_WINDOWS_BANKWINDOW_H
