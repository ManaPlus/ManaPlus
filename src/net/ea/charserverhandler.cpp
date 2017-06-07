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

#include "net/ea/charserverhandler.h"

#include "net/character.h"
#include "net/serverinfo.h"

#include "net/ea/eaprotocol.h"

#include "utils/dtor.h"

#include "debug.h"



namespace Ea
{

extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    Net::CharServerHandler()
{
}

void CharServerHandler::setCharSelectDialog(CharSelectDialog *const window)
                                            const
{
    mCharSelectDialog = window;
    updateCharSelectDialog();
}

void CharServerHandler::requestCharacters() const
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

void CharServerHandler::clear() const
{
    delete_all(mCharacters);
    mCharacters.clear();
}

}  // namespace Ea
