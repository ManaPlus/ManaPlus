/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_SKILLHANDLER_H
#define NET_SKILLHANDLER_H

#include "enums/simpletypes/beingid.h"

#include <iosfwd>

#include "localconsts.h"

namespace Net
{
class SkillHandler notfinal
{
    public:
        SkillHandler()
        { }

        A_DELETE_COPY(SkillHandler)

        virtual ~SkillHandler()
        { }

        virtual void useBeing(const int id,
                              const int level,
                              const BeingId beingId) const = 0;

        virtual void useBeingStart(const int id,
                                   const int level,
                                   const BeingId beingId) const = 0;

        virtual void useStop(const int id) const = 0;

        virtual void usePos(const int id,
                            const int level,
                            const int x, const int y) const = 0;

        virtual void usePos(const int id,
                            const int level,
                            const int x, const int y,
                            const std::string &text) const = 0;

        virtual void useMap(const int id, const std::string &map) const = 0;

        virtual void getAlchemistRanks() const = 0;

        virtual void getBlacksmithRanks() const = 0;

        virtual void getPkRanks() const = 0;

        virtual void getTaekwonRanks() const = 0;

        virtual void feelSaveOk(const int which) const = 0;

        virtual void lessEffects(const bool isLess) const = 0;
};

}  // namespace Net

extern Net::SkillHandler *skillHandler;

#endif  // NET_SKILLHANDLER_H
