/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#include "resources/db/groupdb.h"

#include "configuration.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"
#include "resources/groupinfo.h"

#include "debug.h"

namespace
{
    GroupDb::GroupInfos mGroups;
    const GroupInfo mEmptyGroup;
    bool mLoaded = false;
}  // namespace

void GroupDb::load()
{
    if (mLoaded)
        unload();

    loadXmlFile(paths.getStringValue("groupsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("groupsPatchFile"), SkipError_true);
    loadXmlDir("groupsPatchDir", loadXmlFile);
    mLoaded = true;
}

void GroupDb::loadXmlFile(const std::string &fileName,
                          const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if (rootNode == nullptr ||
        !xmlNameEqual(rootNode, "groups"))
    {
        if (skipError == SkipError_true)
        {
            logger->log("GroupsDb: Error while loading %s!",
                fileName.c_str());
        }
        else
        {
            reportAlways("GroupsDb: Error while loading %s!",
                fileName.c_str());
        }
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        if (xmlNameEqual(node, "group"))
        {
            const int id = XML::getProperty(node,
                "id",
                -1);
            if (id < 0)
            {
                reportAlways("Empty id field in GroupDb");
                continue;
            }
            mGroups[id].name = XML::langProperty(node,
                "name",
                "");
            mGroups[id].longName = XML::langProperty(node,
                "longName",
                "");
        }
    }
}

void GroupDb::unload()
{
    mGroups.clear();
    mLoaded = false;
}

const std::string &GroupDb::getName(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id);
        return mEmptyGroup.name;
    }
    return (*it).second.name;
}

const std::string &GroupDb::getLongName(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id);
        return mEmptyGroup.longName;
    }
    return (*it).second.longName;
}

#ifdef UNITTESTS
GroupDb::GroupInfos &GroupDb::getGroups()
{
    return mGroups;
}
#endif  // UNITTESTS
