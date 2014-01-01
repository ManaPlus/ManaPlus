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

#include "gui/widgets/extendednamesmodel.h"

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
    return static_cast<int>(mNames.size());
}

std::string ExtendedNamesModel::getElementAt(int i)
{
    if (i >= getNumberOfElements() || i < 0)
        return "???";
    return mNames[i];
}

const Image *ExtendedNamesModel::getImageAt(int i)
{
    if (i >= static_cast<int>(mImages.size()) || i < 0)
        return nullptr;

    return mImages[i];
}

void ExtendedNamesModel::clear()
{
    mNames.clear();
    FOR_EACH (std::vector<Image*>::iterator, it, mImages)
    {
        if (*it)
            (*it)->decRef();
    }
    mImages.clear();
}
