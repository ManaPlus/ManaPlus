/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_MODELS_SORTLISTMODELINV_H
#define GUI_MODELS_SORTLISTMODELINV_H

#include "gui/models/listmodel.h"

#include "utils/gettext.h"

#include <string>

static const char *const SORT_NAME_INVENTORY[6] =
{
    // TRANSLATORS: inventory sort mode
    N_("default"),
    // TRANSLATORS: inventory sort mode
    N_("by name"),
    // TRANSLATORS: inventory sort mode
    N_("by id"),
    // TRANSLATORS: inventory sort mode
    N_("by weight"),
    // TRANSLATORS: inventory sort mode
    N_("by amount"),
    // TRANSLATORS: inventory sort mode
    N_("by type")
};

class SortListModelInv final : public ListModel
{
    public:
        ~SortListModelInv()
        { }

        int getNumberOfElements() override final
        { return 6; }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";

            return gettext(SORT_NAME_INVENTORY[i]);
        }
};

#endif  // GUI_MODELS_SORTLISTMODELINV_H
