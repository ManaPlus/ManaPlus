/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_TABS_CHAT_PARTYTAB_H
#define GUI_WIDGETS_TABS_CHAT_PARTYTAB_H

#include "gui/widgets/tabs/chat/chattab.h"

/**
 * A tab for a party chat channel.
 */
class PartyTab notfinal : public ChatTab,
                          public ConfigListener
{
    public:
        explicit PartyTab(const Widget2 *const widget);

        A_DELETE_COPY(PartyTab)

        ~PartyTab() override;

        bool handleCommand(const std::string &restrict type,
                           const std::string &restrict args) override final;

        void playNewMessageSound() const override final;

        void optionChanged(const std::string &value) override final;

    protected:
        void handleInput(const std::string &msg) override final;

        void getAutoCompleteList(StringVect &names) const override final;

        void getAutoCompleteCommands(StringVect &names) const override final;
};

extern PartyTab *partyTab;

#endif  // GUI_WIDGETS_TABS_CHAT_PARTYTAB_H
