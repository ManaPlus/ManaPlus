/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#include "actions/actiondef.h"

#include "being/localplayer.h"

#include "gui/popups/popupmenu.h"

#include "debug.h"

namespace Actions
{

static bool setTarget(const ActorTypeT type, const AllowSort allowSort)
{
    if (localPlayer != nullptr)
        return localPlayer->setNewTarget(type, allowSort) != nullptr;
    return false;
}

impHandler0(targetPlayer)
{
    return setTarget(ActorType::Player, AllowSort_true);
}

impHandler0(targetMonster)
{
    return setTarget(ActorType::Monster, AllowSort_true);
}

impHandler0(targetClosestMonster)
{
    return setTarget(ActorType::Monster, AllowSort_false);
}

impHandler0(targetNPC)
{
    return setTarget(ActorType::Npc, AllowSort_true);
}

impHandler0(targetMercenary)
{
    return setTarget(ActorType::Mercenary, AllowSort_true);
}

impHandler0(targetSkillUnit)
{
    return setTarget(ActorType::SkillUnit, AllowSort_true);
}

impHandler0(targetPet)
{
    return setTarget(ActorType::Pet, AllowSort_true);
}

impHandler0(contextMenu)
{
    if (localPlayer == nullptr)
        return false;
    const Being *const target = localPlayer->getTarget();
    if (target == nullptr)
        return true;

    popupMenu->showPopup(target->getPixelX(),
        target->getPixelY(),
        target);
    return true;
}

}  // namespace Actions
