/*
 *  The ManaPlus Client
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

#ifndef GUI_MODELS_UPDATETYPEMODEL_H
#define GUI_MODELS_UPDATETYPEMODEL_H

#include "utils/gettext.h"

#include "gui/models/listmodel.h"

const char *UPDATE_TYPE_TEXT[3] =
{
    // TRANSLATORS: update type
    N_("Normal"),
    // TRANSLATORS: update type
    N_("Auto Close"),
    // TRANSLATORS: update type
    N_("Skip"),
};

class UpdateTypeModel final : public ListModel
{
    public:
        UpdateTypeModel()
        { }

        A_DELETE_COPY(UpdateTypeModel)

        int getNumberOfElements() override final
        {
            return 3;
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return gettext(UPDATE_TYPE_TEXT[i]);
        }
};

#endif  // GUI_MODELS_UPDATETYPEMODEL_H
