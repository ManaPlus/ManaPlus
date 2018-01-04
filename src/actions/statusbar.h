/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef ACTIONS_STATUSBAR_H
#define ACTIONS_STATUSBAR_H

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(switchQuickDrop);
    decHandler(changeCrazyMove);
    decHandler(changePickupType);
    decHandler(changeMoveType);
    decHandler(changeAttackWeaponType);
    decHandler(changeAttackType);
    decHandler(changeTargetingType);
    decHandler(changeFollowMode);
    decHandler(changeImitationMode);
    decHandler(changeMagicAttackType);
    decHandler(changePvpMode);
    decHandler(changeMoveToTarget);
    decHandler(changeGameModifier);
    decHandler(changeAudio);
    decHandler(away);
    decHandler(camera);
    decHandler(changeMapMode);
    decHandler(changeTrade);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_STATUSBAR_H
