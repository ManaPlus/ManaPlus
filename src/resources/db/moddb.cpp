/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "resources/db/moddb.h"

#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

namespace
{
    ModInfos mModInfos;
    bool mLoaded = false;
}  // namespace

void ModDB::load()
{
    if (mLoaded)
        unload();
    logger->log1("Initializing mod database...");
    loadXmlFile(paths.getStringValue("modsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("modsPatchFile"), SkipError_true);
    loadXmlDir("modsPatchDir", loadXmlFile);
    mLoaded = true;
}

void ModDB::loadXmlFile(const std::string &fileName,
                        const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "mods"))
    {
        logger->log("Mods Database: Error while loading %s!",
            fileName.c_str());
        return;
    }

    for_each_xml_child_node(modNode, rootNode)
    {
        if (xmlNameEqual(modNode, "include"))
        {
            const std::string name = XML::getProperty(modNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }

        if (!xmlNameEqual(modNode, "mod"))
            continue;

        const std::string name = XML::langProperty(
            // TRANSLATORS: unknown info name
            modNode, "name", _("unnamed"));
        ModInfo *currentInfo = nullptr;
        if (mModInfos.find(name) != mModInfos.end())
            currentInfo = mModInfos[name];
        if (currentInfo == nullptr)
            currentInfo = new ModInfo;

        currentInfo->setName(name);
        currentInfo->setDescription(XML::langProperty(
            modNode, "description", ""));
        currentInfo->setHelp(XML::getProperty(
            modNode, "help", ""));
        currentInfo->setLocalDir(XML::getProperty(
            modNode, "localdir", ""));

        mModInfos[name] = currentInfo;
    }
}

void ModDB::unload()
{
    logger->log1("Unloading mod database...");
    delete_all(mModInfos);
    mModInfos.clear();
    mLoaded = false;
}

ModInfo *ModDB::get(const std::string &name)
{
    const ModInfoIterator i = mModInfos.find(name);
    if (i == mModInfos.end())
        return nullptr;
    return i->second;
}

const ModInfos &ModDB::getAll()
{
    return mModInfos;
}
