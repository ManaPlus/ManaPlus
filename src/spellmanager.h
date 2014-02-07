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

#ifndef SPELLMANAGER_H
#define SPELLMANAGER_H

#include <string>
#include <vector>

#include "textcommand.h"

#include "being/being.h"

const int SPELL_MIN_ID = 100000;
const unsigned int SPELL_SHORTCUT_ITEMS = 49;
const unsigned int SPELL_SHORTCUT_TABS = 5;

class SpellManager final
{
    public:
        SpellManager();

        A_DELETE_COPY(SpellManager)

        ~SpellManager();

        TextCommand *getSpell(const int spellId) const A_WARN_UNUSED;

        const TextCommand* getSpellByItem(const int itemId)
                                          const A_WARN_UNUSED;

        bool addSpell(TextCommand *const spell);

        TextCommand *createNewSpell() const A_WARN_UNUSED;

        const std::vector<TextCommand*> &getAll() const A_WARN_UNUSED;

        void useItem(const int itemId) const;

        void invoke(const int spellId) const;

        void load(const bool oldConfig = false);

        void save() const;

        std::string autoComplete(const std::string &partName)
                                 const A_WARN_UNUSED;

        void swap(const int id1, const int id2);

    private:
        void fillSpells();

        static void invokeSpell(const TextCommand *const spell,
                                const Being *const target);

        static void invokeSpell(const TextCommand *const spell);

        static std::string parseCommand(std::string command,
                                        const Being *const target)
                                        A_WARN_UNUSED;

        std::map<unsigned int, TextCommand*> mSpells;
        std::vector<TextCommand*> mSpellsVector;
};

extern SpellManager *spellManager;

#endif  // SPELLMANAGER_H
