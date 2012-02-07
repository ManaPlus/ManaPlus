/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "sound.h"

#include "gui/gui.h"

#include "gui/widgets/button.h"
#include "gui/widgets/textbox.h"

#include "utils/gettext.h"

#include <guichan/font.hpp>

#include "debug.h"

ConfirmDialog::ConfirmDialog(const std::string &title, const std::string &msg,
                             bool ignore, bool modal, Window *parent):
    Window(title, modal, parent, "confirm.xml")
{
    mTextBox = new TextBox;
    mTextBox->setEditable(false);
    mTextBox->setOpaque(false);
    mTextBox->setTextWrapped(msg, 260);

    gcn::Button *yesButton = new Button(_("Yes"), "yes", this);
    gcn::Button *noButton = new Button(_("No"), "no", this);
    gcn::Button *ignoreButton = nullptr;

    if (ignore)
        ignoreButton = new Button(_("Ignore"), "ignore", this);

    const int numRows = mTextBox->getNumberOfRows();
    int inWidth = yesButton->getWidth() + noButton->getWidth() + 
        (2 * getPadding());

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
    mTextBox->setPosition(getPadding(), getPadding());

    // 8 is the padding that GUIChan adds to button widgets
    // (top and bottom combined)
    yesButton->setPosition((width - inWidth) / 2, height + 8);
    noButton->setPosition(yesButton->getX() + yesButton->getWidth()
                          + (2 * getPadding()),
                          height + 8);
    if (ignoreButton)
    {
        ignoreButton->setPosition(noButton->getX() + noButton->getWidth()
                                  + (2 * getPadding()), height + 8);
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
    sound.playGuiSfx("system/newmessage.ogg");
}

void ConfirmDialog::action(const gcn::ActionEvent &event)
{
    setActionEventId(event.getId());
    distributeActionEvent();

    // Can we receive anything else anyway?
    if (event.getId() == "yes" || event.getId() == "no"
        || event.getId() == "ignore")
    {
        scheduleDelete();
    }
}
