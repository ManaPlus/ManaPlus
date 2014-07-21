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
    settings.moveToTargetType = config.getIntValue("moveToTargetType");
    settings.followMode = config.getIntValue("followMode");
    settings.attackWeaponType = config.getIntValue("attackWeaponType");
    settings.attackType = config.getIntValue("attackType");
    settings.quickDropCounter = config.getIntValue("quickDropCounter");
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

static const unsigned moveToTargetTypeSize = 13;

void GameModifiers::changeMoveToTargetType(const bool forward)
{
    changeMode(&settings.moveToTargetType, moveToTargetTypeSize,
        "moveToTargetType",
        &GameModifiers::getMoveToTargetTypeString, 0, true, forward);
}

static const char *const moveToTargetTypeStrings[] =
{
    // TRANSLATORS: move to target type in status bar
    N_("(0) default moves to target"),
    // TRANSLATORS: move to target type in status bar
    N_("(1) moves to target in distance 1"),
    // TRANSLATORS: move to target type in status bar
    N_("(2) moves to target in distance 2"),
    // TRANSLATORS: move to target type in status bar
    N_("(3) moves to target in distance 3"),
    // TRANSLATORS: move to target type in status bar
    N_("(4) moves to target in distance 4"),
    // TRANSLATORS: move to target type in status bar
    N_("(5) moves to target in distance 5"),
    // TRANSLATORS: move to target type in status bar
    N_("(6) moves to target in distance 6"),
    // TRANSLATORS: move to target type in status bar
    N_("(7) moves to target in distance 7"),
    // TRANSLATORS: move to target type in status bar
    N_("(8) moves to target in distance 8"),
    // TRANSLATORS: move to target type in status bar
    N_("(9) moves to target in distance 9"),
    // TRANSLATORS: move to target type in status bar
    N_("(A) moves to target in attack range"),
    // TRANSLATORS: move to target type in status bar
    N_("(a) archer attack range"),
    // TRANSLATORS: move to target type in status bar
    N_("(B) moves to target in attack range - 1"),
    // TRANSLATORS: move to target type in status bar
    N_("(?) move to target")
};

std::string GameModifiers::getMoveToTargetTypeString()
{
    return gettext(getVarItem(&moveToTargetTypeStrings[0],
        settings.moveToTargetType, moveToTargetTypeSize));
}

static const unsigned followModeSize = 4;

void GameModifiers::changeFollowMode(const bool forward)
{
    changeMode(&settings.followMode, followModeSize, "followMode",
        &GameModifiers::getFollowModeString, 0, true, forward);
}

static const char *const followModeStrings[] =
{
    // TRANSLATORS: folow mode in status bar
    N_("(D) default follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(R) relative follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(M) mirror follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(P) pet follow"),
    // TRANSLATORS: folow mode in status bar
    N_("(?) unknown follow")
};

std::string GameModifiers::getFollowModeString()
{
    return gettext(getVarItem(&followModeStrings[0],
        settings.followMode, followModeSize));
}

const unsigned attackWeaponTypeSize = 4;

void GameModifiers::changeAttackWeaponType(const bool forward)
{
    changeMode(&settings.attackWeaponType, attackWeaponTypeSize,
        "attackWeaponType",
        &GameModifiers::getAttackWeaponTypeString, 1, true, forward);
}

static const char *const attackWeaponTypeStrings[] =
{
    // TRANSLATORS: switch attack type in status bar
    N_("(?) attack"),
    // TRANSLATORS: switch attack type in status bar
    N_("(D) default attack"),
    // TRANSLATORS: switch attack type in status bar
    N_("(s) switch attack without shield"),
    // TRANSLATORS: switch attack type in status bar
    N_("(S) switch attack with shield"),
    // TRANSLATORS: switch attack type in status bar
    N_("(?) attack")
};

std::string GameModifiers::getAttackWeaponTypeString()
{
    return gettext(getVarItem(&attackWeaponTypeStrings[0],
        settings.attackWeaponType, attackWeaponTypeSize));
}

const unsigned attackTypeSize = 4;

void GameModifiers::changeAttackType(const bool forward)
{
    changeMode(&settings.attackType, attackTypeSize, "attackType",
        &GameModifiers::getAttackTypeString, 0, true, forward);
}

static const char *const attackTypeStrings[] =
{
    // TRANSLATORS: attack type in status bar
    N_("(D) default attack"),
    // TRANSLATORS: attack type in status bar
    N_("(G) go and attack"),
    // TRANSLATORS: attack type in status bar
    N_("(A) go, attack, pickup"),
    // TRANSLATORS: attack type in status bar
    N_("(d) without auto attack"),
    // TRANSLATORS: attack type in status bar
    N_("(?) attack")
};

std::string GameModifiers::getAttackTypeString()
{
    return gettext(getVarItem(&attackTypeStrings[0],
        settings.attackType, attackTypeSize));
}

const unsigned quickDropCounterSize = 31;

void GameModifiers::changeQuickDropCounter(const bool forward)
{
    changeMode(&settings.quickDropCounter, quickDropCounterSize,
        "quickDropCounter",
        &GameModifiers::getQuickDropCounterString, 1, true, forward);
}

std::string GameModifiers::getQuickDropCounterString()
{
    const unsigned int cnt = settings.quickDropCounter;
    if (cnt > 9)
    {
        return strprintf("(%c) drop counter %u", static_cast<signed char>(
            'a' + cnt - 10), cnt);
    }
    else
    {
        return strprintf("(%u) drop counter %u", cnt, cnt);
    }
}

void GameModifiers::setQuickDropCounter(const int n)
{
    if (n < 1 || n >= static_cast<signed>(quickDropCounterSize))
        return;
    settings.quickDropCounter = n;
    config.setValue("quickDropCounter", n);
    UpdateStatusListener::distributeEvent();
}
