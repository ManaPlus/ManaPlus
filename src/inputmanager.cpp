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
#include "localplayer.h"

#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "gui/inventorywindow.h"
#include "gui/npcdialog.h"
#include "gui/setup.h"
#include "gui/tradewindow.h"

#include <guichan/focushandler.hpp>

#include "debug.h"

InputManager inputManager;

InputManager::InputManager()
{
}

bool InputManager::handleKeyEvent(SDL_Event &event)
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

    if (!keyboard.isActionActive(keyboard.KEY_TARGET)
        && !keyboard.isActionActive(keyboard.KEY_UNTARGET))
    {
        mask += COND_NOTARGET;
    }
    return mask;
}

bool InputManager::checkKey(KeyFunction *key, int mask)
{
//    logger->log("mask=%d, condition=%d", mask, key->condition);
    if (!key || (key->condition & mask) != key->condition)
        return false;

    return (key->modKeyIndex == KeyboardConfig::KEY_NO_VALUE
        || keyboard.isActionActive(key->modKeyIndex));
}
