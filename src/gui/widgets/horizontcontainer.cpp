/*
 *  The Mana Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The Mana Client.
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

HorizontContainer::HorizontContainer(int height, int spacing):
        mSpacing(spacing),
        mCount(0),
        mLastX(0)
{
    setHeight(height);
    addWidgetListener(this);
}

void HorizontContainer::add(gcn::Widget *widget)
{
    if (!widget)
        return;

    Container::add(widget);
    widget->setPosition(mLastX, 0);
    mCount++;
    mLastX += widget->getWidth() + mSpacing;
}

void HorizontContainer::clear()
{
    Container::clear();

    mCount = 0;
}

void HorizontContainer::widgetResized(const gcn::Event &event _UNUSED_)
{
}
