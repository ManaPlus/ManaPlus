/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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
#include "soundmanager.h"

#include "const/sound.h"

#include "gui/dialogsmanager.h"
#include "gui/viewport.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/button.h"
#include "gui/widgets/radiobutton.h"

#include "net/charserverhandler.h"
#include "net/gamehandler.h"

#include "resources/map/map.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include "debug.h"

QuitDialog::QuitDialog(QuitDialog **const pointerToMe) :
    // TRANSLATORS: quit dialog name
    Window(_("Quit"), Modal_true, nullptr, "quit.xml"),
    ActionListener(),
    KeyListener(),
    mOptions(),
    // TRANSLATORS: quit dialog button
    mLogoutQuit(new RadioButton(this, _("Quit"), "quitdialog", false)),
    // TRANSLATORS: quit dialog button
    mForceQuit(new RadioButton(this, _("Quit"), "quitdialog", false)),
    mSwitchAccountServer(new RadioButton(this,
        // TRANSLATORS: quit dialog button
        _("Switch server"), "quitdialog", false)),
    mSwitchCharacter(new RadioButton(this,
        // TRANSLATORS: quit dialog button
        _("Switch character"), "quitdialog", false)),
    mRate(nullptr),
    // TRANSLATORS: quit dialog button
    mOkButton(new Button(this, _("OK"), "ok", BUTTON_SKIN, this)),
    // TRANSLATORS: quit dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", BUTTON_SKIN, this)),
    mMyPointer(pointerToMe),
    mNeedForceQuit(false)
{
    addKeyListener(this);

    ContainerPlacer placer = getPlacer(0, 0);
    const StateT state = client->getState();
    mNeedForceQuit = (state == State::CHOOSE_SERVER
        || state == State::CONNECT_SERVER || state == State::LOGIN
        || state == State::PRE_LOGIN || state == State::LOGIN_ATTEMPT
        || state == State::UPDATE || state == State::LOAD_DATA);

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
        if (state == State::GAME)
            placeOption(placer, mSwitchCharacter);
    }

/*
#ifdef ANDROID
    if (config.getBoolValue("rated") == false
        && config.getIntValue("gamecount") > 3)
    {
        // TRANSLATORS: rate button
        mRate = new RadioButton(this, _("Rate in google play"), "quitdialog",
            false);
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
    placer(1, 0, mOkButton, 1, 1);
    placer(2, 0, mCancelButton, 1, 1);
    reflowLayout(200, 0);
}

void QuitDialog::postInit()
{
    Window::postInit();
    setLocationRelativeTo(getParent());
    setVisible(Visible_true);
    soundManager.playGuiSound(SOUND_SHOW_WINDOW);
    requestModalFocus();
    mOkButton->requestFocus();
}

QuitDialog::~QuitDialog()
{
    if (mMyPointer != nullptr)
        *mMyPointer = nullptr;
    delete2(mForceQuit)
    delete2(mLogoutQuit)
    delete2(mSwitchAccountServer)
    delete2(mSwitchCharacter)
}

void QuitDialog::placeOption(ContainerPlacer &placer,
                             RadioButton *const option)
{
    placer(0, CAST_S32(mOptions.size()), option, 3, 1);
    mOptions.push_back(option);
}

void QuitDialog::action(const ActionEvent &event)
{
    soundManager.playGuiSound(SOUND_HIDE_WINDOW);
    if (event.getId() == "ok")
    {
        if (viewport != nullptr)
        {
            const Map *const map = viewport->getMap();
            if (map != nullptr)
                map->saveExtraLayer();
        }

        if (mForceQuit->isSelected())
        {
            client->setState(State::FORCE_QUIT);
        }
        else if (mLogoutQuit->isSelected())
        {
            DialogsManager::closeDialogs();
            client->setState(State::EXIT);
        }
        else if ((mRate != nullptr) && mRate->isSelected())
        {
            openBrowser("https://play.google.com/store/apps/details?"
                "id=org.evolonline.beta.manaplus");
            config.setValue("rated", true);
            if (mNeedForceQuit)
            {
                client->setState(State::FORCE_QUIT);
            }
            else
            {
                DialogsManager::closeDialogs();
                client->setState(State::EXIT);
            }
        }
        else if (gameHandler->isConnected()
                 && mSwitchAccountServer->isSelected())
        {
            DialogsManager::closeDialogs();
            client->setState(State::SWITCH_SERVER);
        }
        else if (mSwitchCharacter->isSelected())
        {
            if (client->getState() == State::GAME)
            {
                charServerHandler->switchCharacter();
                DialogsManager::closeDialogs();
                serverConfig.write();
            }
        }
    }
    scheduleDelete();
}

void QuitDialog::keyPressed(KeyEvent &event)
{
    const InputActionT actionId = event.getActionId();
    int dir = 0;

    PRAGMA45(GCC diagnostic push)
    PRAGMA45(GCC diagnostic ignored "-Wswitch-enum")
    switch (actionId)
    {
        case InputAction::GUI_SELECT:
        case InputAction::GUI_SELECT2:
            action(ActionEvent(nullptr, mOkButton->getActionEventId()));
            break;
        case InputAction::GUI_CANCEL:
            action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
            break;
        case InputAction::GUI_UP:
            dir = -1;
            break;
        case InputAction::GUI_DOWN:
            dir = 1;
            break;
        default:
            break;
    }
    PRAGMA45(GCC diagnostic pop)

    if (dir != 0)
    {
        STD_VECTOR<RadioButton*>::const_iterator it = mOptions.begin();
        const STD_VECTOR<RadioButton*>::const_iterator
            it_end = mOptions.end();

        for (; it < it_end; ++it)
        {
            if ((*it)->isSelected())
                break;
        }

        if (it == mOptions.end())
        {
            if (mOptions[0] != nullptr)
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
