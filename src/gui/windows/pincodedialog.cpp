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

#include "gui/windows/pincodedialog.h"

#include "pincodemanager.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/pincode.h"

#include "utils/gettext.h"

#include "gui/fonts/font.h"

#include "debug.h"

int PincodeDialog::instances = 0;

PincodeDialog::PincodeDialog(const std::string &restrict title,
                             const std::string &restrict msg,
                             uint32_t seed,
                             Window *const parent) :
    Window(title, Modal_true, parent, "pincode.xml"),
    ActionListener(),
    mPasswordField(new PasswordField(this, std::string())),
    mPincode(new Pincode(this, mPasswordField)),
    // TRANSLATORS: text dialog button
    mOkButton(new Button(this, _("OK"), "OK", BUTTON_SKIN, this))
{
    setStickyButtonLock(true);

    Label *const textLabel = new Label(this, msg);
    Button *const cancelButton = new Button(this,
        // TRANSLATORS: text dialog button
        _("Cancel"),
        "CANCEL",
        BUTTON_SKIN,
        this);

    place(0, 0, textLabel, 4, 1);
    place(0, 1, mPincode, 4, 1);
    place(0, 2, mPasswordField, 4, 1);
    place(2, 3, mOkButton, 1, 1);
    place(3, 3, cancelButton, 1, 1);

    mPasswordField->setEnabled(false);
    mPasswordField->setActionEventId("pincode");
    mPasswordField->addActionListener(this);

    mPincode->shuffle(seed);
    int width = getFont()->getWidth(title);
    if (width < textLabel->getWidth())
        width = textLabel->getWidth();
    if (width < mPincode->getWidth())
        width = mPincode->getWidth();
    reflowLayout(CAST_S32(width + 20), 0);
    updateButtons();
}

void PincodeDialog::postInit()
{
    Window::postInit();
    if (getParent() != nullptr)
    {
        setLocationRelativeTo(getParent());
        getParent()->moveToTop(this);
    }
    setVisible(Visible_true);
    requestModalFocus();
    mPincode->requestFocus();

    instances++;
}

PincodeDialog::~PincodeDialog()
{
    instances--;
    pincodeManager.clearDialog(this);
}

void PincodeDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "pincode")
    {
        updateButtons();
        return;
    }
    if (eventId == "CANCEL")
        setActionEventId("~" + getActionEventId());

    distributeActionEvent();
    close();
}

const std::string &PincodeDialog::getMsg() const
{
    return mPasswordField->getText();
}

void PincodeDialog::close()
{
    pincodeManager.clearDialog(this);
    scheduleDelete();
}

void PincodeDialog::updateButtons()
{
    if (mPasswordField->getText().size() == 4)
    {
        mOkButton->setEnabled(true);
    }
    else
    {
        mOkButton->setEnabled(false);
    }
}
