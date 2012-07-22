/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <string>
#include <map>

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;
class Window;

namespace ActionManager
{
    decHandler(moveUp);
    decHandler(moveDown);
    decHandler(moveLeft);
    decHandler(moveRight);
    decHandler(emote);
    decHandler(outfit);
    decHandler(moveToPoint);
    decHandler(mouseClick);
    decHandler(ok);
    decHandler(shortcut);
    decHandler(toggleChat);
    decHandler(prevChatTab);
    decHandler(nextChatTab);
    decHandler(closeChatTab);
    decHandler(prevSocialTab);
    decHandler(nextSocialTab);
    decHandler(scrollChatUp);
    decHandler(scrollChatDown);
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
    decHandler(changeMagicAttack);
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
    decHandler(targetNPC);
    decHandler(safeVideoMode);

    decHandler(hideWindows);
    decHandler(helpWindowShow);
    decHandler(setupWindowShow);
    decHandler(statusWindowShow);
    decHandler(inventoryWindowShow);
    decHandler(equipmentWindowShow);
    decHandler(skillDialogShow);
    decHandler(minimapWindowShow);
    decHandler(chatWindowShow);
    decHandler(shortcutWindowShow);
    decHandler(debugWindowShow);
    decHandler(socialWindowShow);
    decHandler(emoteShortcutWindowShow);
    decHandler(outfitWindowShow);
    decHandler(shopWindowShow);
    decHandler(dropShortcutWindowShow);
    decHandler(killStatsWindowShow);
    decHandler(spellShortcutWindowShow);
    decHandler(botcheckerWindowShow);
    decHandler(whoIsOnlineWindowShow);
    decHandler(didYouKnowWindowShow);
}

#endif
