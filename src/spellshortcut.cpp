/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers.
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

#include "spellshortcut.h"

#include "configuration.h"
#include "inventory.h"
#include "item.h"
#include "localplayer.h"

#include "gui/widgets/chattab.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "debug.h"

SpellShortcut *spellShortcut;

SpellShortcut::SpellShortcut() :
    mItemSelected(-1)
{
    load();
}

SpellShortcut::~SpellShortcut()
{
}

void SpellShortcut::load()
{
    for (unsigned f = 0; f < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; f ++)
        mItems[f] = -1;

    if (!spellManager)
        return;

    std::vector<TextCommand*> spells = spellManager->getAll();
    unsigned k = 0;

    for (std::vector<TextCommand*>::const_iterator i = spells.begin(),
         i_end = spells.end(); i != i_end
         && k < SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS; ++i)
    {
        mItems[k++] = (*i)->getId();
    }

}

unsigned int SpellShortcut::getSpellsCount() const
{
    return SPELL_SHORTCUT_ITEMS;
}
