/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "gui/setupactiondata.h"

#include "utils/gettext.h"

#include "debug.h"

SetupActionData setupActionDataBasic[] =
{
    {
        // TRANSLATORS: input action name
        N_("Target and attack keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Attack"),
        InputAction::ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target & attack closest monster"),
        InputAction::TARGET_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target & attack closest player"),
        InputAction::ATTACK_HUMAN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Target"),
        InputAction::MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Move to Target type"),
        InputAction::CHANGE_MOVE_TO_TARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to Home location"),
        InputAction::MOVE_TO_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Set home location"),
        InputAction::SET_HOME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Move to navigation point"),
        InputAction::MOVE_TO_POINT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Talk"),
        InputAction::TALK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop Attack / Modifier key"),
        InputAction::STOP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Untarget"),
        InputAction::UNTARGET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target monster"),
        InputAction::TARGET_MONSTER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target closest monster (without filters)"),
        InputAction::TARGET_CLOSEST_MONSTER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target NPC"),
        InputAction::TARGET_NPC,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target Player"),
        InputAction::TARGET_PLAYER,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target mercenary"),
        InputAction::TARGET_MERCENARY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target skill unit"),
        InputAction::TARGET_SKILL_UNIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change targeting type"),
        InputAction::CHANGE_TARGETING_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Target pet"),
        InputAction::TARGET_PET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Catch pet"),
        InputAction::CATCH_PET,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other Keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pickup"),
        InputAction::PICKUP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Pickup Type"),
        InputAction::CHANGE_PICKUP_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Sit"),
        InputAction::SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Screenshot"),
        InputAction::SCREENSHOT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable/Disable Trading"),
        InputAction::CHANGE_TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Open trade window"),
        InputAction::OPEN_TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Start trade with target"),
        InputAction::TRADE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Follow selected player"),
        InputAction::FOLLOW,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Map View Mode"),
        InputAction::PATHFIND,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Select OK"),
        InputAction::OK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quit"),
        InputAction::QUIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Stop or sit"),
        InputAction::STOP_SIT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Return to safe video mode"),
        InputAction::SAFE_VIDEO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Fire your homunculus"),
        InputAction::HOMUNCULUS_FIRE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Buy"),
        InputAction::BUY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Sell"),
        InputAction::SELL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Open context menu"),
        InputAction::CONTEXT_MENU,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
