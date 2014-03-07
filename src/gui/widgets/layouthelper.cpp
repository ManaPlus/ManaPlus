/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/widgets/layouthelper.h"

#include "gui/widgets/basiccontainer2.h"

#include "debug.h"

LayoutHelper::LayoutHelper(BasicContainer2 *const container) :
    WidgetListener(),
    mLayout(),
    mContainer(container)
{
    mContainer->addWidgetListener(this);
}

LayoutHelper::~LayoutHelper()
{
    mContainer->removeWidgetListener(this);
}

const Layout &LayoutHelper::getLayout() const
{
    return mLayout;
}

LayoutCell &LayoutHelper::place(const int x, const int y,
                                Widget *const wg,
                                const int w, const int h)
{
    mContainer->add(wg);
    return mLayout.place(wg, x, y, w, h);
}

ContainerPlacer LayoutHelper::getPlacer(const int x, const int y)
{
    return ContainerPlacer(mContainer, &mLayout.at(x, y));
}

void LayoutHelper::reflowLayout(int w, int h)
{
    mLayout.reflow(w, h);
    mContainer->setSize(w, h);
}

void LayoutHelper::widgetResized(const Event &event A_UNUSED)
{
    const Rect area = mContainer->getChildrenArea();
    int w = area.width;
    int h = area.height;
    mLayout.reflow(w, h);
}
