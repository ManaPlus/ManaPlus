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

#include "gui/widgets/horizontcontainer.h"

#include "debug.h"

HorizontContainer::HorizontContainer(const Widget2 *const widget,
                                     const int height,
                                     const int spacing) :
    Container(widget),
    WidgetListener(),
    ToolTipListener(),
    mSpacing(spacing),
    mCount(0),
    mLastX(spacing)
{
    setHeight(height);
    addWidgetListener(this);
    addMouseListener(this);
}

void HorizontContainer::add(Widget *const widget)
{
    add(widget, mSpacing);
}

void HorizontContainer::add(Widget *const widget, const int spacing)
{
    if (widget == nullptr)
        return;

    Container::add(widget);
    widget->setPosition(mLastX, spacing);
    mCount++;
    mLastX += widget->getWidth() + 2 * mSpacing;
}

void HorizontContainer::clear()
{
    Container::clear();

    mCount = 0;
}

void HorizontContainer::widgetResized(const Event &event A_UNUSED)
{
}
