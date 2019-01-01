/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "listeners/playerlistener.h"

#include "actormanager.h"

#include "being/being.h"

#include "gui/windows/textdialog.h"

#include "debug.h"

PlayerListener::PlayerListener() :
    ActionListener(),
    mNick(),
    mDialog(nullptr),
    mType(ActorType::Unknown)
{
}

void PlayerListener::action(const ActionEvent &event)
{
    if (event.getId() == "ok" && !mNick.empty() && (mDialog != nullptr))
    {
        std::string comment = mDialog->getText();
        Being *const being  = actorManager->findBeingByName(
            mNick, static_cast<ActorTypeT>(mType));
        if (being != nullptr)
            being->setComment(comment);
        Being::saveComment(mNick, comment, mType);
    }
    mDialog = nullptr;
}
