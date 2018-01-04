/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_WIDGETS_TABS_CHAT_CHANNELTAB_H
#define GUI_WIDGETS_TABS_CHAT_CHANNELTAB_H

#include "gui/widgets/tabs/chat/chattab.h"

/**
 * A tab for a chat channel.
 */
class ChannelTab final : public ChatTab
{
    public:
        ChannelTab(const Widget2 *const widget,
            const std::string &channel);

        A_DELETE_COPY(ChannelTab)

        ~ChannelTab() override final;

    protected:
        void handleInput(const std::string &msg) override final;

        bool handleCommand(const std::string &restrict type,
                           const std::string &restrict args) override final;

        void getAutoCompleteCommands(StringVect& commands)
                                     const override final;
};

#endif  // GUI_WIDGETS_TABS_CHAT_CHANNELTAB_H
