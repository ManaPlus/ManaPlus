/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_LAYOUTHELPER_H
#define GUI_WIDGETS_LAYOUTHELPER_H

#include "gui/widgets/layout.h"

#include "listeners/widgetlistener.h"

class BasicContainer2;
class ContainerPlacer;

/**
 * A helper class for adding a layout to a Guichan container widget. The layout
 * will register itself as a widget listener and relayout the widgets in the
 * container dynamically on resize.
 */
class LayoutHelper final : public WidgetListener
{
    public:
        /**
         * Constructor.
         */
        explicit LayoutHelper(BasicContainer2 *const container);

        A_DELETE_COPY(LayoutHelper)

        /**
         * Destructor.
         */
        ~LayoutHelper() override final;

        /**
         * Gets the layout handler.
         */
        const Layout &getLayout() const A_WARN_UNUSED A_CONST;

        /**
         * Computes the position of the widgets according to the current
         * layout. Resizes the managed container so that the layout fits.
         *
         * @note This function is meant to be called with fixed-size
         * containers.
         *
         * @param w if non-zero, force the container to this width.
         * @param h if non-zero, force the container to this height.
         */
        void reflowLayout(int w,
                          int h);

        /**
         * Adds a widget to the container and sets it at given cell.
         */
        LayoutCell &place(const int x,
                          const int y,
                          Widget *const wg,
                          const int w,
                          const int h);

        /**
         * Returns a proxy for adding widgets in an inner table of the layout.
         */
        ContainerPlacer getPlacer(const int x, const int y) A_WARN_UNUSED;

        /**
         * Called whenever the managed container changes size.
         */
        void widgetResized(const Event &event) override final;

    private:
        Layout mLayout;              /**< Layout handler */
        BasicContainer2 *mContainer;  /**< Managed container */
};

#endif  // GUI_WIDGETS_LAYOUTHELPER_H
