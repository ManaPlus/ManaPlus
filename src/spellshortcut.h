/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef SPELLSHORTCUT_H
#define SPELLSHORTCUT_H

#include "spellmanager.h"

const unsigned int SPELLS_SIZE = SPELL_SHORTCUT_ITEMS * SPELL_SHORTCUT_TABS;

/**
 * The class which keeps track of the item shortcuts.
 */
class SpellShortcut final
{
    public:
        /**
         * Constructor.
         */
        SpellShortcut();

        A_DELETE_COPY(SpellShortcut)

        /**
         * Destructor.
         */
        ~SpellShortcut();

        /**
         * Load the configuration information.
         */
        void load();

        unsigned int getSpellsCount() const A_WARN_UNUSED;

        /**
         * Set the item that is selected.
         *
         * @param itemId The ID of the item that is to be assigned.
         */
        void setItemSelected(const int itemId)
        { mItemSelected = itemId; }

        /**
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Returns selected shortcut item ID.
         */
        int getSelectedItem() const A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * Returns the shortcut item ID specified by the index.
         *
         * @param index Index of the shortcut item.
         */
        int getItem(const int index) const
        { return mItems[index]; }

    private:
        int mItems[SPELLS_SIZE];
        int mItemSelected;              /**< The item held by cursor. */
};

extern SpellShortcut *spellShortcut;
extern SpellManager *spellManager;

#endif  // SPELLSHORTCUT_H
