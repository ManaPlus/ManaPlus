/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/windows/worldselectdialog.h"

#include "client.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/widgets/button.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "gui/models/worldlistmodel.h"

#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

extern WorldInfo **server_info;

WorldSelectDialog::WorldSelectDialog(Worlds worlds):
    // TRANSLATORS: world select dialog name
    Window(_("Select World"), false, nullptr, "world.xml"),
    ActionListener(),
    KeyListener(),
    mWorldListModel(new WorldListModel(worlds)),
    mWorldList(new ListBox(this, mWorldListModel, "")),
    // TRANSLATORS: world dialog button
    mChangeLoginButton(new Button(this, _("Change Login"), "login", this)),
    // TRANSLATORS: world dialog button
    mChooseWorld(new Button(this, _("Choose World"), "world", this))
{
    mWorldList->postInit();
    ScrollArea *const worldsScroll = new ScrollArea(this, mWorldList,
        getOptionBool("showbackground"), "world_background.xml");

    worldsScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    place(0, 0, worldsScroll, 3, 5).setPadding(2);
    place(1, 5, mChangeLoginButton);
    place(2, 5, mChooseWorld);

    // Make sure the list has enough height
    getLayout().setRowHeight(0, 60);

    reflowLayout(0, 0);

    if (worlds.empty())
    {
        // Disable Ok button
        mChooseWorld->setEnabled(false);
    }
    else
    {
        // Select first server
        mWorldList->setSelected(0);
    }

    addKeyListener(this);

    center();
}

void WorldSelectDialog::postInit()
{
    setVisible(true);
    mChooseWorld->requestFocus();
}

WorldSelectDialog::~WorldSelectDialog()
{
    delete mWorldListModel;
    mWorldListModel = nullptr;
}

void WorldSelectDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "world")
    {
        mChangeLoginButton->setEnabled(false);
        mChooseWorld->setEnabled(false);
        Net::getLoginHandler()->chooseServer(mWorldList->getSelected());

        // Check in case netcode moves us forward
        if (client->getState() == STATE_WORLD_SELECT)
            client->setState(STATE_WORLD_SELECT_ATTEMPT);
    }
    else if (eventId == "login")
    {
        client->setState(STATE_PRE_LOGIN);
    }
}

void WorldSelectDialog::keyPressed(KeyEvent &keyEvent)
{
    const int actionId = keyEvent.getActionId();

    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(ActionEvent(nullptr,
            mChangeLoginButton->getActionEventId()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(ActionEvent(nullptr, mChooseWorld->getActionEventId()));
    }
}
