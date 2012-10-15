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

#ifndef GUI_HORIZONTCONTAINER_H
#define GUI_HORIZONTCONTAINER_H

#include "gui/widgets/container.h"

#include <guichan/widgetlistener.hpp>

#include "localconsts.h"

/**
 * A widget container.
 *
 * This container places it's contents veritcally.
 */
class HorizontContainer final : public Container, public gcn::WidgetListener
{
    public:
        HorizontContainer(const int height, const int spacing);

        A_DELETE_COPY(HorizontContainer)

        virtual void add(gcn::Widget *widget);

        virtual void add(gcn::Widget *widget, int spacing);

        virtual void clear();

        void widgetResized(const gcn::Event &event) override;

    protected:
        int mSpacing;
        int mCount;
        int mLastX;
};

#endif
