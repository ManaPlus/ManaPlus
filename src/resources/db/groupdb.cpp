/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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

#include "being/localplayer.h"

#include "utils/checkutils.h"

#ifdef TMWA_SUPPORT
#include "net/net.h"
#endif  // TMWA_SUPPORT

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

    logger->log1("Initializing group database...");

    loadXmlFile(paths.getStringValue("groupsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("groupsPatchFile"), SkipError_true);
    loadXmlDir("groupsPatchDir", loadXmlFile)
    mLoaded = true;
}

#define servercommandFirst(name) \
    if (str == #name) \
        return ServerCommandType::name; \
    else
#define servercommand(name) \
    if (str == #name) \
        return ServerCommandType::name; \
    else
#define servercommand2(name1, name2) \
    if (str == #name2) \
        return ServerCommandType::name1; \
    else

static ServerCommandTypeT parseCommand(const std::string &str,
                                       const int id)
{
#include "resources/servercommands.inc"
    {
        reportAlways("GroupsDb: unknown command name: '%s' in group id '%d'.",
            str.c_str(),
            id)
    }

    return ServerCommandType::Max;
}

SERVERCOMMANDS_VOID
#undef servercommandFirst
#undef servercommand
#undef servercommand2

#define serverpermissionFirst(name) \
    if (str == #name) \
        return ServerPermissionType::name; \
    else
#define serverpermission(name) \
    if (str == #name) \
        return ServerPermissionType::name; \
    else

static ServerPermissionTypeT parsePermission(const std::string &str,
                                             const int id)
{
#include "resources/serverpermissions.inc"
    {
        reportAlways("GroupsDb: unknown permission name: "
            "'%s' in group id '%d'.",
            str.c_str(),
            id)
    }

    return ServerPermissionType::Max;
}

SERVERPERMISSION_VOID
#undef serverpermissionFirst
#undef serverpermission

static ServerCommandEnable::Type parseUseFlag(const std::string &str,
                                              const int id)
{
    if (str == "self")
    {
        return ServerCommandEnable::Self;
    }
    else if (str == "other")
    {
        return ServerCommandEnable::Other;
    }
    else if (str == "both")
    {
        return ServerCommandEnable::Both;
    }
    else if (str == "false")
    {
        return ServerCommandEnable::No;
    }
    else
    {
        reportAlways("GroupsDb: unknown use flag: '%s' in group id '%d'."
            "Possible values 'self', 'other', 'both'.",
            str.c_str(),
            id)
        return ServerCommandEnable::No;
    }
}

static void loadCommands(XmlNodePtr rootNode,
                         const int id,
                         GroupInfo *groupInfo) A_NONNULL(3);
static void loadCommands(XmlNodePtr rootNode,
                         const int id,
                         GroupInfo *groupInfo)
{
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "command"))
        {
            const std::string nameStr = XML::getProperty(node,
                "name",
                "");
            const std::string useStr = XML::getProperty(node,
                "use",
                "");
            ServerCommandTypeT name = parseCommand(nameStr, id);
            if (name == ServerCommandType::Max)
                continue;
            ServerCommandEnable::Type useFlag = parseUseFlag(useStr, id);
            if (useFlag == ServerCommandEnable::No)
                continue;
            groupInfo->mCommands[CAST_SIZE(name)] = useFlag;
        }
    }
}

static void loadPermissions(XmlNodePtr rootNode,
                            const int id,
                            GroupInfo *groupInfo) A_NONNULL(3);
static void loadPermissions(XmlNodePtr rootNode,
                            const int id,
                            GroupInfo *groupInfo)
{
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "permission"))
        {
            const std::string nameStr = XML::getProperty(node,
                "name",
                "");
            ServerPermissionTypeT perm = parsePermission(nameStr, id);
            if (perm == ServerPermissionType::Max)
                continue;
            if (!XML::getBoolProperty(node,
                "enable",
                true))
            {
                continue;
            }
            groupInfo->mPermissions[CAST_SIZE(perm)] = Enable_true;
        }
    }
}

static void loadSubNodes(XmlNodePtr groupNode,
                         const int id,
                         GroupInfo *groupInfo) A_NONNULL(3);
static void loadSubNodes(XmlNodePtr groupNode,
                         const int id,
                         GroupInfo *groupInfo)
{
    for_each_xml_child_node(node, groupNode)
    {
        if (xmlNameEqual(node, "commands"))
            loadCommands(node, id, groupInfo);
        if (xmlNameEqual(node, "permissions"))
            loadPermissions(node, id, groupInfo);
    }
}

