/*
 *  The ManaPlus Client
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

#ifndef GUI_MODELS_TARGETTYPEMODEL_H
#define GUI_MODELS_TARGETTYPEMODEL_H

#include "gui/models/listmodel.h"

#include "utils/gettext.h"

const char *TARGET_TYPE_TEXT[3] =
{
    // TRANSLATORS: target type
    N_("No Target"),
    // TRANSLATORS: target type
    N_("Allow Target"),
    // TRANSLATORS: target type
    N_("Need Target")
};

class TargetTypeModel final : public ListModel
{
    public:
        ~TargetTypeModel()
        { }

        int getNumberOfElements() override final
        {
            return 3;
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return TARGET_TYPE_TEXT[i];
        }
};

#endif  // GUI_MODELS_TARGETTYPEMODEL_H
