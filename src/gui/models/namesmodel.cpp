/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "utils/cast.h"
#include "utils/foreach.h"
#include "utils/gettext.h"

#include "debug.h"

NamesModel::NamesModel() :
    ListModel(),
    mNames()
{
}

NamesModel::~NamesModel()
{
}

int NamesModel::getNumberOfElements()
{
    return CAST_S32(mNames.size());
}

std::string NamesModel::getElementAt(int i)
{
    if (i >= getNumberOfElements() || i < 0)
        return "???";
    return mNames[CAST_SIZE(i)];
}

void NamesModel::fillFromArray(const char *const *const arr,
                               const std::size_t sz)
{
    if (arr == nullptr)
        return;
    for (size_t f = 0; f < sz; f ++)
        mNames.push_back(gettext(arr[f]));
}

void NamesModel::fillFromVector(const StringVect &vect)
{
    FOR_EACH(StringVectCIter, it, vect)
    {
        const std::string str = *it;
        if (!str.empty())
            mNames.push_back(str);
    }
}
