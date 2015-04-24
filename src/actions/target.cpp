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

#include "actions/target.h"

#include "actormanager.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"

#include "gui/popups/popupmenu.h"

#include "debug.h"

namespace Actions
{

static bool setTarget(const ActorType::Type type, const bool allowSort)
{
    if (actorManager && localPlayer)
    {
        Being *const target = actorManager->findNearestLivingBeing(
            localPlayer, 20, type, allowSort);

        if (target && target != localPlayer->getTarget())
            localPlayer->setTarget(target);

        return true;
    }
    return false;
}

impHandler0(targetPlayer)
{
    return setTarget(ActorType::Player, true);
}

impHandler0(targetMonster)
{
    return setTarget(ActorType::Monster, true);
}

impHandler0(targetClosestMonster)
{
    return setTarget(ActorType::Monster, false);
}

impHandler0(targetNPC)
{
    return setTarget(ActorType::Npc, true);
}

impHandler0(targetMercenary)
{
#ifdef EATHENA_SUPPORT
    return setTarget(ActorType::Mercenary, true);
#else
    return false;
#endif
}

impHandler0(targetPet)
{
#ifdef EATHENA_SUPPORT
    return setTarget(ActorType::Pet, true);
#else
    return false;
#endif
}

impHandler0(contextMenu)
{
    if (!localPlayer)
        return false;
    Being *const target = localPlayer->getTarget();
    if (!target)
        return true;

    popupMenu->showPopup(target->getPixelX(), target->getPixelY(), target);
    return true;
}

}  // namespace Actions
