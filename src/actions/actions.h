/*
 *  The ManaPlus Client
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

#ifndef ACTIONS_ACTIONS_H
#define ACTIONS_ACTIONS_H

#define decHandler(name) bool name(const InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(moveUp);
    decHandler(moveDown);
    decHandler(moveLeft);
    decHandler(moveRight);
    decHandler(moveForward);
    decHandler(emote);
    decHandler(outfit);
    decHandler(moveToPoint);
    decHandler(mouseClick);
    decHandler(ok);
    decHandler(shortcut);
    decHandler(prevSocialTab);
    decHandler(nextSocialTab);
    decHandler(quit);
    decHandler(dropItem0);
    decHandler(dropItem);
    decHandler(switchQuickDrop);
    decHandler(heal);
    decHandler(crazyMoves);
    decHandler(itenplz);
    decHandler(changeCrazyMove);
    decHandler(changePickupType);
    decHandler(moveToTarget);
    decHandler(moveToHome);
    decHandler(setHome);
    decHandler(changeMoveType);
    decHandler(changeAttackWeaponType);
    decHandler(changeAttackType);
    decHandler(changeFollowMode);
    decHandler(changeImitationMode);
    decHandler(magicAttack);
    decHandler(changeMagicAttackType);
    decHandler(changePvpMode);
    decHandler(changeMoveToTarget);
    decHandler(copyEquippedToOutfit);
    decHandler(changeGameModifier);
    decHandler(changeAudio);
    decHandler(away);
    decHandler(camera);
    decHandler(pickup);
    decHandler(sit);
    decHandler(changeMapMode);
    decHandler(changeTrade);
    decHandler(screenshot);
    decHandler(ignoreInput);
    decHandler(directUp);
    decHandler(directDown);
    decHandler(directLeft);
    decHandler(directRight);
    decHandler(talk);
    decHandler(stopAttack);
    decHandler(untarget);
    decHandler(attack);
    decHandler(targetAttack);
    decHandler(targetPlayer);
    decHandler(targetMonster);
    decHandler(targetClosestMonster);
    decHandler(targetNPC);
    decHandler(safeVideoMode);
    decHandler(stopSit);
    decHandler(showKeyboard);
    decHandler(showWindows);
    decHandler(nextShortcutsTab);
    decHandler(prevShortcutsTab);
    decHandler(nextCommandsTab);
    decHandler(prevCommandsTab);
    decHandler(openTrade);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_ACTIONS_H
