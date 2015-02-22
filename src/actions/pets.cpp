/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#include "actions/pets.h"

#include "actormanager.h"
#include "game.h"
#include "emoteshortcut.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"

#include "gui/chatconsts.h"

#include "net/chathandler.h"
#include "net/pethandler.h"
#include "net/serverfeatures.h"

#include "utils/chatutils.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace Actions
{

static const Being *getPet()
{
    if (!localPlayer)
        return nullptr;

    const std::vector<Being*> &pets = localPlayer->getPets();
    if (pets.empty())
        return nullptr;
    return *pets.begin();
}

impHandler(commandEmotePet)
{
    // need use actual pet id
    petHandler->emote(static_cast<uint8_t>(
        atoi(event.args.c_str())), 0);
    return true;
}

impHandler(talkPet)
{
    if (!serverFeatures->haveTalkPet())
        return false;

    std::string args = event.args;
    if (findCutFirst(args, "/me "))
        args = textToMe(args);
    chatHandler->talkPet(args, GENERAL_CHANNEL);
    return true;
}

impHandler(setPetName)
{
    const std::string args = event.args;
    if (args.empty())
        return false;

    petHandler->setName(args);
    return true;
}

impHandler(petEmote)
{
    if (!serverFeatures->haveTalkPet())
        return false;

    if (event.action >= InputAction::PET_EMOTE_1
        && event.action <= InputAction::PET_EMOTE_48)
    {
        const int emotion = event.action - InputAction::PET_EMOTE_1;
        if (emoteShortcut)
            petHandler->emote(emoteShortcut->getEmote(emotion), 0);
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }

    return false;
}

impHandler(catchPet)
{
    if (!localPlayer || !actorManager)
        return false;

    Being *target = nullptr;
    if (!event.args.empty())
        target = actorManager->findNearestByName(event.args);
    if (!target)
        target = localPlayer->getTarget();
    else
        localPlayer->setTarget(target);
    if (target)
        petHandler->catchPet(target);
    return true;
}

impHandler0(petMoveUp)
{
    const Being *const pet = getPet();
    if (!pet)
        return false;
    petHandler->move(0, pet->getTileX(), pet->getTileY() - 1);
    return true;
}

impHandler0(petMoveDown)
{
    const Being *const pet = getPet();
    if (!pet)
        return false;
    petHandler->move(0, pet->getTileX(), pet->getTileY() + 1);
    return true;
}

impHandler0(petMoveLeft)
{
    const Being *const pet = getPet();
    if (!pet)
        return false;
    petHandler->move(0, pet->getTileX() - 1, pet->getTileY());
    return true;
}

impHandler0(petMoveRight)
{
    const Being *const pet = getPet();
    if (!pet)
        return false;
    petHandler->move(0, pet->getTileX() + 1, pet->getTileY());
    return true;
}

impHandler0(petDirectUp)
{
    const Being *const pet = getPet();
    if (!pet)
        return false;
    petHandler->setDirection(BeingDirection::UP);
    return true;
}

impHandler0(petDirectDown)
{
    petHandler->setDirection(BeingDirection::DOWN);
    return true;
}

impHandler0(petDirectLeft)
{
    petHandler->setDirection(BeingDirection::LEFT);
    return true;
}

impHandler0(petDirectRight)
{
    petHandler->setDirection(BeingDirection::RIGHT);
    return true;
}

impHandler0(petAiStart)
{
    petHandler->startAi(true);
    return true;
}

impHandler0(petAiStop)
{
    petHandler->startAi(false);
    return true;
}

impHandler0(petMove)
{
    int x = 0;
    int y = 0;

    if (parse2Int(event.args, x, y))
    {
        petHandler->move(0, x, y);
        return true;
    }
    return false;
}

}  // namespace Actions
