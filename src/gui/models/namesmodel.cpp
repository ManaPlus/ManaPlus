/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "gui/models/namesmodel.h"

#include "utils/gettext.h"

#include "debug.h"

NamesModel::NamesModel() :
    mNames()
{
}

NamesModel::~NamesModel()
{
}

int NamesModel::getNumberOfElements()
{
    return static_cast<int>(mNames.size());
}

std::string NamesModel::getElementAt(int i)
{
    if (i >= getNumberOfElements() || i < 0)
        return "???";
    return mNames[i];
}

void NamesModel::fillFromArray(const char *const *const arr, std::size_t sz)
{
    for (size_t f = 0; f < sz; f ++)
        mNames.push_back(gettext(arr[f]));
}
