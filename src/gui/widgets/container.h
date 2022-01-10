/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUI_WIDGETS_CONTAINER_H
#define GUI_WIDGETS_CONTAINER_H

#include "gui/widgets/basiccontainer2.h"

/**
 * A widget container.
 *
 * The main difference between the standard Guichan container and this one is
 * that childs added to this container are automatically deleted when the
 * container is deleted.
 *
 * This container is also non-opaque by default.
 */
class Container notfinal : public BasicContainer2
{
    public:
        explicit Container(const Widget2 *const widget);

        A_DELETE_COPY(Container)

        ~Container() override;

        bool safeRemove(Widget *const widget);

        void removeControls();
};

#endif  // GUI_WIDGETS_CONTAINER_H
