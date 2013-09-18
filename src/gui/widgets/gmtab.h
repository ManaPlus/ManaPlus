/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_GMTAB_H
#define GUI_WIDGETS_GMTAB_H

#include "gui/widgets/chattab.h"

/**
 * A tab for whispers from a single player.
 */
class GmTab final : public ChatTab
{
    public:
        explicit GmTab(const Widget2 *const widget);

        A_DELETE_COPY(GmTab)

        ~GmTab();

        int getType() const override A_WARN_UNUSED
        { return ChatTab::TAB_GM; }

        void saveToLogFile(const std::string &msg) const override;

    protected:
        void getAutoCompleteList(StringVect &names) const override;

        void handleInput(const std::string &msg) override;

        void handleCommand(const std::string &msg) override;
};

extern GmTab *gmChatTab;

#endif  // GUI_WIDGETS_GMTAB_H
