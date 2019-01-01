/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "listeners/tooltiplistener.h"

#include "gui/gui.h"
#include "gui/viewport.h"

#include "gui/popups/textboxpopup.h"

#include "debug.h"

ToolTipListener::ToolTipListener() :
    MouseListener(),
    mToolTip()
{
}

void ToolTipListener::mouseMoved(MouseEvent &event)
{
    if (mToolTip.empty() || (textBoxPopup == nullptr))
        return;

    int x = 0;
    int y = 0;
    if (viewport != nullptr)
    {
        x = viewport->mMouseX;
        y = viewport->mMouseY;
    }
    else
    {
        Gui::getMouseState(x, y);
    }

    event.consume();
    textBoxPopup->show(x, y, mToolTip);
}

void ToolTipListener::mouseExited(MouseEvent &event A_UNUSED)
{
    if (mToolTip.empty() || (textBoxPopup == nullptr))
        return;
    textBoxPopup->hide();
}
