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

#include "being/crazymoves.h"

#include "configuration.h"
#include "settings.h"

#include "being/localplayer.h"

#include "enums/being/beingdirection.h"

#include "gui/shortcut/dropshortcut.h"

#include "gui/windows/outfitwindow.h"

#include "net/packetlimiter.h"
#include "net/pethandler.h"
#include "net/playerhandler.h"

#include "resources/db/emotedb.h"

#include "resources/map/map.h"

#include "utils/timer.h"

#include "debug.h"

CrazyMoves *crazyMoves = nullptr;

CrazyMoves::CrazyMoves() :
    mMoveProgram(),
    mDisableCrazyMove(false)
{
}

void CrazyMoves::crazyMove()
{
    const bool oldDisableCrazyMove = mDisableCrazyMove;
    mDisableCrazyMove = true;
    switch (settings.crazyMoveType)
    {
        case 1:
            crazyMove1();
            break;
        case 2:
            crazyMove2();
            break;
        case 3:
            crazyMove3();
            break;
        case 4:
            crazyMove4();
            break;
        case 5:
            crazyMove5();
            break;
        case 6:
            crazyMove6();
            break;
        case 7:
            crazyMove7();
            break;
        case 8:
            crazyMove8();
            break;
        case 9:
            crazyMove9();
            break;
        case 10:
            crazyMoveA();
            break;
        default:
            break;
    }
    mDisableCrazyMove = oldDisableCrazyMove;
}

