/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef BATTLETAB_H
#define BATTLETAB_H

#include "gui/widgets/chattab.h"

/**
 * A tab for a party chat channel.
 */
class BattleTab : public ChatTab
{
    public:
        BattleTab();

        ~BattleTab();

        int getType() const
        { return ChatTab::TAB_BATTLE; }

        void saveToLogFile(std::string &msg);
};

extern BattleTab *battleChatTab;
#endif



