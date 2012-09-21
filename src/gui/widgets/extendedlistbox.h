/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef EXTENDEDLISTBOX_H
#define EXTENDEDLISTBOX_H

#include "gui/widgets/listbox.h"

class ExtendedListBox final : public ListBox
{
    public:
        /**
         * Constructor.
         */
        ExtendedListBox(gcn::ListModel *const listModel);

        ~ExtendedListBox();

        /**
         * Draws the list box.
         */
        void draw(gcn::Graphics *graphics);

        unsigned int getRowHeight() const
        { return mRowHeight; }

        void setRowHeight(unsigned int n)
        { mRowHeight = n; }

    protected:
        unsigned int mRowHeight;
};

#endif