void CrazyMoves::crazyMove1()
{
    if ((localPlayer == nullptr) ||
        (playerHandler == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

//    if (!PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
//        return;

    switch (localPlayer->getDirection())
    {
        case BeingDirection::UP:
            localPlayer->setWalkingDir(BeingDirection::UP);
            localPlayer->setDirection(BeingDirection::LEFT);
            playerHandler->setDirection(BeingDirection::LEFT);
            break;
        case BeingDirection::LEFT:
            localPlayer->setWalkingDir(BeingDirection::LEFT);
            localPlayer->setDirection(BeingDirection::DOWN);
            playerHandler->setDirection(BeingDirection::DOWN);
            break;
        case BeingDirection::DOWN:
            localPlayer->setWalkingDir(BeingDirection::DOWN);
            localPlayer->setDirection(BeingDirection::RIGHT);
            playerHandler->setDirection(BeingDirection::RIGHT);
            break;
        case BeingDirection::RIGHT:
            localPlayer->setWalkingDir(BeingDirection::RIGHT);
            localPlayer->setDirection(BeingDirection::UP);
            playerHandler->setDirection(BeingDirection::UP);
            break;
        default:
            break;
    }
}

void CrazyMoves::crazyMove2()
{
    if ((localPlayer == nullptr) ||
        (playerHandler == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

//    if (!PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
//        return;

    switch (localPlayer->getDirection())
    {
        case BeingDirection::UP:
            localPlayer->setWalkingDir(BeingDirection::UP |
                BeingDirection::LEFT);
            localPlayer->setDirection(BeingDirection::RIGHT);
            playerHandler->setDirection(
                BeingDirection::DOWN | BeingDirection::RIGHT);
            break;

        case BeingDirection::RIGHT:
            localPlayer->setWalkingDir(BeingDirection::UP |
                BeingDirection::RIGHT);
            localPlayer->setDirection(BeingDirection::DOWN);
            playerHandler->setDirection(
                BeingDirection::DOWN | BeingDirection::LEFT);
            break;

        case BeingDirection::DOWN:
            localPlayer->setWalkingDir(BeingDirection::DOWN |
                BeingDirection::RIGHT);
            localPlayer->setDirection(BeingDirection::LEFT);
            playerHandler->setDirection(
                BeingDirection::UP | BeingDirection::LEFT);
            break;

        case BeingDirection::LEFT:
            localPlayer->setWalkingDir(BeingDirection::DOWN |
                BeingDirection::LEFT);
            localPlayer->setDirection(BeingDirection::UP);
            playerHandler->setDirection(
                BeingDirection::UP | BeingDirection::RIGHT);
            break;

        default:
            break;
    }
}

void CrazyMoves::crazyMove3()
{
    if ((localPlayer == nullptr) ||
        (playerHandler == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            localPlayer->move(1, 1);
            settings.crazyMoveState = 1;
            break;
        case 1:
            localPlayer->move(1, -1);
            settings.crazyMoveState = 2;
            break;
        case 2:
            localPlayer->move(-1, -1);
            settings.crazyMoveState = 3;
            break;
        case 3:
            localPlayer->move(-1, 1);
            settings.crazyMoveState = 0;
            break;
        default:
            break;
    }

//    if (!PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
//        return;

    localPlayer->setDirection(BeingDirection::DOWN);
    playerHandler->setDirection(BeingDirection::DOWN);
}

void CrazyMoves::crazyMove4()
{
    if ((localPlayer == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            localPlayer->move(7, 0);
            settings.crazyMoveState = 1;
        break;
        case 1:
            localPlayer->move(-7, 0);
            settings.crazyMoveState = 0;
        break;
        default:
        break;
    }
}

void CrazyMoves::crazyMove5()
{
    if ((localPlayer == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            localPlayer->move(0, 7);
            settings.crazyMoveState = 1;
            break;
        case 1:
            localPlayer->move(0, -7);
            settings.crazyMoveState = 0;
            break;
        default:
            break;
    }
}

void CrazyMoves::crazyMove6()
{
    if ((localPlayer == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            localPlayer->move(3, 0);
            settings.crazyMoveState = 1;
            break;
        case 1:
            localPlayer->move(2, -2);
            settings.crazyMoveState = 2;
            break;
        case 2:
            localPlayer->move(0, -3);
            settings.crazyMoveState = 3;
            break;
        case 3:
            localPlayer->move(-2, -2);
            settings.crazyMoveState = 4;
            break;
        case 4:
            localPlayer->move(-3, 0);
            settings.crazyMoveState = 5;
            break;
        case 5:
            localPlayer->move(-2, 2);
            settings.crazyMoveState = 6;
            break;
        case 6:
            localPlayer->move(0, 3);
            settings.crazyMoveState = 7;
            break;
        case 7:
            localPlayer->move(2, 2);
            settings.crazyMoveState = 0;
            break;
        default:
            break;
    }
}

void CrazyMoves::crazyMove7()
{
    if ((localPlayer == nullptr) ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            localPlayer->move(1, 1);
            settings.crazyMoveState = 1;
            break;
        case 1:
            localPlayer->move(-1, 1);
            settings.crazyMoveState = 2;
            break;
        case 2:
            localPlayer->move(-1, -1);
            settings.crazyMoveState = 3;
            break;
        case 3:
            localPlayer->move(1, -1);
            settings.crazyMoveState = 0;
            break;
        default:
            break;
    }
}

void CrazyMoves::crazyMove8()
{
    if (localPlayer == nullptr ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }
    const Map *const map = localPlayer->getMap();
    if (map == nullptr)
        return;
    const int x = localPlayer->getTileX();
    const int y = localPlayer->getTileY();
    int idx = 0;
    const int dist = 1;

// look
//      up, ri,do,le
    static const int movesX[][4] =
    {
        {-1,  0,  1,  0},   // move left
        { 0,  1,  0, -1},   // move up
        { 1,  0, -1,  0},   // move right
        { 0, -1,  0,  1}    // move down
    };

// look
//      up, ri,do,le
    static const int movesY[][4] =
    {
        { 0, -1,  0,  1},   // move left
        {-1,  0,  1,  0},   // move up
        { 0,  1,  0, -1},   // move right
        { 1,  0, -1,  0}    // move down
    };

    switch (localPlayer->getDirection())
    {
        case BeingDirection::UP:
            idx = 0;
            break;

        case BeingDirection::RIGHT:
            idx = 1;
            break;

        case BeingDirection::DOWN:
            idx = 2;
            break;

        case BeingDirection::LEFT:
            idx = 3;
            break;

        default:
            break;
    }

    int mult = 1;
    const unsigned char blockWalkMask = localPlayer->getBlockWalkMask();
    if (map->getWalk(x + movesX[idx][0],
        y + movesY[idx][0], blockWalkMask))
    {
        while (map->getWalk(x + movesX[idx][0] * mult,
               y + movesY[idx][0] * mult,
               blockWalkMask) && mult <= dist)
        {
            mult ++;
        }
        localPlayer->move(movesX[idx][0] * (mult - 1),
            movesY[idx][0] * (mult - 1));
    }
    else if (map->getWalk(x + movesX[idx][1],
             y + movesY[idx][1], blockWalkMask))
    {
        while (map->getWalk(x + movesX[idx][1] * mult,
               y + movesY[idx][1] * mult,
               blockWalkMask) && mult <= dist)
        {
            mult ++;
        }
        localPlayer->move(movesX[idx][1] * (mult - 1),
            movesY[idx][1] * (mult - 1));
    }
    else if (map->getWalk(x + movesX[idx][2],
             y + movesY[idx][2], blockWalkMask))
    {
        while (map->getWalk(x + movesX[idx][2] * mult,
               y + movesY[idx][2] * mult,
               blockWalkMask) && mult <= dist)
        {
            mult ++;
        }
        localPlayer->move(movesX[idx][2] * (mult - 1),
            movesY[idx][2] * (mult - 1));
    }
    else if (map->getWalk(x + movesX[idx][3],
             y + movesY[idx][3], blockWalkMask))
    {
        while (map->getWalk(x + movesX[idx][3] * mult,
               y + movesY[idx][3] * mult,
               blockWalkMask) && mult <= dist)
        {
            mult ++;
        }
        localPlayer->move(movesX[idx][3] * (mult - 1),
            movesY[idx][3] * (mult - 1));
    }
}

void CrazyMoves::crazyMove9()
{
    int dx = 0;
    int dy = 0;

    if (localPlayer == nullptr ||
        localPlayer->getCurrentAction() == BeingAction::MOVE)
    {
        return;
    }

    switch (settings.crazyMoveState)
    {
        case 0:
            switch (localPlayer->getDirection())
            {
                case BeingDirection::UP   : dy = -1; break;
                case BeingDirection::DOWN : dy = 1; break;
                case BeingDirection::LEFT : dx = -1; break;
                case BeingDirection::RIGHT: dx = 1; break;
                default: break;
            }
            localPlayer->move(dx, dy);
            settings.crazyMoveState = 1;
            break;
        case 1:
            settings.crazyMoveState = 2;
            if (!localPlayer->allowAction())
                return;
            if (playerHandler != nullptr)
                playerHandler->changeAction(BeingAction::SIT);
            break;
        case 2:
            settings.crazyMoveState = 3;
            break;
        case 3:
            settings.crazyMoveState = 0;
            break;
        default:
            break;
    }
}

void CrazyMoves::crazyMoveAm() const
{
    if (localPlayer == nullptr)
        return;

    settings.crazyMoveState ++;
    if (settings.crazyMoveState < mMoveProgram.length())
    {
        int dx = 0;
        int dy = 0;

        signed char param = mMoveProgram[settings.crazyMoveState++];
        if (param == '?')
        {
            const char cmd[] = {'l', 'r', 'u', 'd', 'L', 'R', 'U', 'D'};
            srand(tick_time);
            param = cmd[rand() % 8];
        }
        switch (param)
        {
            case 'd':
                localPlayer->move(0, 1);
                break;
            case 'u':
                localPlayer->move(0, -1);
                break;
            case 'l':
                localPlayer->move(-1, 0);
                break;
            case 'r':
                localPlayer->move(1, 0);
                break;
            case 'D':
                localPlayer->move(1, 1);
                break;
            case 'U':
                localPlayer->move(-1, -1);
                break;
            case 'L':
                localPlayer->move(-1, 1);
                break;
            case 'R':
                localPlayer->move(1, -1);
                break;
            case 'f':
            {
                const uint8_t dir = localPlayer->getDirection();
                if ((dir & BeingDirection::UP) != 0)
                    dy = -1;
                else if ((dir & BeingDirection::DOWN) != 0)
                    dy = 1;
                if ((dir & BeingDirection::LEFT) != 0)
                    dx = -1;
                else if ((dir & BeingDirection::RIGHT) != 0)
                    dx = 1;
                localPlayer->move(dx, dy);
                break;
            }
            case 'b':
            {
                const uint8_t dir = localPlayer->getDirection();
                if ((dir & BeingDirection::UP) != 0)
                    dy = 1;
                else if ((dir & BeingDirection::DOWN) != 0)
                    dy = -1;
                if ((dir & BeingDirection::LEFT) != 0)
                    dx = 1;
                else if ((dir & BeingDirection::RIGHT) != 0)
                    dx = -1;
                localPlayer->move(dx, dy);
                break;
            }
            default:
                break;
        }
    }
}

void CrazyMoves::crazyMoveAd() const
{
    settings.crazyMoveState ++;

    if (settings.crazyMoveState < mMoveProgram.length())
    {
        signed char param = mMoveProgram[settings.crazyMoveState++];
        if (param == '?')
        {
            const char cmd[] = {'l', 'r', 'u', 'd'};
            srand(tick_time);
            param = cmd[rand() % 4];
        }
        switch (param)
        {
            case 'd':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    localPlayer->setDirection(BeingDirection::DOWN);
                    playerHandler->setDirection(
                        BeingDirection::DOWN);
                }
                break;
            case 'u':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    localPlayer->setDirection(BeingDirection::UP);
                    playerHandler->setDirection(
                        BeingDirection::UP);
                }
                break;
            case 'l':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    localPlayer->setDirection(BeingDirection::LEFT);
                    playerHandler->setDirection(
                        BeingDirection::LEFT);
                }
                break;
            case 'r':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    localPlayer->setDirection(BeingDirection::RIGHT);
                    playerHandler->setDirection(
                        BeingDirection::RIGHT);
                }
                break;
            case 'L':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    uint8_t dir = 0;
                    switch (localPlayer->getDirection())
                    {
                        case BeingDirection::UP:
                            dir = BeingDirection::LEFT;
                            break;
                        case BeingDirection::DOWN:
                            dir = BeingDirection::RIGHT;
                            break;
                        case BeingDirection::LEFT:
                            dir = BeingDirection::DOWN;
                            break;
                        case BeingDirection::RIGHT:
                            dir = BeingDirection::UP;
                            break;
                        default:
                            break;
                    }
                    localPlayer->setDirection(dir);
                    playerHandler->setDirection(dir);
                }
                break;
            case 'R':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    uint8_t dir = 0;
                    switch (localPlayer->getDirection())
                    {
                        case BeingDirection::UP:
                            dir = BeingDirection::RIGHT;
                            break;
                        case BeingDirection::DOWN:
                            dir = BeingDirection::LEFT;
                            break;
                        case BeingDirection::LEFT:
                            dir = BeingDirection::UP;
                            break;
                        case BeingDirection::RIGHT:
                            dir = BeingDirection::DOWN;
                            break;
                        default:
                            break;
                    }
                    localPlayer->setDirection(dir);
                    playerHandler->setDirection(dir);
                }
                break;
            case 'b':
//               if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
                {
                    uint8_t dir = 0;
                    switch (localPlayer->getDirection())
                    {
                        case BeingDirection::UP:
                            dir = BeingDirection::DOWN;
                            break;
                        case BeingDirection::DOWN:
                            dir = BeingDirection::UP;
                            break;
                        case BeingDirection::LEFT:
                            dir = BeingDirection::RIGHT;
                            break;
                        case BeingDirection::RIGHT:
                            dir = BeingDirection::LEFT;
                            break;
                        default:
                            break;
                    }
                    localPlayer->setDirection(dir);
                    playerHandler->setDirection(dir);
                }
                break;
            case '0':
                dropShortcut->dropFirst();
                break;
            case 'a':
                dropShortcut->dropItems(1);
                break;
            default:
                break;
        }
    }
}

void CrazyMoves::crazyMoveAs()
{
    settings.crazyMoveState ++;
    if (localPlayer->toggleSit())
        settings.crazyMoveState ++;
}

void CrazyMoves::crazyMoveAo() const
{
    settings.crazyMoveState ++;
    if (settings.crazyMoveState < mMoveProgram.length())
    {
        // wear next outfit
        if (mMoveProgram[settings.crazyMoveState] == 'n')
        {
            settings.crazyMoveState ++;
            outfitWindow->wearNextOutfit(false);
        }
        // wear previous outfit
        else if (mMoveProgram[settings.crazyMoveState] == 'p')
        {
            settings.crazyMoveState ++;
            outfitWindow->wearPreviousOutfit(false);
        }
    }
}

void CrazyMoves::crazyMoveAe() const
{
    settings.crazyMoveState ++;
    const signed char emo = mMoveProgram[settings.crazyMoveState];
    unsigned char emoteId = 0;
    if (emo == '?')
    {
        srand(tick_time);
        emoteId = CAST_U8(
            1 + (rand() % EmoteDB::size()));
    }
    else
    {
        if (emo >= '0' && emo <= '9')
            emoteId = CAST_U8(emo - '0' + 1);
        else if (emo >= 'a' && emo <= 'z')
            emoteId = CAST_U8(emo - 'a' + 11);
        else if (emo >= 'A' && emo <= 'Z')
            emoteId = CAST_U8(emo - 'A' + 37);
    }
    if (mMoveProgram[settings.crazyMoveState - 1] == 'e')
        LocalPlayer::emote(emoteId);
    else if (PacketLimiter::limitPackets(PacketType::PACKET_CHAT))
        petHandler->emote(emoteId);

    settings.crazyMoveState ++;
}

void CrazyMoves::crazyMoveA()
{
    mMoveProgram = config.getStringValue("crazyMoveProgram");

    if (localPlayer->getCurrentAction() == BeingAction::MOVE)
        return;

    if (mMoveProgram.empty())
        return;

    if (settings.crazyMoveState >= mMoveProgram.length())
        settings.crazyMoveState = 0;

    // move command
    if (mMoveProgram[settings.crazyMoveState] == 'm')
    {
        crazyMoveAm();
    }
    // direction command
    else if (mMoveProgram[settings.crazyMoveState] == 'd')
    {
        crazyMoveAd();
    }
    // sit command
    else if (mMoveProgram[settings.crazyMoveState] == 's')
    {
        crazyMoveAs();
    }
    // wear outfits
    else if (mMoveProgram[settings.crazyMoveState] == 'o')
    {
        crazyMoveAo();
    }
    // pause
    else if (mMoveProgram[settings.crazyMoveState] == 'w')
    {
        settings.crazyMoveState ++;
    }
    // pick up
    else if (mMoveProgram[settings.crazyMoveState] == 'p')
    {
        settings.crazyMoveState ++;
        localPlayer->pickUpItems(0);
    }
    // emote
    else if (mMoveProgram[settings.crazyMoveState] == 'e'
             || mMoveProgram[settings.crazyMoveState] == 'E')
    {
        crazyMoveAe();
    }
    else
    {
        settings.crazyMoveState ++;
    }

    if (settings.crazyMoveState >= mMoveProgram.length())
        settings.crazyMoveState = 0;
}
