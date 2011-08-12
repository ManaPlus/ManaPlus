/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SPELLSHORTCUT_H
#define SPELLSHORTCUT_H

#include "spellmanager.h"
#include "logger.h"
#include "utils/stringutils.h"

class TextCommand;

/**
 * The class which keeps track of the item shortcuts.
 */
class SpellShortcut
{
    public:
        /**
         * Constructor.
         */
        SpellShortcut();

        /**
         * Destructor.
         */
        ~SpellShortcut();

        /**
         * Load the configuration information.
         */
        void load();

        unsigned int getSpellsCount() const;

        /**
         * Set the item that is selected.
         *
         * @param itemId The ID of the item that is to be assigned.
         */
        void setItemSelected(int itemId)
        { mItemSelected = itemId; }

        /**
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const
        { return mItemSelected > -1; }

        /**
         * Returns selected shortcut item ID.
         */
        int getSelectedItem() const
        { return mItemSelected; }

        /**
         * Returns the shortcut item ID specified by the index.
         *
         * @param index Index of the shortcut item.
         */
        int getItem(int index) const
        { return mItems[index]; }

    private:
        int mItems[SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS];
        int mItemSelected;              /**< The item held by cursor. */
};

extern SpellShortcut *spellShortcut;
extern SpellManager *spellManager;

#endif
