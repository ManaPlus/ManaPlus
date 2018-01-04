/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_SHORTCUT_SPELLSHORTCUT_H
#define GUI_SHORTCUT_SPELLSHORTCUT_H

#include "const/spells.h"

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
        ~SpellShortcut() A_CONST;

        /**
         * Load the configuration information.
         */
        void load();

        unsigned int getSpellsCount() const A_CONST A_WARN_UNUSED;

        /**
         * Set the item that is selected.
         *
         * @param itemId The ID of the item that is to be assigned.
         */
        void setItemSelected(const int itemId) noexcept2
        { mItemSelected = itemId; }

        /**
         * A flag to check if the item is selected.
         */
        bool isItemSelected() const noexcept2 A_WARN_UNUSED
        { return mItemSelected > -1; }

        /**
         * Returns selected shortcut item ID.
         */
        int getSelectedItem() const noexcept2 A_WARN_UNUSED
        { return mItemSelected; }

        /**
         * Returns the shortcut item ID specified by the index.
         *
         * @param index Index of the shortcut item.
         */
        int getItem(const size_t index) const
        { return mItems[index]; }

    private:
        int mItems[SPELLS_SIZE];
        int mItemSelected;              /**< The item held by cursor. */
};

extern SpellShortcut *spellShortcut;

#endif  // GUI_SHORTCUT_SPELLSHORTCUT_H
