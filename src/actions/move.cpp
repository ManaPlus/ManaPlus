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

#include "actions/move.h"

#include "game.h"

#include "actions/actiondef.h"
#include "actions/pets.h"

#include "being/crazymoves.h"
#include "being/localplayer.h"

#include "enums/being/beingdirection.h"

#include "gui/windows/socialwindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"

#include "gui/popups/popupmenu.h"

#include "input/inputactionoperators.h"

#include "net/playerhandler.h"

#include "debug.h"

namespace Actions
{

static bool closeMoveNpcDialog(bool focus)
{
    NpcDialog *const dialog = NpcDialog::getActive();
    if (dialog)
    {
        if (dialog->isCloseState())
        {
            dialog->closeDialog();
            return true;
        }
        else if (focus)
        {
            dialog->refocus();
        }
    }
    return false;
}

impHandler(moveUp)
{
    if (inputManager.isActionActive(InputAction::EMOTE))
        return directUp(event);
    else if (inputManager.isActionActive(InputAction::PET_EMOTE))
        return petDirectUp(event);
    else if (inputManager.isActionActive(InputAction::STOP_ATTACK))
        return petMoveUp(event);
    else if (!localPlayer->canMove())
        return directUp(event);
    if (popupMenu->isPopupVisible())
    {
        popupMenu->moveUp();
        return true;
    }
    return closeMoveNpcDialog(false);
}

impHandler(moveDown)
{
    if (inputManager.isActionActive(InputAction::EMOTE))
        return directDown(event);
    else if (inputManager.isActionActive(InputAction::PET_EMOTE))
        return petDirectDown(event);
    else if (inputManager.isActionActive(InputAction::STOP_ATTACK))
        return petMoveDown(event);
    else if (!localPlayer->canMove())
        return directDown(event);
    if (popupMenu->isPopupVisible())
    {
        popupMenu->moveDown();
        return true;
    }
    return closeMoveNpcDialog(false);
}

impHandler(moveLeft)
{
    if (outfitWindow && inputManager.isActionActive(InputAction::WEAR_OUTFIT))
    {
        outfitWindow->wearPreviousOutfit();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    if (inputManager.isActionActive(InputAction::EMOTE))
        return directLeft(event);
    else if (inputManager.isActionActive(InputAction::PET_EMOTE))
        return petDirectLeft(event);
    else if (inputManager.isActionActive(InputAction::STOP_ATTACK))
        return petMoveLeft(event);
    else if (!localPlayer->canMove())
        return directLeft(event);
    return closeMoveNpcDialog(false);
}

impHandler(moveRight)
{
    if (outfitWindow && inputManager.isActionActive(InputAction::WEAR_OUTFIT))
    {
        outfitWindow->wearNextOutfit();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    if (inputManager.isActionActive(InputAction::EMOTE))
        return directRight(event);
    else if (inputManager.isActionActive(InputAction::PET_EMOTE))
        return petDirectRight(event);
    else if (inputManager.isActionActive(InputAction::STOP_ATTACK))
        return petMoveRight(event);
    else if (!localPlayer->canMove())
        return directRight(event);
    return closeMoveNpcDialog(false);
}

impHandler(moveForward)
{
    if (inputManager.isActionActive(InputAction::EMOTE))
        return directRight(event);
    return closeMoveNpcDialog(false);
}

impHandler(moveToPoint)
{
    const int num = event.action - InputAction::MOVE_TO_POINT_1;
    if (socialWindow && num >= 0)
    {
        socialWindow->selectPortal(num);
        return true;
    }

    return false;
}

impHandler0(crazyMoves)
{
    if (localPlayer)
    {
        ::crazyMoves->crazyMove();
        return true;
    }
    return false;
}

impHandler0(moveToTarget)
{
    if (localPlayer && !inputManager.isActionActive(InputAction::TARGET_ATTACK)
        && !inputManager.isActionActive(InputAction::ATTACK))
    {
        localPlayer->moveToTarget();
        return true;
    }
    return false;
}

impHandler0(moveToHome)
{
    if (localPlayer && !inputManager.isActionActive(InputAction::TARGET_ATTACK)
        && !inputManager.isActionActive(InputAction::ATTACK))
    {
        localPlayer->moveToHome();
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
    return false;
}

impHandler0(directUp)
{
    if (localPlayer)
    {
        if (localPlayer->getDirection() != BeingDirection::UP)
        {
//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
            {
                localPlayer->setDirection(BeingDirection::UP);
                if (playerHandler)
                    playerHandler->setDirection(BeingDirection::UP);
            }
        }
        return true;
    }
    return false;
}

impHandler0(directDown)
{
    if (localPlayer)
    {
        if (localPlayer->getDirection() != BeingDirection::DOWN)
        {
//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
            {
                localPlayer->setDirection(BeingDirection::DOWN);
                if (playerHandler)
                {
                    playerHandler->setDirection(
                        BeingDirection::DOWN);
                }
            }
        }
        return true;
    }
    return false;
}

impHandler0(directLeft)
{
    if (localPlayer)
    {
        if (localPlayer->getDirection() != BeingDirection::LEFT)
        {
//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
            {
                localPlayer->setDirection(BeingDirection::LEFT);
                if (playerHandler)
                {
                    playerHandler->setDirection(
                        BeingDirection::LEFT);
                }
            }
        }
        return true;
    }
    return false;
}

impHandler0(directRight)
{
    if (localPlayer)
    {
        if (localPlayer->getDirection() != BeingDirection::RIGHT)
        {
//            if (PacketLimiter::limitPackets(PacketType::PACKET_DIRECTION))
            {
                localPlayer->setDirection(BeingDirection::RIGHT);
                if (playerHandler)
                {
                    playerHandler->setDirection(
                        BeingDirection::RIGHT);
                }
            }
        }
        return true;
    }
    return false;
}

}  // namespace Actions
