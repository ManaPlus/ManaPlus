/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/ea/charserverrecv.h"

#include "client.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/character.h"
#include "net/charserverhandler.h"
#include "net/messagein.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

void CharServerRecv::processCharLoginError(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharLoginError")
    switch (msg.readUInt8("error"))
    {
        case 0:
            // TRANSLATORS: error message
            errorMessage = _("Access denied. Most likely, there are "
                             "too many players on this server.");
            break;
        case 1:
            // TRANSLATORS: error message
            errorMessage = _("Cannot use this ID.");
            break;
        default:
            // TRANSLATORS: error message
            errorMessage = _("Unknown char-server failure.");
            break;
    }
    client->setState(State::ERROR);
    BLOCK_END("CharServerRecv::processCharLoginError")
}

void CharServerRecv::processCharCreateFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharCreateFailed")
    switch (msg.readUInt8("error"))
    {
        case 1:
        case 0:
        default:
            // TRANSLATORS: error message
            errorMessage = _("Failed to create character. Most "
                "likely the name is already taken.");
            break;
        case 2:
            // TRANSLATORS: error message
            errorMessage = _("Wrong name.");
            break;
        case 3:
            // TRANSLATORS: error message
            errorMessage = _("Incorrect stats.");
            break;
        case 4:
            // TRANSLATORS: error message
            errorMessage = _("Incorrect hair.");
            break;
        case 5:
            // TRANSLATORS: error message
            errorMessage = _("Incorrect slot.");
            break;
        case 6:
            // TRANSLATORS: error message
            errorMessage = _("Incorrect race.");
            break;
        case 7:
            // TRANSLATORS: error message
            errorMessage = _("Incorrect look.");
            break;
    }
    CREATEWIDGET(OkDialog,
        // TRANSLATORS: error message header
        _("Error"),
        errorMessage,
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        Modal_true,
        ShowCenter_true,
        nullptr,
        260);
    if (charServerHandler->mCharCreateDialog)
        charServerHandler->mCharCreateDialog->unlock();
    BLOCK_END("CharServerRecv::processCharCreateFailed")
}

void CharServerRecv::processCharDelete(Net::MessageIn &msg A_UNUSED)
{
    BLOCK_START("CharServerRecv::processCharDelete")
    delete charServerHandler->mSelectedCharacter;
    charServerHandler->mCharacters.remove(
        charServerHandler->mSelectedCharacter);
    charServerHandler->mSelectedCharacter = nullptr;
    charServerHandler->updateCharSelectDialog();
    charServerHandler->unlockCharSelectDialog();
    CREATEWIDGET(OkDialog,
        // TRANSLATORS: info message header
        _("Info"),
        // TRANSLATORS: info message
        _("Character deleted."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::OK,
        Modal_true,
        ShowCenter_true,
        nullptr,
        260);
    BLOCK_END("CharServerRecv::processCharDelete")
}

}  // namespace Ea
