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

#ifndef GUI_WIDGETS_TABS_SOCIALFRIENDSFUNCTOR_H
#define GUI_WIDGETS_TABS_SOCIALFRIENDSFUNCTOR_H

#include "avatar.h"

#include "utils/stringutils.h"

#include "localconsts.h"

class SortFriendsFunctor final
{
    public:
        A_DEFAULT_COPY(SortFriendsFunctor)

        bool operator() (const Avatar *const m1,
                         const Avatar *const m2) const
        {
            if ((m1 == nullptr) || (m2 == nullptr))
                return false;

            if (m1->getOnline() != m2->getOnline())
            {
                return static_cast<int>(m1->getOnline()) >
                    static_cast<int>(m2->getOnline());
            }

            if (m1->getName() != m2->getName())
            {
                std::string s1 = m1->getName();
                std::string s2 = m2->getName();
                toLower(s1);
                toLower(s2);
                return s1 < s2;
            }
            return false;
        }
};

#endif  // GUI_WIDGETS_TABS_SOCIALFRIENDSFUNCTOR_H
