/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/windows/npcpostdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/textfield.h"
#include "gui/widgets/scrollarea.h"

#include "gui/widgets/tabs/chattab.h"

#include "net/net.h"
#include "net/npchandler.h"

#include "utils/gettext.h"

#include "debug.h"

NpcPostDialog::DialogList NpcPostDialog::instances;

NpcPostDialog::NpcPostDialog(const int npcId):
    // TRANSLATORS: npc post dialog caption
    Window(_("NPC"), false, nullptr, "npcpost.xml"),
    gcn::ActionListener(),
    mNpcId(npcId),
    mText(new TextBox(this)),
    mSender(new TextField(this))
{
    setContentSize(400, 180);
}

void NpcPostDialog::postInit()
{
    // create text field for receiver
    // TRANSLATORS: label in npc post dialog
    Label *const senderText = new Label(this, _("To:"));
    senderText->setPosition(5, 5);
    mSender->setPosition(senderText->getWidth() + 5, 5);
    mSender->setWidth(65);

    // create button for sending
    // TRANSLATORS: button in npc post dialog
    Button *const sendButton = new Button(this, _("Send"), "send", this);
    sendButton->setPosition(400 - sendButton->getWidth(),
                            170 - sendButton->getHeight());
    // TRANSLATORS: button in npc post dialog
    Button *const cancelButton = new Button(this, _("Cancel"), "cancel", this);
    cancelButton->setPosition(sendButton->getX()
        - (cancelButton->getWidth() + 2), sendButton->getY());

    // create textfield for letter
    mText->setHeight(400 - (mSender->getHeight() + sendButton->getHeight()));
    mText->setEditable(true);

    // create scroll box for letter text
    ScrollArea *const scrollArea = new ScrollArea(mText);
    scrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    scrollArea->setDimension(gcn::Rectangle(
                5, mSender->getHeight() + 5,
                380, 140 - (mSender->getHeight() + sendButton->getHeight())));

    add(senderText);
    add(mSender);
    add(scrollArea);
    add(sendButton);
    add(cancelButton);

    setLocationRelativeTo(getParent());

    instances.push_back(this);
    setVisible(true);
    enableVisibleSound(true);
}

NpcPostDialog::~NpcPostDialog()
{
    instances.remove(this);
}

void NpcPostDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "send")
    {
        if (mSender->getText().empty() || mText->getText().empty())
        {
            if (localChatTab)
            {
                // TRANSLATORS: npc post message error
                localChatTab->chatLog(_("Failed to send as sender or letter "
                    "invalid."));
            }
        }
        else
        {
            Net::getNpcHandler()->sendLetter(mNpcId, mSender->getText(),
                                             mText->getText());
        }
        setVisible(false);
    }
    else if (eventId == "cancel")
    {
        setVisible(false);
    }
}

void NpcPostDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (!visible)
        scheduleDelete();
}

void NpcPostDialog::closeAll()
{
    FOR_EACH (DialogList::const_iterator, it, instances)
        (*it)->close();
}
