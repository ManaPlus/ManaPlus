/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "logger.h"

#include "gui/charcreatedialog.h"
#include "gui/okdialog.h"

#include "net/net.h"

#include "net/ea/loginhandler.h"
#include "net/ea/eaprotocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern Net::CharHandler *charHandler;

namespace Ea
{

CharServerHandler::CharServerHandler()
{
}

void CharServerHandler::setCharSelectDialog(CharSelectDialog *window)
{
    mCharSelectDialog = window;
    updateCharSelectDialog();
}

void CharServerHandler::setCharCreateDialog(CharCreateDialog *window)
{
    mCharCreateDialog = window;

    if (!mCharCreateDialog)
        return;

    std::vector<std::string> attributes;
    attributes.push_back(_("Strength:"));
    attributes.push_back(_("Agility:"));
    attributes.push_back(_("Vitality:"));
    attributes.push_back(_("Intelligence:"));
    attributes.push_back(_("Dexterity:"));
    attributes.push_back(_("Luck:"));

    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    mCharCreateDialog->setAttributes(attributes, 30, 1, 9);
    mCharCreateDialog->setFixedGender(true, token.sex);
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
    return EA_SPRITE_HAIR;
}

unsigned int CharServerHandler::maxSprite() const
{
    return EA_SPRITE_VECTOREND;
}

void CharServerHandler::processCharLoginError(Net::MessageIn &msg)
{
    switch (msg.readInt8())
    {
        case 0:
            errorMessage = _("Access denied. Most likely, there are "
                             "too many players on this server.");
            break;
        case 1:
            errorMessage = _("Cannot use this ID.");
            break;
        default:
            errorMessage = _("Unknown char-server failure.");
            break;
    }
    Client::setState(STATE_ERROR);
}

void CharServerHandler::processCharCreate(Net::MessageIn &msg, bool withColors)
{
    Net::Character *character = new Net::Character;
    readPlayerData(msg, character, withColors);
    mCharacters.push_back(character);

    updateCharSelectDialog();

    // Close the character create dialog
    if (mCharCreateDialog)
    {
        mCharCreateDialog->scheduleDelete();
        mCharCreateDialog = 0;
    }
}

void CharServerHandler::processCharCreateFailed(Net::MessageIn &msg)
{
    switch (msg.readInt8())
    {
        case 1:
        case 0:
        default:
            errorMessage = _("Failed to create character. Most "
                                "likely the name is already taken.");
            break;
        case 2:
            errorMessage = _("Wrong name.");
            break;
        case 3:
            errorMessage = _("Incorrect stats.");
            break;
        case 4:
            errorMessage = _("Incorrect hair.");
            break;
        case 5:
            errorMessage = _("Incorrect slot.");
            break;
    }
    new OkDialog(_("Error"), errorMessage);
    if (mCharCreateDialog)
        mCharCreateDialog->unlock();
}

void CharServerHandler::processCharDelete(Net::MessageIn &msg A_UNUSED)
{
    delete mSelectedCharacter;
    mCharacters.remove(mSelectedCharacter);
    mSelectedCharacter = 0;
    updateCharSelectDialog();
    unlockCharSelectDialog();
    new OkDialog(_("Info"), _("Character deleted."));
}

void CharServerHandler::processCharDeleteFailed(Net::MessageIn &msg A_UNUSED)
{
    unlockCharSelectDialog();
    new OkDialog(_("Error"), _("Failed to delete character."));
}

} // namespace Ea
