/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "actions/actiondef.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/being/beingdirection.h"

#include "input/inputactionoperators.h"

#include "listeners/inputactionreplaylistener.h"

#include "gui/shortcut/emoteshortcut.h"

#include "net/chathandler.h"
#include "net/pethandler.h"
#include "net/serverfeatures.h"

#include "utils/chatutils.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace Actions
{

static const Being *getPet()
{
    const BeingId id = PlayerInfo::getPetBeingId();
    if (id == BeingId_zero)
        return nullptr;
    return actorManager->findBeing(id);
}

impHandler(commandEmotePet)
{
    petHandler->emote(CAST_U8(atoi(event.args.c_str())));
    return true;
}

impHandler(talkPet)
{
    if (!serverFeatures->haveTalkPet())
        return false;

    std::string args = event.args;
    if (findCutFirst(args, "/me "))
        args = textToMe(args);
    chatHandler->talkPet(args);
    return true;
}

impHandler(setPetName)
{
    const std::string args = event.args;
    if (args.empty())
    {
        const Being *const pet = getPet();
        if (pet == nullptr)
            return false;
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Rename your pet"),
            pet->getName(),
            InputAction::PET_SET_NAME);
        return false;
    }

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
        if (emoteShortcut != nullptr)
        {
            const int emotion = event.action - InputAction::PET_EMOTE_1;
            petHandler->emote(emoteShortcut->getEmote(emotion));
        }
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }

    return false;
}

impHandler(catchPet)
{
    if ((localPlayer == nullptr) || (actorManager == nullptr))
        return false;

    Being *target = nullptr;
    const std::string args = event.args;
    if (!args.empty())
    {
        if (args[0] == ':')
        {
            target = actorManager->findBeing(fromInt(atoi(
                args.substr(1).c_str()), BeingId));
        }
        else
        {
            target = actorManager->findNearestByName(args,
                ActorType::Unknown);
        }
    }

    if (target == nullptr)
        target = localPlayer->getTarget();
    else
        localPlayer->setTarget(target);
    if (target != nullptr)
        petHandler->catchPet(target);
    return true;
}

impHandler0(petMoveUp)
{
    const Being *const pet = getPet();
    if (pet == nullptr)
        return false;
    petHandler->move(pet->getTileX(), pet->getTileY() - 1);
    return true;
}

impHandler0(petMoveDown)
{
    const Being *const pet = getPet();
    if (pet == nullptr)
        return false;
    petHandler->move(pet->getTileX(), pet->getTileY() + 1);
    return true;
}

impHandler0(petMoveLeft)
{
    const Being *const pet = getPet();
    if (pet == nullptr)
        return false;
    petHandler->move(pet->getTileX() - 1, pet->getTileY());
    return true;
}

impHandler0(petMoveRight)
{
    const Being *const pet = getPet();
    if (pet == nullptr)
        return false;
    petHandler->move(pet->getTileX() + 1, pet->getTileY());
    return true;
}

impHandler0(petDirectUp)
{
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

impHandler(petMove)
{
    int x = 0;
    int y = 0;

    if (parse2Int(event.args, x, y))
    {
        petHandler->move(x, y);
        return true;
    }
    return false;
}

impHandler0(petFeed)
{
    if (petHandler != nullptr)
        petHandler->feed();
    return true;
}

impHandler0(petDropLoot)
{
    if (petHandler != nullptr)
        petHandler->dropLoot();
    return true;
}

impHandler0(petReturnToEgg)
{
    if (petHandler != nullptr)
        petHandler->returnToEgg();
    return true;
}

impHandler0(petUnequip)
{
    if (petHandler != nullptr)
        petHandler->unequip();
    return true;
}

}  // namespace Actions
