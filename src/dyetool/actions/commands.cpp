/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "actions/commands.h"

#include "actions/actiondef.h"

#include "debug.h"

namespace Actions
{

impHandlerVoid(chatAnnounce)
impHandlerVoid(chatIgnore)
impHandlerVoid(chatUnignore)
impHandlerVoid(chatErase)
impHandlerVoid(chatFriend)
impHandlerVoid(chatDisregard)
impHandlerVoid(chatNeutral)
impHandlerVoid(chatBlackList)
impHandlerVoid(chatEnemy)
impHandlerVoid(chatNuke)
impHandlerVoid(chatAdd)
impHandlerVoid(present)
impHandlerVoid(printAll)
impHandlerVoid(move)
impHandlerVoid(setTarget)
impHandlerVoid(commandOutfit)
impHandlerVoid(commandEmote)
impHandlerVoid(awayMessage)
impHandlerVoid(pseudoAway)
impHandlerVoid(follow)
impHandlerVoid(navigate)
impHandlerVoid(navigateTo)
impHandlerVoid(moveCamera)
impHandlerVoid(restoreCamera)
impHandlerVoid(imitation)
impHandlerVoid(sendMail)
impHandlerVoid(info)
impHandlerVoid(wait)
impHandlerVoid(addPriorityAttack)
impHandlerVoid(addAttack)
impHandlerVoid(removeAttack)
impHandlerVoid(addIgnoreAttack)
impHandlerVoid(setDrop)
impHandlerVoid(url)
impHandlerVoid(openUrl)
impHandlerVoid(execute)
impHandlerVoid(enableHighlight)
impHandlerVoid(disableHighlight)
impHandlerVoid(dontRemoveName)
impHandlerVoid(removeName)
impHandlerVoid(disableAway)
impHandlerVoid(enableAway)
impHandlerVoid(testParticle)
impHandlerVoid(talkRaw)
impHandlerVoid(gm)
impHandlerVoid(hack)
impHandlerVoid(debugSpawn)
impHandlerVoid(serverIgnoreWhisper)
impHandlerVoid(serverUnIgnoreWhisper)
impHandlerVoid(setHomunculusName)
impHandlerVoid(fireHomunculus)
impHandlerVoid(leaveParty)
impHandlerVoid(leaveGuild)
impHandlerVoid(warp)
impHandlerVoid(homunTalk)
impHandlerVoid(homunEmote)
impHandlerVoid(commandHomunEmote)
impHandlerVoid(createPublicChatRoom)
impHandlerVoid(joinChatRoom)
impHandlerVoid(leaveChatRoom)
impHandlerVoid(confSet)
impHandlerVoid(serverConfSet)
impHandlerVoid(confGet)
impHandlerVoid(serverConfGet)
impHandlerVoid(slide)
impHandlerVoid(selectSkillLevel)
impHandlerVoid(skill)
impHandlerVoid(craft)
impHandlerVoid(npcClipboard)
impHandlerVoid(addPickup)
impHandlerVoid(removePickup)
impHandlerVoid(ignorePickup)
impHandlerVoid(monsterInfo)
impHandlerVoid(itemInfo)
impHandlerVoid(whoDrops)
impHandlerVoid(mobSearch)
impHandlerVoid(mobSpawnSearch)
impHandlerVoid(playerGmCommands)
impHandlerVoid(playerCharGmCommands)
impHandlerVoid(commandShowLevel)
impHandlerVoid(commandShowStats)
impHandlerVoid(commandShowStorage)
impHandlerVoid(commandShowCart)
impHandlerVoid(commandShowInventory)
impHandlerVoid(locatePlayer)
impHandlerVoid(commandShowAccountInfo)
impHandlerVoid(commandSpawn)
impHandlerVoid(commandSpawnSlave)
impHandlerVoid(commandSpawnClone)
impHandlerVoid(commandSpawnSlaveClone)
impHandlerVoid(commandSpawnEvilClone)
impHandlerVoid(commandSavePosition)
impHandlerVoid(commandLoadPosition)
impHandlerVoid(commandRandomWarp)
impHandlerVoid(commandGotoNpc)
impHandlerVoid(commandKiller)
impHandlerVoid(commandKillable)
impHandlerVoid(commandHeal)
impHandlerVoid(commandAlive)
impHandlerVoid(commandDisguise)
impHandlerVoid(commandImmortal)
impHandlerVoid(commandHide)
impHandlerVoid(commandNuke)
impHandlerVoid(commandKill)
impHandlerVoid(commandJail)
impHandlerVoid(commandUnjail)
impHandlerVoid(commandNpcMove)
impHandlerVoid(commandNpcHide)
impHandlerVoid(commandNpcShow)
impHandlerVoid(commandChangePartyLeader)
impHandlerVoid(commandPartyRecall)
impHandlerVoid(commandBreakGuild)
impHandlerVoid(commandGuildRecall)
impHandlerVoid(mailTo)
impHandlerVoid(adoptChild)
impHandlerVoid(showSkillLevels)
impHandlerVoid(showSkillType)
impHandlerVoid(selectSkillType)
impHandlerVoid(showSkillOffsetX)
impHandlerVoid(showSkillOffsetY)
impHandlerVoid(setSkillOffsetX)
impHandlerVoid(setSkillOffsetY)
impHandlerVoid(partyItemShare)
impHandlerVoid(partyExpShare)
impHandlerVoid(partyAutoItemShare)
impHandlerVoid(outfitToChat)
impHandlerVoid(moveAttackUp)
impHandlerVoid(moveAttackDown)

}  // namespace Actions
