/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#include "being.h"

const int SPELL_MIN_ID = 100000;
const unsigned int SPELL_SHORTCUT_ITEMS = 49;
const unsigned int SPELL_SHORTCUT_TABS = 5;

class SpellManager final
{
    public:
        SpellManager();

        A_DELETE_COPY(SpellManager);

        ~SpellManager();

        TextCommand *getSpell(const int spellId);

        TextCommand* getSpellByItem(const int itemId);

        bool addSpell(TextCommand *const spell);

        TextCommand *createNewSpell() const;

        std::vector<TextCommand*> getAll() const;

        void useItem(const int itemId);

        void invoke(const int spellId);

        void load(const bool oldConfig = false);

        void save();

        std::string autoComplete(std::string partName);

    private:
        void fillSpells();

        void invokeSpell(const TextCommand *const spell,
                         const Being *const target) const;

        void invokeSpell(const TextCommand *const spell) const;

        std::string parseCommand(std::string command,
                                 const Being *const target) const;

        std::map<unsigned int, TextCommand*> mSpells;
        std::vector<TextCommand*> mSpellsVector;
};

extern SpellManager *spellManager;

#endif // SPELLMANAGER_H
