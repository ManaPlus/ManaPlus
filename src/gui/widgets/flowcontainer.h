/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_FLOWCONTAINER_H
#define GUI_WIDGETS_FLOWCONTAINER_H

#include "gui/widgets/container.h"

#include <guichan/widgetlistener.hpp>

#include "localconsts.h"

/**
 * A container that arranges its contents like words on a page.
 *
 * \ingroup GUI
 */
class FlowContainer final : public Container,
                            public gcn::WidgetListener
{
    public:
        /**
         * Constructor. Initializes the shortcut container.
         */
        FlowContainer(const Widget2 *const widget,
                      const int boxWidth, const int boxHeight);

        A_DELETE_COPY(FlowContainer)

        /**
         * Destructor.
         */
        ~FlowContainer()
        { }

        /**
         * Invoked when a widget changes its size. This is used to determine
         * the new height of the container.
         */
        void widgetResized(const gcn::Event &event) override final;

        int getBoxWidth() const A_WARN_UNUSED
        { return mBoxWidth; }

        int getBoxHeight() const A_WARN_UNUSED
        { return mBoxHeight; }

        void add(gcn::Widget *widget) override final;

    private:
        int mBoxWidth;
        int mBoxHeight;
        int mGridWidth, mGridHeight;
};

#endif  // GUI_WIDGETS_FLOWCONTAINER_H
