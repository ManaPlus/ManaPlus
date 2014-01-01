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

#include "net/ea/npchandler.h"

#include "gui/windows/npcdialog.h"

#include "debug.h"

namespace Ea
{

NpcHandler::NpcHandler() :
    mDialog(nullptr)
{
}

void NpcHandler::sendLetter(const int npcId A_UNUSED,
                            const std::string &recipient A_UNUSED,
                            const std::string &text A_UNUSED) const
{
}

void NpcHandler::startShopping(const int beingId A_UNUSED) const
{
}

void NpcHandler::endShopping(const int beingId A_UNUSED) const
{
}

void NpcHandler::processNpcChoice(Net::MessageIn &msg)
{
    if (mDialog)
    {
        mDialog->choiceRequest();
        mDialog->parseListItems(msg.readString(msg.getLength() - 8));
    }
    else
    {
        msg.readString(msg.getLength() - 8);
    }
}

void NpcHandler::processNpcMessage(Net::MessageIn &msg)
{
    const std::string message = msg.readString(msg.getLength() - 8);
    // ignore future legacy npc commands.
    if (message.size() > 3 && message.substr(0, 3) == "###")
        return;
    if (mDialog)
        mDialog->addText(message);
}

void NpcHandler::processNpcClose(Net::MessageIn &msg A_UNUSED)
{
    // Show the close button
    if (mDialog)
        mDialog->showCloseButton();
}

void NpcHandler::processNpcNext(Net::MessageIn &msg A_UNUSED)
{
    // Show the next button
    if (mDialog)
        mDialog->showNextButton();
}

void NpcHandler::processNpcIntInput(Net::MessageIn &msg A_UNUSED)
{
    // Request for an integer
    if (mDialog)
        mDialog->integerRequest(0);
}

void NpcHandler::processNpcStrInput(Net::MessageIn &msg A_UNUSED)
{
    // Request for a string
    if (mDialog)
        mDialog->textRequest("");
}

}  // namespace Ea
