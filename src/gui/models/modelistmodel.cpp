/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "render/graphics.h"

#include "utils/sdlhelper.h"
#include "utils/stringutils.h"

#include "gui/models/modelistmodel.h"

#include <algorithm>

#include "debug.h"

#ifndef ANDROID
static bool modeSorter(const std::string &mode1, const std::string &mode2)
{
    const int width1 = atoi(mode1.substr(0, mode1.find('x')).c_str());
    const int height1 = atoi(mode1.substr(mode1.find('x') + 1).c_str());
    if (!width1 || !height1)
        return false;

    const int width2 = atoi(mode2.substr(0, mode2.find('x')).c_str());
    const int height2 = atoi(mode2.substr(mode2.find('x') + 1).c_str());
    if (!width2 || !height2)
        return false;
    if (width1 != width2)
        return width1 < width2;

    if (height1 != height2)
        return height1 < height2;

    return false;
}
#endif  // ANDROID

ModeListModel::ModeListModel() :
    mVideoModes()
{
    SDL::getAllVideoModes(mVideoModes);
#ifndef ANDROID
    addCustomMode("640x480");
    addCustomMode("800x600");
    addCustomMode("1024x768");
    addCustomMode("1280x1024");
    addCustomMode("1400x900");
    addCustomMode("1500x990");
    addCustomMode(toString(mainGraphics->mActualWidth).append("x")
        .append(toString(mainGraphics->mActualHeight)));

    std::sort(mVideoModes.begin(), mVideoModes.end(), &modeSorter);
    mVideoModes.push_back("custom");
#endif  // ANDROID
}

#ifndef ANDROID
void ModeListModel::addCustomMode(const std::string &mode)
{
    StringVectCIter it = mVideoModes.begin();
    const StringVectCIter it_end = mVideoModes.end();
    while (it != it_end)
    {
        if (*it == mode)
            return;
        ++ it;
    }
    mVideoModes.push_back(mode);
}
#endif  // ANDROID

int ModeListModel::getIndexOf(const std::string &widthXHeightMode)
{
    std::string currentMode("");
    for (int i = 0; i < getNumberOfElements(); i++)
    {
        currentMode = getElementAt(i);
        if (currentMode == widthXHeightMode)
            return i;
    }
    return -1;
}
