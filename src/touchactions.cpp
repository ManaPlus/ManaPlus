/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "touchactions.h"

#include "being.h"
#include "game.h"
#include "keydata.h"
#include "logger.h"
#include "touchmanager.h"

#ifdef ANDROID
#include <SDL_screenkeyboard.h>
#endif

#include "debug.h"

bool padClicked(false);

#ifdef ANDROID
void showKeyboard(const gcn::MouseInput &mouseInput)
{
    SDL_ANDROID_ToggleScreenKeyboardTextInput(nullptr);
}
#else
void showKeyboard(const gcn::MouseInput &mouseInput A_UNUSED)
{
}
#endif

static void moveChar(int x, int y)
{
    Game *const game = Game::instance();
    if (!game)
        return;

    static const int lim = 10;
    x -= 50;
    y -= 50;

    if (x > lim)
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, false);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, true);
    }
    else if (x < -lim)
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, true);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, false);
    }
    else
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, false);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, false);
    }
    if (y > lim)
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, true);
        touchManager.setActionActive(Input::KEY_MOVE_UP, false);
    }
    else if (y < -lim)
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, false);
        touchManager.setActionActive(Input::KEY_MOVE_UP, true);
    }
    else
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, false);
        touchManager.setActionActive(Input::KEY_MOVE_UP, false);
    }
}

void padClick(const gcn::MouseInput &mouseInput)
{
    moveChar(mouseInput.getX(), mouseInput.getY());
    padClicked = true;
}

void padEvents(const gcn::MouseInput &mouseInput)
{
    if (mouseInput.getType() == gcn::MouseInput::MOVED)
    {
        if (padClicked)
            moveChar(mouseInput.getX(), mouseInput.getY());
    }
}

void padOut(const gcn::MouseInput &mouseInput A_UNUSED)
{
    padClicked = false;
    moveChar(50, 50);
}

void padUp(const gcn::MouseInput &mouseInput A_UNUSED)
{
    padClicked = false;
    moveChar(50, 50);
}
