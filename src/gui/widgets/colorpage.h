/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_COLORPAGE_H
#define GUI_WIDGETS_COLORPAGE_H

#include "gui/widgets/listbox.h"

#include "localconsts.h"

class ColorPage final : public ListBox
{
    public:
        ColorPage(const Widget2 *const widget,
                  gcn::ListModel *const listModel,
                  const std::string &skin);

        A_DELETE_COPY(ColorPage)

        ~ColorPage();

        void draw(gcn::Graphics *graphics) override final;

        void resetAction();

        void adjustSize() override final;

    private:
        int mItemPadding;
        unsigned int mRowHeight;
};

#endif  // GUI_WIDGETS_COLORPAGE_H
