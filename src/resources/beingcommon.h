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

#ifndef RESOURCES_BEINGCOMMON_H
#define RESOURCES_BEINGCOMMON_H

#include "utils/xml.h"

#include "utils/stringvector.h"

#include "localconsts.h"

class BeingInfo;

#define loadXmlDir(name, function) \
    StringVect listVect; \
    BeingCommon::getIncludeFiles(paths.getStringValue( \
        name), listVect); \
    FOR_EACH (StringVectCIter, itVect, listVect) \
        function(*itVect);

namespace BeingCommon
{
    void readBasicAttributes(BeingInfo *const info,
                             XmlNodePtrConst node,
                             const std::string &hoverCursor);

    void getIncludeFiles(const std::string &dir, StringVect &list);
}

#endif  // RESOURCES_BEINGCOMMON_H
