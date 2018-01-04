/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/windows/confirmdialog.h"

#include "soundmanager.h"

#include "gui/fonts/font.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include "debug.h"

ConfirmDialog::ConfirmDialog(const std::string &restrict title,
                             const std::string &restrict msg,
                             const std::string &restrict soundEvent,
                             const bool ignore,
                             const Modal modal,
                             Window *const parent) :
    Window(title, modal, parent, "confirm.xml"),
    ActionListener(),
    mTextBox(new TextBox(this)),
    mIgnore(ignore)
{
    mTextBox->setEditable(false);
    mTextBox->setOpaque(Opaque_false);
    mTextBox->setTextWrapped(msg, 260);
    soundManager.playGuiSound(soundEvent);
}

void ConfirmDialog::postInit()
{
    Window::postInit();
    // TRANSLATORS: confirm dialog button
    Button *const yesButton = new Button(this, _("Yes"), "yes", this);
    // TRANSLATORS: confirm dialog button
    Button *const noButton = new Button(this, _("No"), "no", this);
    Button *const ignoreButton = mIgnore ? new Button(
        // TRANSLATORS: confirm dialog button
        this, _("Ignore"), "ignore", this) : nullptr;

    const int numRows = mTextBox->getNumberOfRows();
    int inWidth = yesButton->getWidth() + noButton->getWidth() +
        (2 * mPadding);

    if (ignoreButton != nullptr)
        inWidth += ignoreButton->getWidth();

    const int fontHeight = getFont()->getHeight();
    const int height = numRows * fontHeight;
    int width = getFont()->getWidth(getCaption()) + mCaptionOffsetX;

    if (width < mTextBox->getMinWidth())
        width = mTextBox->getMinWidth();
    if (width < inWidth)
        width = inWidth;

    int windowWidth = width;
    if (windowWidth < mTextBox->getMinWidth() + fontHeight)
    {
        windowWidth = mTextBox->getMinWidth() + fontHeight;
    }
    setContentSize(windowWidth,
        height + fontHeight + noButton->getHeight());
    mTextBox->setPosition(mPadding, mPadding);

    const int buttonPadding = getOption("buttonPadding", 8);
    yesButton->setPosition((width - inWidth) / 2, height + buttonPadding);
    noButton->setPosition(yesButton->getX() + yesButton->getWidth()
        + (2 * mPadding), height + buttonPadding);
    if (ignoreButton != nullptr)
    {
        ignoreButton->setPosition(noButton->getX() + noButton->getWidth()
            + (2 * mPadding), height + buttonPadding);
    }

    add(mTextBox);
    add(yesButton);
    add(noButton);

    if (mIgnore && (ignoreButton != nullptr))
        add(ignoreButton);

    if (getParent() != nullptr)
    {
        center();
        getParent()->moveToTop(this);
    }
    setVisible(Visible_true);
    yesButton->requestFocus();
}

void ConfirmDialog::action(const ActionEvent &event)
{
    setActionEventId(event.getId());
    distributeActionEvent();
    scheduleDelete();
}
