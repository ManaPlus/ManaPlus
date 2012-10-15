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

#include "gui/worldselectdialog.h"

#include "client.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/sdlinput.h"

#include "gui/widgets/button.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

extern WorldInfo **server_info;

/**
 * The list model for the server list.
 */
class WorldListModel final : public gcn::ListModel
{
    public:
        WorldListModel(Worlds worlds):
                mWorlds(worlds)
        {
        }

        A_DELETE_COPY(WorldListModel)

        virtual ~WorldListModel()
        { }

        int getNumberOfElements()
        {
            return static_cast<int>(mWorlds.size());
        }

        std::string getElementAt(int i)
        {
            const WorldInfo *const si = mWorlds[i];
            if (si)
                return si->name + " (" + toString(si->online_users) + ")";
            else
                return "???";
        }
    private:
        Worlds mWorlds;
};

WorldSelectDialog::WorldSelectDialog(Worlds worlds):
    Window(_("Select World"), false, nullptr, "world.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
    mWorldListModel(new WorldListModel(worlds)),
    mWorldList(new ListBox(mWorldListModel)),
    mChangeLoginButton(new Button(_("Change Login"), "login", this)),
    mChooseWorld(new Button(_("Choose World"), "world", this))
{
    ScrollArea *const worldsScroll = new ScrollArea(mWorldList,
        getOptionBool("showbackground"), "world_background.xml");

    worldsScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

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
    setVisible(true);
    mChooseWorld->requestFocus();
}

WorldSelectDialog::~WorldSelectDialog()
{
    delete mWorldListModel;
    mWorldListModel = nullptr;
}

void WorldSelectDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "world")
    {
        mChangeLoginButton->setEnabled(false);
        mChooseWorld->setEnabled(false);
        Net::getLoginHandler()->chooseServer(mWorldList->getSelected());

        // Check in case netcode moves us forward
        if (Client::getState() == STATE_WORLD_SELECT)
            Client::setState(STATE_WORLD_SELECT_ATTEMPT);
    }
    else if (event.getId() == "login")
    {
        Client::setState(STATE_PRE_LOGIN);
    }
}

void WorldSelectDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    const int actionId = static_cast<KeyEvent*>(
        &keyEvent)->getActionId();

    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(gcn::ActionEvent(nullptr,
            mChangeLoginButton->getActionEventId()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(gcn::ActionEvent(nullptr, mChooseWorld->getActionEventId()));
    }
}
