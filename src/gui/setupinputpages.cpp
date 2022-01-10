/*
 *  The ManaPlus Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "const/gui/pages.h"

#include "gui/setupinputpages.h"

#include "utils/gettext.h"

#include "input/pages/basic.h"
#include "input/pages/chat.h"
#include "input/pages/craft.h"
#include "input/pages/emotes.h"
#include "input/pages/gui.h"
#include "input/pages/move.h"
#include "input/pages/other.h"
#include "input/pages/outfits.h"
#include "input/pages/shortcuts.h"
#include "input/pages/windows.h"

#include "debug.h"

const char *const pages[SETUP_PAGES + 1] =
{
    // TRANSLATORS: input tab sub tab name
    N_("Basic"),
    // TRANSLATORS: input tab sub tab name
    N_("Move"),
    // TRANSLATORS: input tab sub tab name
    N_("Shortcuts"),
    // TRANSLATORS: input tab sub tab name
    N_("Windows"),
    // TRANSLATORS: input tab sub tab name
    N_("Craft"),
    // TRANSLATORS: input tab sub tab name
    N_("Emotes"),
    // TRANSLATORS: input tab sub tab name
    N_("Outfits"),
    // TRANSLATORS: input tab sub tab name
    N_("Chat"),
    // TRANSLATORS: input tab sub tab name
    N_("Other"),
    // TRANSLATORS: input tab sub tab name
    N_("Gui"),
    nullptr
};

SetupActionData *const setupActionData[SETUP_PAGES] =
{
    setupActionDataBasic,
    setupActionDataMove,
    setupActionDataShortcuts,
    setupActionDataWindows,
    setupActionDataCraft,
    setupActionDataEmotes,
    setupActionDataOutfits,
    setupActionDataChat,
    setupActionDataOther,
    setupActionDataGui
};
