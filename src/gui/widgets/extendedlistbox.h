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

#ifndef GUI_WIDGETS_EXTENDEDLISTBOX_H
#define GUI_WIDGETS_EXTENDEDLISTBOX_H

#include "gui/widgets/extendedlistboxitem.h"
#include "gui/widgets/listbox.h"

class ExtendedListBox final : public ListBox
{
    public:
        /**
         * Constructor.
         */
        ExtendedListBox(const Widget2 *const widget,
                        ListModel *const listModel,
                        const std::string &skin,
                        const unsigned int rowHeight = 13);

        A_DELETE_COPY(ExtendedListBox)

        ~ExtendedListBox();

        /**
         * Draws the list box.
         */
        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

        void adjustSize() override;

        int getSelectionByMouse(const int y) const override final;

    protected:
        int mImagePadding;
        int mSpacing;
        int mHeight;
        STD_VECTOR<ExtendedListBoxItem> mListItems;
        STD_VECTOR<ExtendedListBoxItem> mSelectedItems;
};

#endif  // GUI_WIDGETS_EXTENDEDLISTBOX_H