static void parseInherit(XmlNodePtr groupNode,
                         const int id,
                         GroupInfo *groupInfo) A_NONNULL(3);
static void parseInherit(XmlNodePtr groupNode,
                         const int id,
                         GroupInfo *groupInfo)
{
    const std::string inherit = XML::langProperty(groupNode,
        "inherit",
        "");
    STD_VECTOR<int> parts;
    splitToIntVector(parts, inherit, ',');
    FOR_EACH (STD_VECTOR<int>::const_iterator, it, parts)
    {
        const int id2 = *it;
        GroupDb::GroupInfos::const_iterator it2 = mGroups.find(id2);
        if (it2 == mGroups.end())
        {
            reportAlways("Unknown inherit group id '%d' in group '%d'",
                id2,
                id)
            continue;
        }
        GroupInfo *const groupInfo2 = (*it2).second;
        for (size_t f = 0; f < CAST_SIZE(ServerCommandType::Max); f ++)
        {
            ServerCommandEnable::Type enable = groupInfo2->mCommands[f];
            if (enable != ServerCommandEnable::No)
                groupInfo->mCommands[f] = enable;
        }
        for (size_t f = 0; f < CAST_SIZE(ServerPermissionType::Max); f ++)
        {
            if (groupInfo2->mPermissions[f] == Enable_true)
                groupInfo->mPermissions[f] = Enable_true;
        }
    }
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
                fileName.c_str())
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
                reportAlways("Empty id field in GroupDb")
                continue;
            }
            GroupInfosIter it = mGroups.find(id);
            GroupInfo *group = nullptr;
            if (it != mGroups.end())
            {
                reportAlways("GroupDb: group with id %d already added",
                    id)
                group = (*it).second;
            }
            else
            {
                group = new GroupInfo;
                mGroups[id] = group;
            }
            group->name = XML::langProperty(node,
                "name",
                "");
            group->longName = XML::langProperty(node,
                "longName",
                "");
            group->badge = XML::langProperty(node,
                "badge",
                paths.getStringValue("gmbadge"));
            group->showBadge = XML::getBoolProperty(node,
                "showBadge",
                false);
            group->highlightName = XML::getBoolProperty(node,
                "highlightName",
                false);
            loadSubNodes(node, id, group);
            parseInherit(node, id, group);
        }
    }
}

void GroupDb::unload()
{
    logger->log1("Unloading group database...");
    FOR_EACH (GroupInfosIter, it, mGroups)
    {
        delete (*it).second;
    }
    mGroups.clear();
    mLoaded = false;
}

const std::string &GroupDb::getName(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return mEmptyGroup.name;
    }
    return (*it).second->name;
}

const std::string &GroupDb::getLongName(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return mEmptyGroup.longName;
    }
    return (*it).second->longName;
}

bool GroupDb::getShowBadge(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return mEmptyGroup.showBadge;
    }
    return (*it).second->showBadge;
}

bool GroupDb::getHighlightName(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return mEmptyGroup.highlightName;
    }
    return (*it).second->highlightName;
}

const std::string &GroupDb::getBadge(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return mEmptyGroup.badge;
    }
    return (*it).second->badge;
}

const GroupInfo *GroupDb::getGroup(const int id)
{
    GroupInfos::const_iterator it = mGroups.find(id);
    if (it == mGroups.end())
    {
        reportAlways("Unknown group id requested: %d", id)
        return &mEmptyGroup;
    }
    return (*it).second;
}

bool GroupDb::isAllowCommand(const ServerCommandTypeT command)
{
    if (localPlayer == nullptr)
        return false;
    const int groupId = localPlayer->getGroupId();
    const GroupInfo *const group = GroupDb::getGroup(groupId);

#ifdef TMWA_SUPPORT
    // allow any commands for legacy if group > 0
    if (Net::getNetworkType() == ServerType::TMWATHENA &&
        localPlayer->isGM())
    {
        return true;
    }
#endif
    if (group->mPermissions[CAST_SIZE(ServerPermissionType::all_commands)] ==
        Enable_true)
    {
        return true;
    }
    const ServerCommandEnable::Type enabled =
        group->mCommands[CAST_SIZE(command)];
    return (enabled & ServerCommandEnable::Self) != 0;
}

#ifdef UNITTESTS
GroupDb::GroupInfos &GroupDb::getGroups()
{
    return mGroups;
}
#endif  // UNITTESTS
