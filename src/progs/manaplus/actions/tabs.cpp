/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "actions/tabs.h"

#include "actions/actiondef.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/shortcutwindow.h"

#include "debug.h"

namespace Actions
{

impHandler0(prevSocialTab)
{
    if (socialWindow != nullptr)
    {
        socialWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextSocialTab)
{
    if (socialWindow != nullptr)
    {
        socialWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(nextShortcutsTab)
{
    if (itemShortcutWindow != nullptr)
    {
        itemShortcutWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(prevShortcutsTab)
{
    if (itemShortcutWindow != nullptr)
    {
        itemShortcutWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextCommandsTab)
{
    if (spellShortcutWindow != nullptr)
    {
        spellShortcutWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(prevCommandsTab)
{
    if (spellShortcutWindow != nullptr)
    {
        spellShortcutWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextInvTab)
{
    InventoryWindow::nextTab();
    return true;
}

impHandler0(prevInvTab)
{
    InventoryWindow::prevTab();
    return true;
}

}  // namespace Actions
