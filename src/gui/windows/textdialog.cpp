/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/windows/textdialog.h"

#include "input/keyboardconfig.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/passwordfield.h"

#include "utils/gettext.h"

#include "gui/fonts/font.h"

#include "debug.h"

int TextDialog::instances = 0;

namespace
{
    const std::string emptyStr;
}  // namespace

TextDialog::TextDialog(const std::string &restrict title,
                       const std::string &restrict msg,
                       Window *const parent,
                       const bool isPassword) :
    Window(title, Modal_true, parent, "textdialog.xml"),
    ActionListener(),
    mTextField(nullptr),
    mPasswordField(nullptr),
    // TRANSLATORS: text dialog button
    mOkButton(new Button(this, _("OK"), "OK", this)),
    mEnabledKeyboard(keyboard.isEnabled())
{
    keyboard.setEnabled(false);

    Label *const textLabel = new Label(this, msg);
    // TRANSLATORS: text dialog button
    Button *const cancelButton = new Button(this, _("Cancel"), "CANCEL", this);

    place(0, 0, textLabel, 4, 1);
    if (isPassword)
    {
        mPasswordField = new PasswordField(this, std::string());
        place(0, 1, mPasswordField, 4, 1);
    }
    else
    {
        mTextField = new TextField(this,
            std::string(),
            LoseFocusOnTab_true,
            nullptr,
            std::string(),
            false);
        place(0, 1, mTextField, 4, 1);
    }
    place(2, 2, mOkButton, 1, 1);
    place(3, 2, cancelButton, 1, 1);

    int width = getFont()->getWidth(title);
    if (width < textLabel->getWidth())
        width = textLabel->getWidth();
    reflowLayout(CAST_S32(width + 20));
}

void TextDialog::postInit()
{
    Window::postInit();
    if (getParent() != nullptr)
    {
        setLocationRelativeTo(getParent());
        getParent()->moveToTop(this);
    }
    setVisible(Visible_true);
    requestModalFocus();
    if (mPasswordField != nullptr)
        mPasswordField->requestFocus();
    else if (mTextField != nullptr)
        mTextField->requestFocus();

    instances++;
}

TextDialog::~TextDialog()
{
    instances--;
}

void TextDialog::action(const ActionEvent &event)
{
    if (event.getId() == "CANCEL")
        setActionEventId("~" + getActionEventId());

    distributeActionEvent();
    close();
}

const std::string &TextDialog::getText() const
{
    if (mTextField != nullptr)
        return mTextField->getText();
    else if (mPasswordField != nullptr)
        return mPasswordField->getText();
    return emptyStr;
}

void TextDialog::setText(const std::string &text)
{
    if (mTextField != nullptr)
        mTextField->setText(text);
    else if (mPasswordField != nullptr)
        mPasswordField->setText(text);
}

void TextDialog::close()
{
    keyboard.setEnabled(mEnabledKeyboard);
    scheduleDelete();
}
