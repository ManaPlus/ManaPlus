/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "actions/statusbar.h"

#include "game.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "gui/viewport.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "listeners/updatestatuslistener.h"

#include "resources/map/map.h"

#include "utils/gettext.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif  // USE_SDL2
#endif  // ANDROID
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

namespace Actions
{

impHandler0(switchQuickDrop)
{
    callYellowBarCond(changeQuickDropCounter);
}

impHandler0(changeCrazyMove)
{
    callYellowBar(changeCrazyMoveType);
}

impHandler0(changePickupType)
{
    callYellowBar(changePickUpType);
}

impHandler0(changeMoveType)
{
    callYellowBar(changeMoveType);
}

impHandler0(changeAttackWeaponType)
{
    callYellowBar(changeAttackWeaponType);
}

impHandler0(changeAttackType)
{
    callYellowBar(changeAttackType);
}

impHandler0(changeTargetingType)
{
    callYellowBar(changeTargetingType);
}

impHandler0(changeFollowMode)
{
    callYellowBar(changeFollowMode);
}

impHandler0(changeImitationMode)
{
    callYellowBar(changeImitationMode);
}

impHandler0(changeMagicAttackType)
{
    callYellowBar(changeMagicAttackType);
}

impHandler0(changePvpMode)
{
    callYellowBar(changePvpAttackType);
}

impHandler0(changeMoveToTarget)
{
    callYellowBar(changeMoveToTargetType);
}

impHandler0(changeGameModifier)
{
    if (localPlayer != nullptr)
    {
        GameModifiers::changeGameModifiers(false);
        return true;
    }
    return false;
}

impHandler0(changeAudio)
{
    soundManager.changeAudio();
    if (localPlayer != nullptr)
        localPlayer->updateMusic();
    return true;
}

impHandler0(away)
{
    GameModifiers::changeAwayMode(true);
    if (localPlayer != nullptr)
    {
        localPlayer->updateStatus();
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(camera)
{
    if (viewport != nullptr)
    {
        viewport->toggleCameraMode();
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(changeMapMode)
{
    if (viewport != nullptr)
        viewport->toggleMapDrawType();
    UpdateStatusListener::distributeEvent();
    if (Game::instance() != nullptr)
    {
        if (Map *const map = Game::instance()->getCurrentMap())
            map->redrawMap();
    }
    return true;
}

impHandler0(changeTrade)
{
    unsigned int deflt = playerRelations.getDefault();
    if ((deflt & PlayerRelation::TRADE) != 0U)
    {
        if (localChatTab != nullptr)
        {
            // TRANSLATORS: disable trades message
            localChatTab->chatLog(_("Ignoring incoming trade requests"),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        deflt &= ~PlayerRelation::TRADE;
    }
    else
    {
        if (localChatTab != nullptr)
        {
            // TRANSLATORS: enable trades message
            localChatTab->chatLog(_("Accepting incoming trade requests"),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        deflt |= PlayerRelation::TRADE;
    }

    playerRelations.setDefault(deflt);
    return true;
}

}  // namespace Actions
