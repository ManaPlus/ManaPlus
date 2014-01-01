/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef GUI_WIDGETS_PROGRESSINDICATOR_H
#define GUI_WIDGETS_PROGRESSINDICATOR_H

#include "gui/widgets/widget2.h"

#include <guichan/widget.hpp>

#include "localconsts.h"

class SimpleAnimation;

/**
 * A widget that indicates progress. Suitable to use instead of a progress bar
 * in cases where it is unknown how long something is going to take.
 */
class ProgressIndicator final : public gcn::Widget,
                                public Widget2
{
    public:
        ProgressIndicator();

        A_DELETE_COPY(ProgressIndicator)

        ~ProgressIndicator();

        void logic() override final;

        void draw(gcn::Graphics *graphics) override final;

    private:
        SimpleAnimation *mIndicator;
};

#endif  // GUI_WIDGETS_PROGRESSINDICATOR_H
