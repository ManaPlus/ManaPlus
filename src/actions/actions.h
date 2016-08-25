/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "localconsts.h"

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(emote);
    decHandler(outfit);
    decHandler(mouseClick);
    decHandler(ok);
    decHandler(shortcut);
    decHandler(quit);
    decHandler(dropItem0);
    decHandler(dropItem);
    decHandler(dropItemId);
    decHandler(dropItemInv);
    decHandler(dropItemIdAll);
    decHandler(dropItemInvAll);
    decHandler(heal);
    decHandler(healmd);
    decHandler(itenplz);
    decHandler(setHome);
    decHandler(magicAttack);
    decHandler(copyEquippedToOutfit);
    decHandler(pickup);
    decHandler(sit);
    decHandler(screenshot);
    decHandler(ignoreInput);
    decHandler(talk);
    decHandler(buy);
    decHandler(sell);
    decHandler(stopAttack);
    decHandler(untarget);
    decHandler(attack);
    decHandler(targetAttack);
    decHandler(safeVideoMode);
    decHandler(stopSit);
    decHandler(showKeyboard);
    decHandler(showWindows);
    decHandler(openTrade);
    decHandler(ipcToggle);
    decHandler(where);
    decHandler(who);
    decHandler(cleanGraphics);
    decHandler(cleanFonts);
    decHandler(attackHuman);
    decHandler(trade);
    decHandler(priceLoad);
    decHandler(priceSave);
    decHandler(cacheInfo);
    decHandler(disconnect);
    decHandler(undress);
    decHandler(dirs);
    decHandler(uptime);
    decHandler(dump);
    decHandler(serverIgnoreAll);
    decHandler(serverUnIgnoreAll);
    decHandler(error)
#ifndef ENABLE_CILKPLUS
    __attribute__ ((noreturn))
#endif
    ;
    decHandler(dumpGraphics);
    decHandler(dumpEnvironment);
    decHandler(dumpTests);
    decHandler(dumpOGL);
    decHandler(dumpGL);
    decHandler(dumpMods);
#if defined USE_OPENGL && defined DEBUG_SDLFONT
    decHandler(testSdlFont);
#endif
    decHandler(createItems);
    decHandler(createItem);
    decHandler(uploadConfig);
    decHandler(uploadServerConfig);
    decHandler(uploadLog);
    decHandler(mercenaryFire);
    decHandler(useItem);
    decHandler(useItemInv);
    decHandler(invToStorage);
    decHandler(tradeAdd);
    decHandler(storageToInv);
    decHandler(protectItem);
    decHandler(unprotectItem);
    decHandler(kick);
    decHandler(clearDrop);
    decHandler(testInfo);
    decHandler(craftKey);
    decHandler(resetGameModifiers);
    decHandler(barToChat);
    decHandler(seen);
    decHandler(dumpMemoryUsage);
    decHandler(setEmoteType);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_ACTIONS_H
