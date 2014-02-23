/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_MODELS_COLORLISTMODEL_H
#define GUI_MODELS_COLORLISTMODEL_H

#include "gui/models/listmodel.h"

#include "utils/gettext.h"

const char *COLOR_NAME[14] =
{
    // TRANSLATORS: chat color
    N_("default"),
    // TRANSLATORS: chat color
    N_("black"),
    // TRANSLATORS: chat color
    N_("red"),
    // TRANSLATORS: chat color
    N_("green"),
    // TRANSLATORS: chat color
    N_("blue"),
    // TRANSLATORS: chat color
    N_("gold"),
    // TRANSLATORS: chat color
    N_("yellow"),
    // TRANSLATORS: chat color
    N_("pink"),
    // TRANSLATORS: chat color
    N_("purple"),
    // TRANSLATORS: chat color
    N_("grey"),
    // TRANSLATORS: chat color
    N_("brown"),
    // TRANSLATORS: chat color
    N_("rainbow 1"),
    // TRANSLATORS: chat color
    N_("rainbow 2"),
    // TRANSLATORS: chat color
    N_("rainbow 3"),
};

class ColorListModel final : public ListModel
{
    public:
        ~ColorListModel()
        { }

        int getNumberOfElements()
        {
            return 14;
        }

        std::string getElementAt(int i)
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return gettext(COLOR_NAME[i]);
        }
};

#endif  // GUI_MODELS_COLORLISTMODEL_H
