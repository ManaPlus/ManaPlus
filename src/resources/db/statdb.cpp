/*
 *  The ManaPlus Client
 *  Copyright (C) 2016  The ManaPlus Developers
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

#include "resources/db/statdb.h"

#include "configuration.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"

#include "resources/beingcommon.h"

#include "resources/item/itemfieldtype.h"

#include "utils/gettext.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    static std::vector<BasicStat> mBasicStats;
}  // namespace

void StatDb::addBasicStats()
{
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("str", _("Strength %s")));
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("agi", _("Agility %s")));
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("vit", _("Vitality %s")));
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("int", _("Intelligence %s")));
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("dex", _("Dexterity %s")));
    // TRANSLATORS: player stat
    mBasicStats.push_back(BasicStat("luck", _("Luck %s")));
}

const std::vector<BasicStat> &StatDb::getExtraStats()
{
    return mBasicStats;
}

void StatDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing stat database...");

    addBasicStats();

    loadXmlFile(paths.getStringValue("statFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("statPatchFile"), SkipError_true);
    loadXmlDir("statPatchDir", loadXmlFile);
    mLoaded = true;
}

void StatDb::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document doc(fileName,
        UseResman_true,
        skipError);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "stats"))
    {
        logger->log("StatDb: Error while loading %s!",
            fileName.c_str());
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


    }
}

void StatDb::unload()
{
    logger->log1("Unloading stat database...");

    mLoaded = false;
}
