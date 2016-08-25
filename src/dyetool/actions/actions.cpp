/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#include "actions/actions.h"

#include "actions/actiondef.h"

#include "debug.h"

namespace Actions
{

impHandlerVoid(emote)
impHandlerVoid(outfit)
impHandlerVoid(mouseClick)
impHandlerVoid(ok)
impHandlerVoid(shortcut)
impHandlerVoid(quit)
impHandlerVoid(dropItem0)
impHandlerVoid(dropItem)
impHandlerVoid(dropItemId)
impHandlerVoid(dropItemInv)
impHandlerVoid(dropItemIdAll)
impHandlerVoid(dropItemInvAll)
impHandlerVoid(heal)
impHandlerVoid(healmd)
impHandlerVoid(itenplz)
impHandlerVoid(setHome)
impHandlerVoid(magicAttack)
impHandlerVoid(copyEquippedToOutfit)
impHandlerVoid(pickup)
impHandlerVoid(sit)
impHandlerVoid(screenshot)
impHandlerVoid(ignoreInput)
impHandlerVoid(buy)
impHandlerVoid(sell)
impHandlerVoid(talk)
impHandlerVoid(stopAttack)
impHandlerVoid(untarget)
impHandlerVoid(attack)
impHandlerVoid(targetAttack)
impHandlerVoid(attackHuman)
impHandlerVoid(safeVideoMode)
impHandlerVoid(stopSit)
impHandlerVoid(showKeyboard)
impHandlerVoid(showWindows)
impHandlerVoid(openTrade)
impHandlerVoid(ipcToggle)
impHandlerVoid(where)
impHandlerVoid(who)
impHandlerVoid(cleanGraphics)
impHandlerVoid(cleanFonts)
impHandlerVoid(trade)
impHandlerVoid(priceLoad)
impHandlerVoid(priceSave)
impHandlerVoid(cacheInfo)
impHandlerVoid(disconnect)
impHandlerVoid(undress)
impHandlerVoid(dirs)
impHandlerVoid(uptime)
impHandlerVoid(dump)
impHandlerVoid(serverIgnoreAll)
impHandlerVoid(serverUnIgnoreAll)
impHandler0(error)
{
    exit(0);
}
impHandlerVoid(dumpGraphics)
impHandlerVoid(dumpEnvironment)
impHandlerVoid(dumpTests)
impHandlerVoid(dumpOGL)
impHandlerVoid(dumpGL)
impHandlerVoid(dumpMods)
#if defined USE_OPENGL && defined DEBUG_SDLFONT
impHandlerVoid(testSdlFont)
#endif
impHandlerVoid(createItems)
impHandlerVoid(createItem)
impHandlerVoid(uploadConfig)
impHandlerVoid(uploadServerConfig)
impHandlerVoid(uploadLog)
impHandlerVoid(mercenaryFire)
impHandlerVoid(useItem)
impHandlerVoid(useItemInv)
impHandlerVoid(invToStorage)
impHandlerVoid(tradeAdd)
impHandlerVoid(storageToInv)
impHandlerVoid(protectItem)
impHandlerVoid(unprotectItem)
impHandlerVoid(kick)
impHandlerVoid(clearDrop)
impHandlerVoid(testInfo)
impHandlerVoid(craftKey)
impHandlerVoid(resetGameModifiers)
impHandlerVoid(barToChat)
impHandlerVoid(seen)
impHandlerVoid(dumpMemoryUsage)
impHandlerVoid(setEmoteType)

}  // namespace Actions
