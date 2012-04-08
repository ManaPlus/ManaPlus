/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "inputmanager.h"

#include "game.h"
#include "keyboardconfig.h"
#include "keydata.h"
#include "localplayer.h"

#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/inventorywindow.h"
#include "gui/npcdialog.h"
#include "gui/npcpostdialog.h"
#include "gui/setup.h"
#include "gui/textdialog.h"
#include "gui/tradewindow.h"

#include <guichan/exception.hpp>
#include <guichan/focushandler.hpp>

#include "debug.h"

InputManager inputManager;

extern QuitDialog *quitDialog;

InputManager::InputManager()
{
}

bool InputManager::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (setupWindow && setupWindow->isVisible() &&
            keyboard.getNewKeyIndex() > Input::KEY_NO_VALUE)
        {
            keyboard.setNewKey(event);
            keyboard.callbackNewKey();
            keyboard.setNewKeyIndex(Input::KEY_NO_VALUE);
            return true;
        }

        // send straight to gui for certain windows
        if (quitDialog || TextDialog::isActive() ||
            NpcPostDialog::isActive())
        {
            try
            {
                if (guiInput)
                    guiInput->pushInput(event);
                if (gui)
                    gui->handleInput();
            }
            catch (const gcn::Exception &e)
            {
                const char* err = e.getMessage().c_str();
                logger->log("Warning: guichan input exception: %s", err);
            }
            return true;
        }
    }

    try
    {
        if (guiInput)
            guiInput->pushInput(event);
    }
    catch (const gcn::Exception &e)
    {
        const char *err = e.getMessage().c_str();
        logger->log("Warning: guichan input exception: %s", err);
    }
    if (gui)
    {
        bool res = gui->handleInput();
        if (res && event.type == SDL_KEYDOWN)
            return true;
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (handleKeyEvent(event))
            return true;
    }

    return false;
}

bool InputManager::handleKeyEvent(const SDL_Event &event)
{
    return keyboard.triggerAction(event);
}

int InputManager::getInputConditionMask()
{
    int mask = 1;
    if (keyboard.isEnabled())
        mask += COND_ENABLED;
    if ((!chatWindow || !chatWindow->isInputFocused()) &&
        !NpcDialog::isAnyInputFocused() &&
        !InventoryWindow::isAnyInputFocused() &&
        (!tradeWindow || !tradeWindow->isInpupFocused()))
    {
        mask += COND_NOINPUT;
    }

    if (!player_node || !player_node->getAway())
        mask += COND_NOAWAY;

    if (!setupWindow || !setupWindow->isVisible())
        mask += COND_NOSETUP;

    if (Game::instance()->getValidSpeed())
        mask += COND_VALIDSPEED;

    if (!gui->getFocusHandler()->getModalFocused())
        mask += COND_NOMODAL;

    NpcDialog *dialog = NpcDialog::getActive();
    if (!dialog || !dialog->isTextInputFocused())
        mask += COND_NONPCINPUT;

    if (!player_node || !player_node->getDisableGameModifiers())
        mask += COND_EMODS;

    if (!keyboard.isActionActive(Input::KEY_TARGET)
        && !keyboard.isActionActive(Input::KEY_UNTARGET))
    {
        mask += COND_NOTARGET;
    }
    return mask;
}

bool InputManager::checkKey(const KeyData *key, int mask)
{
//    logger->log("mask=%d, condition=%d", mask, key->condition);
    if (!key || (key->condition & mask) != key->condition)
        return false;

    return (key->modKeyIndex == Input::KEY_NO_VALUE
        || keyboard.isActionActive(key->modKeyIndex));
}
