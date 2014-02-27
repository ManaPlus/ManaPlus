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

#ifndef GUI_MODELS_MAGICSCHOOLMODEL_H
#define GUI_MODELS_MAGICSCHOOLMODEL_H

#include "gui/models/listmodel.h"

#include "utils/gettext.h"

#include "localconsts.h"

const char *MAGIC_SCHOOL_TEXT[6] =
{
    // TRANSLATORS: magic school
    N_("General Magic"),
    // TRANSLATORS: magic school
    N_("Life Magic"),
    // TRANSLATORS: magic school
    N_("War Magic"),
    // TRANSLATORS: magic school
    N_("Transmute Magic"),
    // TRANSLATORS: magic school
    N_("Nature Magic"),
    // TRANSLATORS: magic school
    N_("Astral Magic")
};

class MagicSchoolModel final : public ListModel
{
    public:
        ~MagicSchoolModel()
        { }

        int getNumberOfElements() override final
        {
            return 6;
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return MAGIC_SCHOOL_TEXT[i];
        }
};

#endif  // GUI_MODELS_MAGICSCHOOLMODEL_H
