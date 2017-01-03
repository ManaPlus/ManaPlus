/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "utils/files.h"
#include "utils/xml.h"

#include "localconsts.h"

class BeingInfo;

struct SpriteDisplay;

#define loadXmlDir(name, function) \
    { \
        StringVect listVect; \
        Files::getFilesInDir(paths.getStringValue( \
            name), listVect, ".xml"); \
        FOR_EACH (StringVectCIter, itVect, listVect) \
            function(*itVect, SkipError_true); \
    }

#define loadXmlDir2(name, function, ext, skipError) \
    { \
        StringVect listVect; \
        Files::getFilesInDir(name, listVect, ext); \
        FOR_EACH (StringVectCIter, itVect, listVect) \
            function(*itVect, skipError); \
    }

namespace BeingCommon
{
    void readBasicAttributes(BeingInfo *const info,
                             XmlNodePtrConst node,
                             const std::string &hoverCursor) A_NONNULL(1);

    void readWalkingAttributes(BeingInfo *const info,
                               XmlNodePtrConst node,
                               const int moreBlockFlags) A_NONNULL(1);

    void readAiAttributes(BeingInfo *const info,
                          XmlNodePtrConst node);

    void getIncludeFiles(const std::string &dir,
                         StringVect &list,
                         const std::string &ext);

    bool readObjectNodes(XmlNodePtrConst &spriteNode,
                         SpriteDisplay &display,
                         BeingInfo *const currentInfo,
                         const std::string &dbName) A_NONNULL(3);
}  // namespace BeingCommon

#endif  // RESOURCES_BEINGCOMMON_H
