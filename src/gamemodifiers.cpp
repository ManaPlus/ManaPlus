/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gamemodifiers.h"

#include "configuration.h"
#include "settings.h"

#include "being/localplayer.h"

#include "gui/widgets/tabs/chattab.h"

#include "listeners/updatestatuslistener.h"

#include "utils/gettext.h"

#include "debug.h"

GameModifiers *modifiers = nullptr;

GameModifiers::GameModifiers()
{
    settings.crazyMoveType = config.getIntValue("crazyMoveType");
}

GameModifiers::~GameModifiers()
{
}

void GameModifiers::changeMode(unsigned *restrict const var,
                               const unsigned limit,
                               const char *restrict const conf,
                               std::string (GameModifiers::*const func)(),
                               const unsigned def,
                               const bool save,
                               const bool forward)
{
    if (!var)
        return;

    if (forward)
    {
        (*var) ++;
        if (*var >= limit)
            *var = def;
    }
    else
    {
        if (!*var)
            *var = limit - 1;
        else
            (*var) --;
    }

    if (save)
        config.setValue(conf, *var);
    UpdateStatusListener::distributeEvent();
    const std::string str = (this->*func)();
    if (str.size() > 4)
        debugMsg(str.substr(4));
}

const char *GameModifiers::getVarItem(const char *const *const arr,
                                      const unsigned index,
                                      const unsigned sz)
{
    if (index < sz)
        return arr[index];
    return arr[sz];
}

static const unsigned moveTypeSize = 5;

void GameModifiers::changeMoveType(const bool forward)
{
    player_node->setMoveState(0);
    changeMode(&settings.moveType, moveTypeSize, "invertMoveDirection",
        &GameModifiers::getMoveTypeString, 0, false, forward);
}

static const char *const moveTypeStrings[] =
{
    // TRANSLATORS: move type in status bar
    N_("(D) default moves"),
    // TRANSLATORS: move type in status bar
    N_("(I) invert moves"),
    // TRANSLATORS: move type in status bar
    N_("(c) moves with some crazy moves"),
    // TRANSLATORS: move type in status bar
    N_("(C) moves with crazy moves"),
    // TRANSLATORS: move type in status bar
    N_("(d) double normal + crazy"),
    // TRANSLATORS: move type in status bar
    N_("(?) unknown move")
};

std::string GameModifiers::getMoveTypeString()
{
    return gettext(getVarItem(&moveTypeStrings[0],
        settings.moveType, moveTypeSize));
}

static const unsigned crazyMoveTypeSize = 11;

void GameModifiers::changeCrazyMoveType(const bool forward)
{
    player_node->setCrazyMoveState(0U);
    changeMode(&settings.crazyMoveType, crazyMoveTypeSize, "crazyMoveType",
        &GameModifiers::getCrazyMoveTypeString, 1, true, forward);
}

std::string GameModifiers::getCrazyMoveTypeString()
{
    const unsigned int crazyMoveType = settings.crazyMoveType;
    if (crazyMoveType < crazyMoveTypeSize - 1)
    {
        // TRANSLATORS: crazy move type in status bar
        return strprintf(_("(%u) crazy move number %u"),
            crazyMoveType, crazyMoveType);
    }
    else if (crazyMoveType == crazyMoveTypeSize - 1)
    {
        // TRANSLATORS: crazy move type in status bar
        return _("(a) custom crazy move");
    }
    else
    {
        // TRANSLATORS: crazy move type in status bar
        return _("(?) crazy move");
    }
}
