/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef GUI_VERTCONTAINER_H
#define GUI_VERTCONTAINER_H

#include "gui/widgets/container.h"

#include <guichan/widgetlistener.hpp>

#include <vector>

/**
 * A widget container.
 *
 * This container places it's contents veritcally.
 */
class VertContainer : public Container, public gcn::WidgetListener
{
    public:
        VertContainer(int verticalItemSize, bool resizable = true,
                      int leftSpacing = 0);

        virtual void add(gcn::Widget *widget, bool resizable,
                         int spacing = -1);

        virtual void add(gcn::Widget *widget, int spacing = -1);

        virtual void clear();

        void widgetResized(const gcn::Event &event);

    private:
        std::vector<gcn::Widget*> mResizableWidgets;
        int mVerticalItemSize;
        int mCount;
        int mNextY;
        int mLeftSpacing;
        int mVerticalSpacing;
        bool mResizable;
};

#endif
