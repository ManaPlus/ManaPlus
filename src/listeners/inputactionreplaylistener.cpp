/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "listeners/inputactionreplaylistener.h"

#include "gui/windows/editdialog.h"

#include "input/inputmanager.h"

#include "debug.h"

InputActionReplayListener inputActionReplayListener;

InputActionReplayListener::InputActionReplayListener() :
    ActionListener(),
    mDialog(nullptr),
    mAction()
{
}

void InputActionReplayListener::openDialog(const std::string &caption,
                                           const std::string &text,
                                           const InputAction::Type action0)
{
    mDialog = new EditDialog(caption, text, "OK");
    mDialog->postInit();
    mDialog->addActionListener(this);
    mAction = action0;
}

void InputActionReplayListener::action(const ActionEvent &event)
{
    if (event.getId() != "OK")
    {
        mDialog = nullptr;
        mAction = InputAction::NO_VALUE;
        return;
    }
    if (mDialog)
    {
        inputManager.executeChatCommand(mAction,
            mDialog->getMsg(),
            nullptr);
        mDialog = nullptr;
    }
    mAction = InputAction::NO_VALUE;
}
