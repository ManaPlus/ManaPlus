/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "gui/models/extendednamesmodel.h"

#include "utils/cast.h"
#include "utils/foreach.h"

#include "debug.h"

ExtendedNamesModel::ExtendedNamesModel() :
    mNames(),
    mImages()
{
}

ExtendedNamesModel::~ExtendedNamesModel()
{
    clear();
}

int ExtendedNamesModel::getNumberOfElements()
{
    return CAST_S32(mNames.size());
}

std::string ExtendedNamesModel::getElementAt(int i)
{
    if (i >= getNumberOfElements() || i < 0)
        return "???";
    return mNames[CAST_SIZE(i)];
}

const Image *ExtendedNamesModel::getImageAt(int i)
{
    if (i >= CAST_S32(mImages.size()) || i < 0)
        return nullptr;

    return mImages[CAST_SIZE(i)];
}

void ExtendedNamesModel::clear()
{
    mNames.clear();
    FOR_EACH (STD_VECTOR<Image*>::iterator, it, mImages)
    {
        if (*it != nullptr)
            (*it)->decRef();
    }
    mImages.clear();
}
