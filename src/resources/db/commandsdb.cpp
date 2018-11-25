/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "resources/db/commandsdb.h"

#include "configuration.h"
#include "logger.h"
#include "textcommand.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    CommandsMap mCommands;
    bool mLoaded = false;
}  // namespace

void CommandsDB::load()
{
    if (mLoaded)
        unload();
    logger->log1("Initializing commands database...");
    loadXmlFile(paths.getStringValue("defaultCommandsFile"),
        SkipError_false);
    loadXmlFile(paths.getStringValue("defaultCommandsPatchFile"),
        SkipError_true);
    loadXmlDir("defaultCommandsPatchDir", loadXmlFile)
    mLoaded = true;
}

static CommandTargetT parseTarget(const std::string &text)
{
    if (text == "allow target")
        return CommandTarget::AllowTarget;
    else if (text == "need target")
        return CommandTarget::NeedTarget;
    else
        return CommandTarget::NoTarget;
}

void CommandsDB::loadXmlFile(const std::string &fileName,
                             const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "commands"))
    {
        logger->log("Commands Database: Error while loading %s!",
            fileName.c_str());
        return;
    }

    for_each_xml_child_node(commandNode, rootNode)
    {
        if (xmlNameEqual(commandNode, "include"))
        {
            const std::string name = XML::getProperty(commandNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }

        if (!xmlNameEqual(commandNode, "command"))
            continue;

        const int id = XML::getProperty(commandNode, "id", -1);
        if (id == -1)
            continue;

        const CommandsMapIter it = mCommands.find(id);
        if (it != mCommands.end())
        {
            logger->log("Commands database: duplicate id: %d", id);
            TextCommand *tempCmd = (*it).second;
            mCommands.erase(it);
            delete tempCmd;
        }
        const std::string name = XML::langProperty(
            commandNode, "name", "?");
        const std::string command = XML::getProperty(
            commandNode, "command", "");
        const std::string comment = XML::getProperty(
            commandNode, "comment", "");
        const CommandTargetT targetType = parseTarget(XML::getProperty(
            commandNode, "target", ""));
        const std::string icon = XML::getProperty(
            commandNode, "icon", "");
#ifdef TMWA_SUPPORT
        const int skill1 = XML::getIntProperty(
            commandNode, "skill1", 0, 0, 1000000);
        const int level1 = XML::getIntProperty(
            commandNode, "level1", 0, 0, 1000);
        const int skill2 = XML::getIntProperty(
            commandNode, "skill2", 0, 0, 1000000);
        const int level2 = XML::getIntProperty(
            commandNode, "level2", 0, 0, 1000);
        const int mana = XML::getIntProperty(
            commandNode, "mana", 0, 0, 100000);
#endif  // TMWA_SUPPORT

        TextCommand *cmd = nullptr;
#ifdef TMWA_SUPPORT
        if (skill1 != 0)
        {
            cmd = new TextCommand(id,
                name,
                command,
                comment,
                targetType,
                icon,
                level1,
                static_cast<MagicSchoolT>(skill2),
                level2,
                mana);
        }
        else
#endif  // TMWA_SUPPORT
        {
            cmd = new TextCommand(id,
                name,
                command,
                comment,
                targetType,
                icon);
        }
        mCommands[id] = cmd;
    }
}

void CommandsDB::unload()
{
    logger->log1("Unloading commands database...");
    mCommands.clear();
    mLoaded = false;
}

std::map<int, TextCommand*> &CommandsDB::getAll()
{
    return mCommands;
}
