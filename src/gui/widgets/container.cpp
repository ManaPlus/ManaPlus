/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/widgets/container.h"

#include "gui/gui.h"

#include "debug.h"

Container::Container(const Widget2 *const widget) :
    BasicContainer2(widget)
{
    setOpaque(false);
}

Container::~Container()
{
    if (gui)
        gui->removeDragged(this);
    removeControls();
}

void Container::removeControls()
{
    while (!mWidgets.empty())
        delete mWidgets.front();
}

bool Container::safeRemove(Widget *const widget)
{
    for (WidgetListConstIterator iter = mWidgets.begin();
         iter != mWidgets.end(); ++iter)
    {
        if (*iter == widget)
        {
            remove(widget);
            return true;
        }
    }
    return false;
}
