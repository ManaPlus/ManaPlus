/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/setupactiondata.h"

#include "utils/gettext.h"

#include "debug.h"

SetupActionData setupActionDataOther[] =
{
    {
        // TRANSLATORS: input action name
        N_("Input ignore"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore input 1"),
        InputAction::IGNORE_INPUT_1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Ignore input 2"),
        InputAction::IGNORE_INPUT_2,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Direction keys"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Player direct up"),
        InputAction::DIRECT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Player direct down"),
        InputAction::DIRECT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Player direct left"),
        InputAction::DIRECT_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Player direct right"),
        InputAction::DIRECT_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet direct up"),
        InputAction::PET_DIRECT_UP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet direct down"),
        InputAction::PET_DIRECT_DOWN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet direct left"),
        InputAction::PET_DIRECT_LEFT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet direct right"),
        InputAction::PET_DIRECT_RIGHT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Other"),
        InputAction::NO_VALUE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Crazy moves"),
        InputAction::CRAZY_MOVES,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Crazy Move mode"),
        InputAction::CHANGE_CRAZY_MOVES_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items from 0 slot"),
        InputAction::QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick Drop N Items"),
        InputAction::QUICK_DROPN,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch Quick Drop Counter"),
        InputAction::SWITCH_QUICK_DROP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick heal target or self"),
        InputAction::MAGIC_INMA1,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Heal the most injured player"),
        InputAction::HEAL_MOST_DAMAGED,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use #itenplz spell"),
        InputAction::MAGIC_ITENPLZ,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Use magic attack"),
        InputAction::MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch magic attack"),
        InputAction::SWITCH_MAGIC_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Switch pvp attack"),
        InputAction::SWITCH_PVP_ATTACK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change move type"),
        InputAction::INVERT_DIRECTION,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Weapon Type"),
        InputAction::CHANGE_ATTACK_WEAPON_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Attack Type"),
        InputAction::CHANGE_ATTACK_TYPE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Follow mode"),
        InputAction::CHANGE_FOLLOW_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Change Imitation mode"),
        InputAction::CHANGE_IMITATION_MODE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Disable / Enable Game modifier keys"),
        InputAction::DISABLE_GAME_MODIFIERS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("On / Off audio"),
        InputAction::CHANGE_AUDIO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Enable / Disable away mode"),
        InputAction::AWAY,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Emulate right click from keyboard"),
        InputAction::RIGHT_CLICK,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Toggle camera mode"),
        InputAction::CAMERA,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Toggle ipc mode"),
        InputAction::IPC_TOGGLE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show information about position in chat"),
        InputAction::WHERE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show online players number in chat"),
        InputAction::WHO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Show onscreen keyboard"),
        InputAction::SHOW_KEYBOARD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Clean cached graphics"),
        InputAction::CLEAN_GRAPHICS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Clean cached fonts"),
        InputAction::CLEAN_FONTS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print visible players in chat"),
        InputAction::PRESENT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print all visible beings in chat"),
        InputAction::PRINT_ALL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Load shop items list from disk"),
        InputAction::PRICE_LOAD,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Save shop items list to disk"),
        InputAction::PRICE_SAVE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print debug cache info"),
        InputAction::CACHE_INFO,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Undress selected player"),
        InputAction::UNDRESS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Quick disconnect from server"),
        InputAction::DISCONNECT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Disable debug particle"),
        InputAction::TEST_PARTICLE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Create items (for gms)"),
        InputAction::CREATE_ITEMS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print configured directories in chat"),
        InputAction::DIRS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print client uptime in chat"),
        InputAction::UPTIME,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Dump debug information"),
        InputAction::DUMP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Crash client"),
        InputAction::ERROR,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Dump graphics info into chat"),
        InputAction::DUMP_GRAPHICS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Dump tests info into chat"),
        InputAction::DUMP_TESTS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print OpenGL version in chat"),
        InputAction::DUMP_GL,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Print enabled mods in chat"),
        InputAction::DUMP_MODS,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Dump environments into log"),
        InputAction::DUMP_ENVIRONMENT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Dump OpenGL state into log"),
        InputAction::DUMP_OGL,
        "",
    },
#if defined USE_OPENGL && defined DEBUG_SDLFONT
    {
        // TRANSLATORS: input action name
        N_("Test SDL font speed"),
        InputAction::TEST_SDL_FONT,
        "",
    },
#endif  // defined USE_OPENGL && defined DEBUG_SDLFONT
    {
        // TRANSLATORS: input action name
        N_("Upload main config"),
        InputAction::UPLOAD_CONFIG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Upload server config"),
        InputAction::UPLOAD_SERVER_CONFIG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Upload log file"),
        InputAction::UPLOAD_LOG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Fire your mercenary"),
        InputAction::MERCENARY_FIRE,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet ai start"),
        InputAction::PET_AI_START,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Pet ai stop"),
        InputAction::PET_AI_STOP,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Feed homunculus"),
        InputAction::HOMUNCULUS_FEED,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Feed pet"),
        InputAction::PET_FEED,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Drop pet's loot"),
        InputAction::PET_DROP_LOOT,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Return pet to egg"),
        InputAction::PET_RETURN_TO_EGG,
        "",
    },
    {
        // TRANSLATORS: input action name
        N_("Unequip pet"),
        InputAction::PET_UNEQUIP,
        "",
    },
    {
        "",
        InputAction::NO_VALUE,
        ""
    }
};
