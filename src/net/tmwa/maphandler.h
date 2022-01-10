/*
 *  The ManaPlus Client
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

#ifndef NET_TMWA_MAPHANDLER_H
#define NET_TMWA_MAPHANDLER_H

#include "net/maphandler.h"

namespace TmwAthena
{

class MapHandler final : public Net::MapHandler
{
    public:
        MapHandler();

        A_DELETE_COPY(MapHandler)

        ~MapHandler() override final;

        void blockPlayCancel() const override final;

        void destroyInstance() const override final;

        void camera(const int action,
                    const float range,
                    const float rotation,
                    const float latitude) const override final;

        void clientVersion(const int version) const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_MAPHANDLER_H
