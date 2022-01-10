/*
 *  The ManaPlus Client
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

#ifndef GUI_WIDGETS_VERTCONTAINER_H
#define GUI_WIDGETS_VERTCONTAINER_H

#include "gui/widgets/container.h"

#include "listeners/widgetlistener.h"

#include "localconsts.h"

/**
 * A widget container.
 *
 * This container places it's contents veritcally.
 */
class VertContainer final : public Container,
                            public WidgetListener
{
    public:
        VertContainer(const Widget2 *const widget,
                      const int verticalItemSize,
                      const bool resizable,
                      const int leftSpacing);

        A_DELETE_COPY(VertContainer)

        void add2(Widget *const widget,
                  const bool resizable,
                  const int spacing);

        void add1(Widget *const widget,
                  const int spacing);

        void clear() override final;

        void widgetResized(const Event &event) override final;

    private:
        STD_VECTOR<Widget*> mResizableWidgets;
        int mVerticalItemSize;
        int mCount;
        int mNextY;
        int mLeftSpacing;
        int mVerticalSpacing;
        bool mResizable;
};

#endif  // GUI_WIDGETS_VERTCONTAINER_H
