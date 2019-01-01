/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef LISTENERS_TABLEMODELLISTENER_H
#define LISTENERS_TABLEMODELLISTENER_H

#include "localconsts.h"

class Widget;

class TableModelListener notfinal
{
    public:
        TableModelListener()
        { }

        A_DELETE_COPY(TableModelListener)

        /**
         * Must be invoked by the TableModel whenever a global change is about
         * to occur or has occurred (e.g., when a row or column is being
         * removed or added).
         *
         * This method is triggered twice, once before and once after
         * the update.
         *
         * \param completed whether we are signalling the end of the update
         */
        virtual void modelUpdated(const bool completed) = 0;

        virtual ~TableModelListener()
        { }
};

#endif  // LISTENERS_TABLEMODELLISTENER_H
