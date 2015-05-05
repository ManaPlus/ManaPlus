/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/ea/charserverhandler.h"

#include "client.h"

#include "enums/gui/dialogtype.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/okdialog.h"

#include "net/ea/eaprotocol.h"

#include "net/character.h"
#include "net/messagein.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace Ea
{

extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    Net::CharServerHandler()
{
}

void CharServerHandler::setCharSelectDialog(CharSelectDialog *const window)
{
    mCharSelectDialog = window;
    updateCharSelectDialog();
}

void CharServerHandler::requestCharacters()
{
    connect();
}

unsigned int CharServerHandler::baseSprite() const
{
    return EA_SPRITE_BASE;
}

unsigned int CharServerHandler::hairSprite() const
{
    return EA_SPRITE_HAIR_COLOR;
}

unsigned int CharServerHandler::maxSprite() const
{
    return EA_SPRITE_VECTOREND;
}

void CharServerHandler::processCharLoginError(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharLoginError")
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
    client->setState(STATE_ERROR);
    BLOCK_END("CharServerHandler::processCharLoginError")
}

void CharServerHandler::processCharCreateFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerHandler::processCharCreateFailed")
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
    // TRANSLATORS: error message header
    new OkDialog(_("Error"), errorMessage,
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        Modal_true,
        true,
        nullptr,
        260);
    if (mCharCreateDialog)
        mCharCreateDialog->unlock();
    BLOCK_END("CharServerHandler::processCharCreateFailed")
}

void CharServerHandler::processCharDelete(Net::MessageIn &msg A_UNUSED)
{
    BLOCK_START("CharServerHandler::processCharDelete")
    delete mSelectedCharacter;
    mCharacters.remove(mSelectedCharacter);
    mSelectedCharacter = nullptr;
    updateCharSelectDialog();
    unlockCharSelectDialog();
    // TRANSLATORS: info message
    new OkDialog(_("Info"), _("Character deleted."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::OK,
        Modal_true,
        true,
        nullptr,
        260);
    BLOCK_END("CharServerHandler::processCharDelete")
}

void CharServerHandler::clear()
{
    delete_all(mCharacters);
    mCharacters.clear();
}

}  // namespace Ea
