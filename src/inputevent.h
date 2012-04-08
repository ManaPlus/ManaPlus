/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <map>
#include <vector>

typedef std::vector<int> KeysVector;
typedef KeysVector::iterator KeysVectorIter;
typedef KeysVector::const_iterator KeysVectorCIter;

typedef std::map<int, KeysVector> KeyToActionMap;
typedef KeyToActionMap::iterator KeyToActionMapIter;

struct InputEvent
{
    InputEvent(int action0, int mask0);

    int action;

    int mask;
};

#endif
