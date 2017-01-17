/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef GUI_MODELS_TYPELISTMODEL_H
#define GUI_MODELS_TYPELISTMODEL_H

#include "gui/models/listmodel.h"

/**
 * Server Type List Model
 */
class TypeListModel final : public ListModel
{
    public:
        TypeListModel() :
            ListModel()
        { }

        A_DELETE_COPY(TypeListModel)

        /**
         * Used to get number of line in the list
         */
        int getNumberOfElements() override final A_WARN_UNUSED
#if defined(TMWA_SUPPORT)
        { return 3; }
#else  // defined(TMWA_SUPPORT)
        { return 2; }
#endif  // defined(TMWA_SUPPORT)

        /**
         * Used to get an element from the list
         */
        std::string getElementAt(int elementIndex)
                                 override final A_WARN_UNUSED
        {
#ifdef TMWA_SUPPORT
            if (elementIndex == 0)
                return "TmwAthena";
            else if (elementIndex == 1)
                return "Hercules";
            else if (elementIndex == 2)
                return "Evol2";
            else
#else   // TMWA_SUPPORT
            if (elementIndex == 0)
                return "Hercules";
            else if (elementIndex == 1)
                return "Evol2";
            else
#endif  // TMWA_SUPPORT
                return "Unknown";
        }
};

#endif  // GUI_MODELS_TYPELISTMODEL_H
