/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "gui/widgets/containerplacer.h"

#include "gui/widgets/basiccontainer2.h"
#include "gui/widgets/layoutcell.h"

#include "debug.h"

ContainerPlacer ContainerPlacer::at(const int x, const int y)
{
    return ContainerPlacer(mContainer, mCell ? &mCell->at(x, y) : nullptr);
}

LayoutCell &ContainerPlacer::operator()
    (const int x, const int y, Widget *const wg, const int w, const int h)
{
    if (mContainer)
        mContainer->add(wg);
    if (mCell)
        return mCell->place(wg, x, y, w, h);
    return LayoutCell::emptyCell;
}
