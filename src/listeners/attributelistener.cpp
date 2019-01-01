/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#include "listeners/attributelistener.h"

#include "utils/foreach.h"

#include "debug.h"

defineListener(AttributeListener)

void AttributeListener::distributeEvent(const AttributesT id,
                                        const int64_t oldVal,
                                        const int64_t newVal)
{
    FOR_EACH (STD_VECTOR<AttributeListener*>::iterator,
              it, mListeners)
    {
        AttributeListener *const listener = *it;
        if (listener != nullptr)
            listener->attributeChanged(id, oldVal, newVal);
    }
}
