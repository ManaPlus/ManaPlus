/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#include "gui/editdialog.h"

#include "gui/gui.h"

#include "gui/widgets/button.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

EditDialog::EditDialog(const std::string &title, const std::string &msg,
                       const std::string &eventOk, const int width,
                       Window *const parent, const bool modal):
    Window(title, modal, parent, "edit.xml"),
    gcn::ActionListener(),
    mEventOk(eventOk),
    mTextField(new TextField(this))
{
    mTextField->setText(msg);
    // TRANSLATORS: edit dialog label
    Button *const okButton = new Button(this, _("OK"), mEventOk, this);

    const int numRows = 1;
    const int fontHeight = getFont()->getHeight();
    const int height = numRows * fontHeight;

    setContentSize(width, height + fontHeight + okButton->getHeight());
    mTextField->setPosition(getPadding(), getPadding());
    mTextField->setWidth(width - (2 * getPadding()));

    okButton->setPosition((width - okButton->getWidth()) / 2,
        height + getOption("buttonPadding", 8));

    add(mTextField);
    add(okButton);

    center();
    setVisible(true);
    okButton->requestFocus();
}

void EditDialog::action(const gcn::ActionEvent &event)
{
    // Proxy button events to our listeners
    FOR_EACH (ActionListenerIterator, i, mActionListeners)
        (*i)->action(event);

    if (event.getId() == mEventOk)
        scheduleDelete();
}
