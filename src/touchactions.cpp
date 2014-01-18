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

#include "touchactions.h"

#include "mouseinput.h"
#include "touchmanager.h"

#include "input/inputmanager.h"

#include "debug.h"

bool padClicked(false);
int halfJoyPad = 50;

#define impHandler(name) void name(const MouseInput &mouseInput)
#define impHandler0(name) void name(const MouseInput &mouseInput A_UNUSED)

void setHalfJoyPad(const int s)
{
    halfJoyPad = s;
}

static void moveChar(int x, int y)
{
    static const int lim1 = 10;
    static const int diff = 20;

    // set center at (0,0)
    x -= halfJoyPad;
    y -= halfJoyPad;

    // some magic for checking at what sector was click
    if (abs(x) < lim1)
        x = 0;

    if (abs(y) < lim1)
        y = 0;

    const int x2 = abs(x);
    const int y2 = abs(y);
    if (x2 > y2)
    {
        if (y2 && x2 * 10 / y2 > diff)
            y = 0;
    }
    else
    {
        if (x2 && y2 * 10 / x2 > diff)
            x = 0;
    }

    // detecting direction
    if (x > 0)
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, false);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, true);
    }
    else if (x < 0)
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, true);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, false);
    }
    else
    {
        touchManager.setActionActive(Input::KEY_MOVE_LEFT, false);
        touchManager.setActionActive(Input::KEY_MOVE_RIGHT, false);
    }
    if (y > 0)
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, true);
        touchManager.setActionActive(Input::KEY_MOVE_UP, false);
    }
    else if (y < 0)
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, false);
        touchManager.setActionActive(Input::KEY_MOVE_UP, true);
    }
    else
    {
        touchManager.setActionActive(Input::KEY_MOVE_DOWN, false);
        touchManager.setActionActive(Input::KEY_MOVE_UP, false);
    }
    inputManager.updateConditionMask();
}

impHandler(padClick)
{
    moveChar(mouseInput.getX(), mouseInput.getY());
    padClicked = true;
}

impHandler(padEvents)
{
    if (mouseInput.getType() == gcn::MouseInput::MOVED)
    {
        if (padClicked)
            moveChar(mouseInput.getX(), mouseInput.getY());
    }
}

impHandler0(padOut)
{
    padClicked = false;
    moveChar(halfJoyPad, halfJoyPad);
}

impHandler0(padUp)
{
    padClicked = false;
    moveChar(halfJoyPad, halfJoyPad);
}
