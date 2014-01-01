/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <list>

#include "localconsts.h"

/**
 * A position along a being's path.
 */
struct Position final
{
    Position(const int x0, const int y0) :
        x(x0), y(y0)
    { }

    int x;
    int y;
};

typedef std::list<Position> Path;
typedef Path::iterator PathIterator;

/**
 * Appends a string representation of a position to the output stream.
 */
std::ostream& operator <<(std::ostream &os, const Position &p);

/**
 * Appends a string representation of a path (sequence of positions) to the
 * output stream.
 */
std::ostream& operator <<(std::ostream &os, const Path &path);

#endif  // POSITION_H
