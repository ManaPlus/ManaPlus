/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_MODELS_WORLDLISTMODEL_H
#define GUI_MODELS_WORLDLISTMODEL_H

#include "gui/models/listmodel.h"

#include "utils/cast.h"
#include "utils/stringutils.h"

#include "net/worldinfo.h"

/**
 * The list model for the server list.
 */
class WorldListModel final : public ListModel
{
    public:
        explicit WorldListModel(const Worlds &worlds) :
            mWorlds(worlds)
        {
        }

        A_DELETE_COPY(WorldListModel)

        int getNumberOfElements() override final
        {
            return CAST_S32(mWorlds.size());
        }

        std::string getElementAt(int i) override final
        {
            const WorldInfo *const si = mWorlds[i];
            if (si != nullptr)
            {
                return std::string(si->name).append(" (").append(
                    toString(si->online_users)).append(")");
            }
            return "???";
        }
    private:
        Worlds mWorlds;
};

#endif  // GUI_MODELS_WORLDLISTMODEL_H
