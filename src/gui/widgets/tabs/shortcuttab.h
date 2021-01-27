/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_WIDGETS_TABS_SHORTCUTTAB_H
#define GUI_WIDGETS_TABS_SHORTCUTTAB_H

#include "gui/widgets/tabs/tab.h"

#include "localconsts.h"

class ShortcutContainer;

class ShortcutTab final : public Tab
{
    public:
        ShortcutTab(const Widget2 *const widget,
                    const std::string &name,
                    ShortcutContainer *const content) :
            Tab(widget),
            mContent(content)
        {
            setCaption(name);
        }

        A_DELETE_COPY(ShortcutTab)

        ShortcutContainer* mContent;
};

#endif  // GUI_WIDGETS_TABS_SHORTCUTTAB_H
