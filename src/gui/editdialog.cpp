/*
 *  The Mana World
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *
 *  This file is part of The Mana World.
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gui/editdialog.h"

#include "gui/gui.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

EditDialog::EditDialog(const std::string &title, const std::string &msg,
                       std::string eventOk, int width,
                       Window *parent, bool modal):
    Window(title, modal, parent)
{
    mTextField = new TextField;
    mTextField->setText(msg);

    mEventOk = eventOk;

    gcn::Button *okButton = new Button(_("OK"), mEventOk, this);

    const int numRows = 1;
    const int fontHeight = getFont()->getHeight();
    const int height = numRows * fontHeight;

    setContentSize(width, height + fontHeight + okButton->getHeight());
    mTextField->setPosition(getPadding(), getPadding());
    mTextField->setWidth(width - (2 * getPadding()));

    okButton->setPosition((width - okButton->getWidth()) / 2, height + 8);

    add(mTextField);
    add(okButton);

    center();
    setVisible(true);
    okButton->requestFocus();
}

void EditDialog::action(const gcn::ActionEvent &event)
{
    // Proxy button events to our listeners
    ActionListenerIterator i;
    for (i = mActionListeners.begin(); i != mActionListeners.end(); ++i)
        (*i)->action(event);

    if (event.getId() == mEventOk)
        scheduleDelete();
}
