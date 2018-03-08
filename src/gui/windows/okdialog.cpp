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

#include "gui/windows/okdialog.h"

#include "soundmanager.h"

#include "const/sound.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textbox.h"

#include "gui/fonts/font.h"

#include "debug.h"

OkDialog::OkDialog(const std::string &restrict title,
                   const std::string &restrict msg,
                   const std::string &restrict button,
                   const DialogTypeT soundEvent,
                   const Modal modal,
                   const ShowCenter showCenter,
                   Window *const parent,
                   const int minWidth) :
    Window(title, modal, parent, "ok.xml"),
    ActionListener(),
    mTextBox(new TextBox(this))
{
    mTextBox->setEditable(false);
    mTextBox->setOpaque(Opaque_false);
    mTextBox->setTextWrapped(msg, minWidth);

    // TRANSLATORS: ok dialog button
    Button *const okButton = new Button(this,
        button,
        "ok",
        BUTTON_SKIN,
        this);

    int width = getFont()->getWidth(title);
    if (width < mTextBox->getMinWidth())
        width = mTextBox->getMinWidth();
    if (width < okButton->getWidth())
        width = okButton->getWidth();

    if (mTextBox->getWidth() > width)
        width = mTextBox->getWidth();
    if (okButton->getWidth() > width)
        width = okButton->getWidth();
    setContentSize(width, mTextBox->getHeight() + okButton->getHeight()
        + getOption("buttonPadding", 8));
    mTextBox->setPosition((width - mTextBox->getWidth()) / 2, 0);
    okButton->setPosition((width - okButton->getWidth()) / 2,
        mTextBox->getHeight() + getOption("buttonPadding", 8));

    // +++ virtual method call
    add(mTextBox);
    add(okButton);

    if (showCenter == ShowCenter_true)
        center();
    else
        centerHorisontally();
    // +++ virtual method call
    setVisible(Visible_true);
    okButton->requestFocus();

    if (soundEvent == DialogType::OK)
        soundManager.playGuiSound(SOUND_INFO);
    else if (soundEvent == DialogType::ERROR)
        soundManager.playGuiSound(SOUND_ERROR);
}

void OkDialog::action(const ActionEvent &event)
{
    setActionEventId(event.getId());
    distributeActionEvent();
    scheduleDelete();
}
