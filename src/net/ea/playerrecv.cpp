/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/ea/playerrecv.h"

#include "game.h"
#include "notifymanager.h"
#include "settings.h"
#include "soundmanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/nostat.h"

#include "enums/resources/notifytypes.h"

#include "gui/viewport.h"

#include "gui/windows/statuswindow.h"

#include "input/inputmanager.h"

#include "resources/map/map.h"

#include "net/playerhandler.h"

#include "utils/stringutils.h"

#include "debug.h"

// Max. distance we are willing to scroll after a teleport;
// everything beyond will reset the port hard.
static const int MAP_TELEPORT_SCROLL_DISTANCE = 8;

namespace Ea
{

void PlayerRecv::processPlayerWarp(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerWarp")
    std::string mapPath = msg.readString(16, "map name");
    int x = msg.readInt16("x");
    int y = msg.readInt16("y");

    logger->log("Warping to %s (%d, %d)", mapPath.c_str(), x, y);

    if (!localPlayer)
        logger->log1("SMSG_PLAYER_WARP localPlayer null");

    /*
      * We must clear the local player's target *before* the call
      * to changeMap, as it deletes all beings.
      */
    if (localPlayer)
        localPlayer->stopAttack();

    Game *const game = Game::instance();
    if (!game)
    {
        BLOCK_END("PlayerRecv::processPlayerWarp")
        return;
    }

    const std::string &currentMapName = game->getCurrentMapName();
    const bool sameMap = (currentMapName == mapPath);

    // Switch the actual map, deleting the previous one if necessary
    mapPath = mapPath.substr(0, mapPath.rfind('.'));
    game->changeMap(mapPath);

    int scrollOffsetX = 0;
    int scrollOffsetY = 0;

    if (localPlayer)
    {
        const Map *const map = game->getCurrentMap();
        if (map)
        {
            if (x >= map->getWidth())
                x = map->getWidth() - 1;
            if (y >= map->getHeight())
                y = map->getHeight() - 1;
            if (x < 0)
                x = 0;
            if (y < 0)
                y = 0;
            /* Scroll if neccessary */
            if (!sameMap
                || (abs(x - localPlayer->getTileX())
                > MAP_TELEPORT_SCROLL_DISTANCE)
                || (abs(y - localPlayer->getTileY())
                > MAP_TELEPORT_SCROLL_DISTANCE))
            {
                scrollOffsetX = (x - localPlayer->getTileX())
                    * map->getTileWidth();
                scrollOffsetY = (y - localPlayer->getTileY())
                    * map->getTileHeight();
            }
        }

        localPlayer->setAction(BeingAction::STAND, 0);
        localPlayer->setTileCoords(x, y);
        localPlayer->navigateClean();
        localPlayer->reset();
    }

    logger->log("Adjust scrolling by %d:%d", scrollOffsetX, scrollOffsetY);

    if (viewport)
    {
        viewport->returnCamera();
        viewport->scrollBy(scrollOffsetX, scrollOffsetY);
    }
    BLOCK_END("PlayerRecv::processPlayerWarp")
}

void PlayerRecv::processPlayerStatUpdate1(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate1")
    const int type = msg.readInt16("type");
    const int value = msg.readInt32("value");
    if (!localPlayer)
    {
        BLOCK_END("PlayerRecv::processPlayerStatUpdate1")
        return;
    }

    playerHandler->setStat(msg, type, value, NoStat, Notify_true);
    BLOCK_END("PlayerRecv::processPlayerStatUpdate1")
}

void PlayerRecv::processPlayerStatUpdate2(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate2")
    const int type = msg.readInt16("type");
    const int value = msg.readInt32("value");
    playerHandler->setStat(msg, type, value, NoStat, Notify_true);
    BLOCK_END("PlayerRecv::processPlayerStatUpdate2")
}

void PlayerRecv::processPlayerStatUpdate3(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate3")
    const int type = msg.readInt32("type");
    const int base = msg.readInt32("base");
    const int bonus = msg.readInt32("bonus");

    playerHandler->setStat(msg, type, base, bonus, Notify_false);
    BLOCK_END("PlayerRecv::processPlayerStatUpdate3")
}

void PlayerRecv::processPlayerStatUpdate4(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate4")
    const uint16_t type = msg.readInt16("type");
    const uint8_t ok = msg.readUInt8("flag");
    const int value = msg.readUInt8("value");

    if (ok != 1)
    {
        const int oldValue = PlayerInfo::getStatBase(
            static_cast<AttributesT>(type));
        const int points = PlayerInfo::getAttribute(
            Attributes::PLAYER_CHAR_POINTS) + oldValue - value;
        PlayerInfo::setAttribute(Attributes::PLAYER_CHAR_POINTS, points);
        NotifyManager::notify(NotifyTypes::SKILL_RAISE_ERROR);
    }

    playerHandler->setStat(msg, type, value, NoStat, Notify_true);
    BLOCK_END("PlayerRecv::processPlayerStatUpdate4")
}

void PlayerRecv::processPlayerStatUpdate6(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerStatUpdate6")
    const int type = msg.readInt16("type");
    const int value = msg.readUInt8("value");
    if (statusWindow)
        playerHandler->setStat(msg, type, value, NoStat, Notify_true);
    BLOCK_END("PlayerRecv::processPlayerStatUpdate6")
}

void PlayerRecv::processPlayerArrowMessage(Net::MessageIn &msg)
{
    BLOCK_START("PlayerRecv::processPlayerArrowMessage")
    const int type = msg.readInt16("type");
    switch (type)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ARROWS_EQUIP_NEEDED);
            break;
        case 3:
            // arrows equiped
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
    BLOCK_END("PlayerRecv::processPlayerArrowMessage")
}

void PlayerRecv::processMapMusic(Net::MessageIn &msg)
{
    const int size = msg.readInt16("len") - 4;
    const std::string music = msg.readString(size, "name");
    soundManager.playMusic(music,
        SkipError_false);

    Map *const map = viewport->getMap();
    if (map)
        map->setMusicFile(music);
}

void PlayerRecv::processMapMask(Net::MessageIn &msg)
{
    const int mask = msg.readInt32("mask");
    msg.readInt32("unused");
    Map *const map = Game::instance()->getCurrentMap();
    if (map)
        map->setMask(mask);
}

void PlayerRecv::processPlayerClientCommand(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 4;
    std::string command = msg.readString(sz, "command");
    std::string cmd;
    std::string args;

    if (settings.awayMode ||
        settings.pseudoAwayMode ||
        !settings.enableRemoteCommands)
    {
        return;
    }

    if (!parse2Str(command, cmd, args))
    {
        cmd = command;
        args.clear();
    }
    inputManager.executeRemoteChatCommand(cmd, args, nullptr);
}

}  // namespace Ea
