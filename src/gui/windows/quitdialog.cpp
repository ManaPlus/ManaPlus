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

#include "gui/windows/quitdialog.h"

#include "client.h"
#include "configuration.h"
#include "game.h"
#include "soundconsts.h"
#include "soundmanager.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/viewport.h"

#include "gui/widgets/layout.h"
#include "gui/widgets/button.h"
#include "gui/widgets/radiobutton.h"

#include "net/charserverhandler.h"
#include "net/gamehandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/process.h"

#include "debug.h"

QuitDialog::QuitDialog(QuitDialog **const pointerToMe):
    // TRANSLATORS: quit dialog name
    Window(_("Quit"), true, nullptr, "quit.xml"),
    ActionListener(),
    KeyListener(),
    mOptions(),
    // TRANSLATORS: quit dialog button
    mLogoutQuit(new RadioButton(this, _("Quit"), "quitdialog")),
    // TRANSLATORS: quit dialog button
    mForceQuit(new RadioButton(this, _("Quit"), "quitdialog")),
    mSwitchAccountServer(new RadioButton(this,
        // TRANSLATORS: quit dialog button
        _("Switch server"), "quitdialog")),
    mSwitchCharacter(new RadioButton(this,
        // TRANSLATORS: quit dialog button
        _("Switch character"), "quitdialog")),
    mRate(nullptr),
    // TRANSLATORS: quit dialog button
    mOkButton(new Button(this, _("OK"), "ok", this)),
    // TRANSLATORS: quit dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mMyPointer(pointerToMe),
    mNeedForceQuit(false)
{
    addKeyListener(this);

    ContainerPlacer placer = getPlacer(0, 0);
    const State state = client->getState();
    mNeedForceQuit = (state == STATE_CHOOSE_SERVER
        || state == STATE_CONNECT_SERVER || state == STATE_LOGIN
        || state == STATE_PRE_LOGIN || state == STATE_LOGIN_ATTEMPT
        || state == STATE_UPDATE || state == STATE_LOAD_DATA);

    // All states, when we're not logged in to someone.
    if (mNeedForceQuit)
    {
        placeOption(placer, mForceQuit);
    }
    else
    {
        // Only added if we are connected to an accountserver or gameserver
        placeOption(placer, mLogoutQuit);
        placeOption(placer, mSwitchAccountServer);

        // Only added if we are connected to a gameserver
        if (state == STATE_GAME)
            placeOption(placer, mSwitchCharacter);
    }

/*
#ifdef ANDROID
    if (config.getBoolValue("rated") == false
        && config.getIntValue("gamecount") > 3)
    {
        mRate = new RadioButton(this, _("Rate in google play"), "quitdialog");
        placeOption(placer, mRate);
        mOptions[mOptions.size() - 1]->setSelected(true);
    }
    else
#endif
*/
    {
        mOptions[0]->setSelected(true);
    }

    placer = getPlacer(0, 1);
    placer(1, 0, mOkButton, 1);
    placer(2, 0, mCancelButton, 1);
    reflowLayout(200, 0);
}

void QuitDialog::postInit()
{
    setLocationRelativeTo(getParent());
    setVisible(true);
    soundManager.playGuiSound(SOUND_SHOW_WINDOW);
    requestModalFocus();
    mOkButton->requestFocus();
}

QuitDialog::~QuitDialog()
{
    if (mMyPointer)
        *mMyPointer = nullptr;
    delete2(mForceQuit);
    delete2(mLogoutQuit);
    delete2(mSwitchAccountServer);
    delete2(mSwitchCharacter);
}

void QuitDialog::placeOption(ContainerPlacer &placer,
                             RadioButton *const option)
{
    placer(0, static_cast<int>(mOptions.size()), option, 3);
    mOptions.push_back(option);
}

void QuitDialog::action(const ActionEvent &event)
{
    soundManager.playGuiSound(SOUND_HIDE_WINDOW);
    if (event.getId() == "ok")
    {
        if (viewport)
        {
            const Map *const map = viewport->getMap();
            if (map)
                map->saveExtraLayer();
        }

        if (mForceQuit->isSelected())
        {
            client->setState(STATE_FORCE_QUIT);
        }
        else if (mLogoutQuit->isSelected())
        {
            Game::closeDialogs();
            client->setState(STATE_EXIT);
        }
        else if (mRate && mRate->isSelected())
        {
            openBrowser("https://play.google.com/store/apps/details?"
                "id=org.evolonline.beta.manaplus");
            config.setValue("rated", true);
            if (mNeedForceQuit)
            {
                client->setState(STATE_FORCE_QUIT);
            }
            else
            {
                Game::closeDialogs();
                client->setState(STATE_EXIT);
            }
        }
        else if (Net::getGameHandler()->isConnected()
                 && mSwitchAccountServer->isSelected())
        {
            Game::closeDialogs();
            client->setState(STATE_SWITCH_SERVER);
        }
        else if (mSwitchCharacter->isSelected())
        {
            if (client->getState() == STATE_GAME)
            {
                Net::getCharServerHandler()->switchCharacter();
                Game::closeDialogs();
                serverConfig.write();
            }
        }
    }
    scheduleDelete();
}

void QuitDialog::keyPressed(KeyEvent &event)
{
    const int actionId = event.getActionId();
    int dir = 0;

    switch (actionId)
    {
        case Input::KEY_GUI_SELECT:
        case Input::KEY_GUI_SELECT2:
            action(ActionEvent(nullptr, mOkButton->getActionEventId()));
            break;
        case Input::KEY_GUI_CANCEL:
            action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
            break;
        case Input::KEY_GUI_UP:
            dir = -1;
            break;
        case Input::KEY_GUI_DOWN:
            dir = 1;
            break;
        default:
            break;
    }

    if (dir != 0)
    {
        std::vector<RadioButton*>::const_iterator it = mOptions.begin();
        const std::vector<RadioButton*>::const_iterator
            it_end = mOptions.end();

        for (; it < it_end; ++it)
        {
            if ((*it)->isSelected())
                break;
        }

        if (it == mOptions.end())
        {
            if (mOptions[0])
                mOptions[0]->setSelected(true);
            return;
        }
        else if (it == mOptions.begin() && dir < 0)
        {
            it = mOptions.end();
        }

        it += dir;

        if (it == mOptions.end())
            it = mOptions.begin();

        (*it)->setSelected(true);
    }
}
