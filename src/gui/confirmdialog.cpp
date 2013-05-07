/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/confirmdialog.h"

#include "soundmanager.h"

#include "gui/gui.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

ConfirmDialog::ConfirmDialog(const std::string &title, const std::string &msg,
                             const bool ignore, const bool modal,
                             Window *const parent):
    Window(title, modal, parent, "confirm.xml"),
    gcn::ActionListener(),
    mTextBox(new TextBox(this))
{
    mTextBox->setEditable(false);
    mTextBox->setOpaque(false);
    mTextBox->setTextWrapped(msg, 260);

    // TRANSLATORS: confirm dialog button
    Button *const yesButton = new Button(this, _("Yes"), "yes", this);
    // TRANSLATORS: confirm dialog button
    Button *const noButton = new Button(this, _("No"), "no", this);
    Button *const ignoreButton = ignore ? new Button(
        // TRANSLATORS: confirm dialog button
        this, _("Ignore"), "ignore", this) : nullptr;

    const int numRows = mTextBox->getNumberOfRows();
    int inWidth = yesButton->getWidth() + noButton->getWidth() +
        (2 * mPadding);

    if (ignoreButton)
        inWidth += ignoreButton->getWidth();

    const int fontHeight = getFont()->getHeight();
    const int height = numRows * fontHeight;
    int width = getFont()->getWidth(title);

    if (width < mTextBox->getMinWidth())
        width = mTextBox->getMinWidth();
    if (width < inWidth)
        width = inWidth;

    setContentSize(mTextBox->getMinWidth() + fontHeight, height + fontHeight +
                   noButton->getHeight());
    mTextBox->setPosition(mPadding, mPadding);

    // 8 is the padding that GUIChan adds to button widgets
    // (top and bottom combined)
    const int buttonPadding = getOption("buttonPadding", 8);
    yesButton->setPosition((width - inWidth) / 2, height + buttonPadding);
    noButton->setPosition(yesButton->getX() + yesButton->getWidth()
        + (2 * mPadding), height + buttonPadding);
    if (ignoreButton)
    {
        ignoreButton->setPosition(noButton->getX() + noButton->getWidth()
            + (2 * mPadding), height + buttonPadding);
    }

    add(mTextBox);
    add(yesButton);
    add(noButton);

    if (ignore && ignoreButton)
        add(ignoreButton);

    if (getParent())
    {
        center();
        getParent()->moveToTop(this);
    }
    setVisible(true);
    yesButton->requestFocus();
    soundManager.playGuiSound(SOUND_REQUEST);
}

void ConfirmDialog::action(const gcn::ActionEvent &event)
{
    setActionEventId(event.getId());
    distributeActionEvent();
    scheduleDelete();
}
