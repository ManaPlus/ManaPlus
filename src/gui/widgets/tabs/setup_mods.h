/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SETUP_MODS_H
#define GUI_WIDGETS_TABS_SETUP_MODS_H

#include "gui/widgets/tabs/setuptabscroll.h"

class Setup_Mods final : public SetupTabScroll
{
    public:
        explicit Setup_Mods(const Widget2 *const widget);

        A_DELETE_COPY(Setup_Mods)

        ~Setup_Mods() override final;

        void apply() override final;

        void externalUpdated() override final;

        void externalUnloaded() override final;

        void loadMods();

        void saveMods() const;

    protected:
};

#endif  // GUI_WIDGETS_TABS_SETUP_MODS_H
