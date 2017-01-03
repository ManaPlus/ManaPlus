/*
 *  The ManaPlus Client
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

#ifndef INPUT_INPUTACTIONSORTFUNCTOR_H
#define INPUT_INPUTACTIONSORTFUNCTOR_H

#include "input/inputactiondata.h"

#include "localconsts.h"

class InputActionSortFunctor final
{
    public:
        bool operator() (const InputActionT key1,
                         const InputActionT key2) const
        {
            const size_t k1 = CAST_SIZE(key1);
            const size_t k2 = CAST_SIZE(key2);
            return keys[k1].priority > keys[k2].priority;
        }

        const InputActionData *keys A_NONNULLPOINTER;
};

#endif  // INPUT_INPUTACTIONSORTFUNCTOR_H
