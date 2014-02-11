/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "resources/beingcommon.h"

#include "utils/files.h"
#include "utils/stringutils.h"

#include "resources/beinginfo.h"

#include <algorithm>

#include "debug.h"

void BeingCommon::readBasicAttributes(BeingInfo *const info,
                                      XmlNodePtrConst node,
                                      const std::string &hoverCursor)
{
    info->setTargetCursorSize(XML::getProperty(node,
        "targetCursor", "medium"));

    info->setHoverCursor(XML::getProperty(node, "hoverCursor", hoverCursor));

    info->setTargetOffsetX(XML::getProperty(node, "targetOffsetX", 0));
    info->setTargetOffsetY(XML::getProperty(node, "targetOffsetY", 0));

    info->setNameOffsetX(XML::getProperty(node, "nameOffsetX", 0));
    info->setNameOffsetY(XML::getProperty(node, "nameOffsetY", 0));
    info->setSortOffsetY(XML::getProperty(node, "sortOffsetY", 0));

    info->setHpBarOffsetX(XML::getProperty(node, "hpBarOffsetX", 0));
    info->setHpBarOffsetY(XML::getProperty(node, "hpBarOffsetY", 0));
}

void BeingCommon::getIncludeFiles(const std::string &dir,
                                  StringVect &list,
                                  const std::string &ext)
{
    const std::string path = dir + "/";
    StringVect tempList;
    Files::getFilesWithDir(path, tempList);
    FOR_EACH (StringVectCIter, it, tempList)
    {
        const std::string &str = *it;
        if (findLast(str, ext))
            list.push_back(str);
    }
    std::sort(list.begin(), list.end());
}
