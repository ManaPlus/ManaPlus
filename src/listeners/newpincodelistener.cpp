/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "listeners/newpincodelistener.h"

#include "client.h"
#include "pincodemanager.h"

#include "gui/windows/pincodedialog.h"

#include "debug.h"

NewPincodeListener newPincodeListener;

void NewPincodeListener::action(const ActionEvent &event)
{
    if (event.getId() == "ok")
    {
        const PincodeDialog *const dialog = dynamic_cast<PincodeDialog*>(
            event.getSource());
        if (dialog != nullptr)
        {
            const std::string pincode = dialog->getMsg();
            pincodeManager.setNewPincode(pincode);
        }
    }
    else
    {
        client->setState(State::SWITCH_LOGIN);
    }
}
