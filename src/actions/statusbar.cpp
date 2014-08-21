/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "actormanager.h"
#include "dropshortcut.h"
#include "emoteshortcut.h"
#include "game.h"
#include "itemshortcut.h"
#include "soundmanager.h"

#include "actions/actiondef.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/dialogsmanager.h"
#include "gui/gui.h"
#include "gui/popupmanager.h"
#include "gui/sdlinput.h"
#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"
#include "gui/windows/tradewindow.h"
#include "gui/windows/questswindow.h"
#include "gui/windows/quitdialog.h"
#include "gui/windows/whoisonline.h"
#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/debugwindow.h"
#include "gui/windows/didyouknowwindow.h"
#include "gui/windows/equipmentwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/minimap.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/setupwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/updaterwindow.h"

#include "gui/widgets/tabs/chattab.h"

#include "render/graphics.h"

#include "net/net.h"
#include "net/playerhandler.h"
#include "net/tradehandler.h"

#include "listeners/updatestatuslistener.h"

#include "resources/map/map.h"

#include "utils/gettext.h"

#ifdef ANDROID
#ifndef USE_SDL2
#include <SDL_screenkeyboard.h>
#endif
#endif

#include "debug.h"

extern ShortcutWindow *spellShortcutWindow;
extern std::string tradePartnerName;
extern QuitDialog *quitDialog;

namespace Actions
{

impHandler0(switchQuickDrop)
{
    callYellowBarCond(changeQuickDropCounter)
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
    if (localPlayer)
    {
        GameModifiers::changeGameModifiers();
        return true;
    }
    return false;
}

impHandler0(changeAudio)
{
    soundManager.changeAudio();
    if (localPlayer)
        localPlayer->updateMusic();
    return true;
}

impHandler0(away)
{
    GameModifiers::changeAwayMode();
    if (localPlayer)
    {
        localPlayer->updateStatus();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(camera)
{
    if (viewport)
    {
        viewport->toggleCameraMode();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(changeMapMode)
{
    if (viewport)
        viewport->toggleMapDrawType();
    UpdateStatusListener::distributeEvent();
    if (Game::instance())
    {
        if (Map *const map = Game::instance()->getCurrentMap())
            map->redrawMap();
    }
    return true;
}

impHandler0(changeTrade)
{
    unsigned int deflt = player_relations.getDefault();
    if (deflt & PlayerRelation::TRADE)
    {
        if (localChatTab)
        {
            // TRANSLATORS: disable trades message
            localChatTab->chatLog(_("Ignoring incoming trade requests"),
                ChatMsgType::BY_SERVER);
        }
        deflt &= ~PlayerRelation::TRADE;
    }
    else
    {
        if (localChatTab)
        {
            // TRANSLATORS: enable trades message
            localChatTab->chatLog(_("Accepting incoming trade requests"),
                ChatMsgType::BY_SERVER);
        }
        deflt |= PlayerRelation::TRADE;
    }

    player_relations.setDefault(deflt);
    return true;
}

}  // namespace Actions
