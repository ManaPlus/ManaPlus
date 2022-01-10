/*
 *  The ManaPlus Client
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

#include "gui/windows/bankwindow.h"

#include "net/bankhandler.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/db/unitsdb.h"

#include "debug.h"

BankWindow *bankWindow = nullptr;

BankWindow::BankWindow() :
    // TRANSLATORS: bank window name
    Window(_("Bank"), Modal_false, nullptr, "bank.xml"),
    ActionListener(),
    BankListener(),
    mBankMoneyLabel(new Label(this, strprintf(
        // TRANSLATORS: bank window money label
        _("Money in bank: %s"), "            "))),
    mInputMoneyTextField(new IntTextField(this, 0, 0, 2147483647,
        Enable_true, 0)),
    // TRANSLATORS: bank window button
    mWithdrawButton(new Button(this, _("Withdraw"), "withdraw",
        BUTTON_SKIN, this)),
    // TRANSLATORS: bank window button
    mDepositButton(new Button(this, _("Deposit"), "deposit",
        BUTTON_SKIN, this))
{
    setWindowName("Bank");
    setCloseButton(true);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    mBankMoneyLabel->adjustSize();
    ContainerPlacer placer = getPlacer(0, 0);
    placer(0, 0, mBankMoneyLabel, 7, 1);
    placer(0, 1, mInputMoneyTextField, 10, 1);
    placer(0, 2, mDepositButton, 5, 1);
    placer(5, 2, mWithdrawButton, 5, 1);

    setContentSize(300, 100);
    setDefaultSize(300, 100, ImagePosition::CENTER, 0, 0);

    center();
    setDefaultSize();
    loadWindowState();
    reflowLayout(300, 0);
    enableVisibleSound(true);
}

BankWindow::~BankWindow()
{
}

void BankWindow::widgetShown(const Event &event)
{
    if (event.getSource() == this)
        bankHandler->check();
}

void BankWindow::bankMoneyChanged(const int money)
{
    // TRANSLATORS: bank window money label
    mBankMoneyLabel->setCaption(strprintf(_("Money in bank: %s"),
        UnitsDb::formatCurrency(money).c_str()));
}

void BankWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "deposit")
        bankHandler->deposit(mInputMoneyTextField->getValue());
    else if (eventId == "withdraw")
        bankHandler->withdraw(mInputMoneyTextField->getValue());
}
