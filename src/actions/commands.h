/*
 *  The ManaPlus Client
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

#ifndef ACTIONS_COMMANDS_H
#define ACTIONS_COMMANDS_H

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(chatAnnounce);
    decHandler(chatIgnore);
    decHandler(chatUnignore);
    decHandler(chatFriend);
    decHandler(chatDisregard);
    decHandler(chatNeutral);
    decHandler(chatBlackList);
    decHandler(chatEnemy);
    decHandler(chatErase);
    decHandler(chatNuke);
    decHandler(chatAdd);
    decHandler(present);
    decHandler(printAll);
    decHandler(move);
    decHandler(setTarget);
    decHandler(commandOutfit);
    decHandler(commandEmote);
    decHandler(awayMessage);
    decHandler(pseudoAway);
    decHandler(follow);
    decHandler(navigate);
    decHandler(navigateTo);
    decHandler(moveCamera);
    decHandler(restoreCamera);
    decHandler(imitation);
    decHandler(sendMail);
    decHandler(info);
    decHandler(wait);
    decHandler(addPriorityAttack);
    decHandler(addAttack);
    decHandler(removeAttack);
    decHandler(addIgnoreAttack);
    decHandler(setDrop);
    decHandler(url);
    decHandler(openUrl);
    decHandler(execute);
    decHandler(enableHighlight);
    decHandler(disableHighlight);
    decHandler(dontRemoveName);
    decHandler(removeName);
    decHandler(disableAway);
    decHandler(enableAway);
    decHandler(testParticle);
    decHandler(talkRaw);
    decHandler(gm);
    decHandler(hack);
    decHandler(debugSpawn);
    decHandler(serverIgnoreWhisper);
    decHandler(serverUnIgnoreWhisper);
    decHandler(setHomunculusName);
    decHandler(fireHomunculus);
    decHandler(leaveParty);
    decHandler(leaveGuild);
    decHandler(warp);
    decHandler(homunTalk);
    decHandler(homunEmote);
    decHandler(commandHomunEmote);
    decHandler(createPublicChatRoom);
    decHandler(joinChatRoom);
    decHandler(leaveChatRoom);
    decHandler(confSet);
    decHandler(serverConfSet);
    decHandler(confGet);
    decHandler(serverConfGet);
    decHandler(slide);
    decHandler(selectSkillLevel);
    decHandler(skill);
    decHandler(craft);
    decHandler(npcClipboard);
    decHandler(addPickup);
    decHandler(removePickup);
    decHandler(ignorePickup);
    decHandler(monsterInfo);
    decHandler(itemInfo);
    decHandler(whoDrops);
    decHandler(mobSearch);
    decHandler(mobSpawnSearch);
    decHandler(playerGmCommands);
    decHandler(playerCharGmCommands);
    decHandler(commandShowLevel);
    decHandler(commandShowStats);
    decHandler(commandShowStorage);
    decHandler(commandShowCart);
    decHandler(commandShowInventory);
    decHandler(locatePlayer);
    decHandler(commandShowAccountInfo);
    decHandler(commandSpawn);
    decHandler(commandSpawnSlave);
    decHandler(commandSpawnClone);
    decHandler(commandSpawnSlaveClone);
    decHandler(commandSpawnEvilClone);
    decHandler(commandSavePosition);
    decHandler(commandLoadPosition);
    decHandler(commandRandomWarp);
    decHandler(commandGotoNpc);
    decHandler(commandKiller);
    decHandler(commandKillable);
    decHandler(commandHeal);
    decHandler(commandAlive);
    decHandler(commandDisguise);
    decHandler(commandImmortal);
    decHandler(commandHide);
    decHandler(commandNuke);
    decHandler(commandKill);
    decHandler(commandJail);
    decHandler(commandUnjail);
    decHandler(commandNpcMove);
    decHandler(commandNpcHide);
    decHandler(commandNpcShow);
    decHandler(commandChangePartyLeader);
    decHandler(commandPartyRecall);
    decHandler(commandBreakGuild);
    decHandler(commandGuildRecall);
    decHandler(mailTo);
    decHandler(adoptChild);
    decHandler(showSkillLevels);
    decHandler(showSkillType);
    decHandler(selectSkillType);
    decHandler(showSkillOffsetX);
    decHandler(showSkillOffsetY);
    decHandler(setSkillOffsetX);
    decHandler(setSkillOffsetY);
    decHandler(partyItemShare);
    decHandler(partyExpShare);
    decHandler(partyAutoItemShare);
    decHandler(outfitToChat);
    decHandler(outfitClear);
    decHandler(moveAttackUp);
    decHandler(moveAttackDown);
    decHandler(movePriorityAttackUp);
    decHandler(movePriorityAttackDown);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_COMMANDS_H
