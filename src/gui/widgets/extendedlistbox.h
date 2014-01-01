/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_EXTENDEDLISTBOX_H
#define GUI_WIDGETS_EXTENDEDLISTBOX_H

#include "gui/widgets/listbox.h"

struct ExtendedListBoxItem
{
    ExtendedListBoxItem(const int row0,
                        const std::string &text0,
                        const bool image0,
                        const int y0) :
        row(row0),
        text(text0),
        image(image0),
        y(y0)
    {
    }
    int row;
    std::string text;
    bool image;
    int y;
};

class ExtendedListBox final : public ListBox
{
    public:
        /**
         * Constructor.
         */
        ExtendedListBox(const Widget2 *const widget,
                        gcn::ListModel *const listModel,
                        const std::string &skin,
                        const int rowHeight = 13);

        A_DELETE_COPY(ExtendedListBox)

        ~ExtendedListBox();

        /**
         * Draws the list box.
         */
        void draw(gcn::Graphics *graphics) override final;

        void adjustSize() override;

        int getSelectionByMouse(const int y) const override final;

    protected:
        int mImagePadding;
        int mSpacing;
        int mHeight;
        std::vector<ExtendedListBoxItem> mListItems;
        std::vector<ExtendedListBoxItem> mSelectedItems;
};

#endif  // GUI_WIDGETS_EXTENDEDLISTBOX_H
