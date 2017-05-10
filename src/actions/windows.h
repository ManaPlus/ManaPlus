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

#ifndef ACTIONS_WINDOWS_H
#define ACTIONS_WINDOWS_H

#define decHandler(name) bool name(InputEvent &event)

struct InputEvent;

namespace Actions
{
    decHandler(hideWindows);
    decHandler(helpWindowShow);
    decHandler(aboutWindowShow);
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
    decHandler(whoIsOnlineWindowShow);
    decHandler(didYouKnowWindowShow);
    decHandler(questsWindowShow);
    decHandler(updaterWindowShow);
    decHandler(bankWindowShow);
    decHandler(cartWindowShow);
    decHandler(quickWindowShow);
    decHandler(mailWindowShow);
    decHandler(showItems);
}  // namespace Actions

#undef decHandler

#endif  // ACTIONS_WINDOWS_H
