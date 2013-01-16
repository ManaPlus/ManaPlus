/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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
        ExtendedListBox(const Widget2 *const widget,
                        gcn::ListModel *const listModel,
                        int rowHeight = 13);

        A_DELETE_COPY(ExtendedListBox)

        ~ExtendedListBox();

        /**
         * Draws the list box.
         */
        void draw(gcn::Graphics *graphics) override;

        unsigned int getRowHeight() const override A_WARN_UNUSED
        { return mRowHeight; }

        void setRowHeight(unsigned int n)
        { mRowHeight = n; }

    protected:
        unsigned int mRowHeight;
        int mImagePadding;
        int mSpacing;
};

#endif
