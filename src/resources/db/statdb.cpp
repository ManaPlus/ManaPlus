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
    mBasicStats.push_back(BasicStat(Attributes::STR,
        "str",
        // TRANSLATORS: player stat
        _("Strength")));
    mBasicStats.push_back(BasicStat(Attributes::AGI,
        "agi",
        // TRANSLATORS: player stat
        _("Agility")));
    mBasicStats.push_back(BasicStat(Attributes::VIT,
        "vit",
        // TRANSLATORS: player stat
        _("Vitality")));
    mBasicStats.push_back(BasicStat(Attributes::INT,
        "int",
        // TRANSLATORS: player stat
        _("Intelligence")));
    mBasicStats.push_back(BasicStat(Attributes::DEX,
        "dex",
        // TRANSLATORS: player stat
        _("Dexterity")));
    mBasicStats.push_back(BasicStat(Attributes::LUK,
        "luk",
        // TRANSLATORS: player stat
        _("Luck")));
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

    loadXmlFile(paths.getStringValue("statFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("statPatchFile"), SkipError_true);
    loadXmlDir("statPatchDir", loadXmlFile);
    mLoaded = true;
}

static void loadBasicStats(const XmlNodePtr rootNode)
{
    const int maxAttr = static_cast<int>(Attributes::MAX_ATTRIBUTE);
    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "stat"))
        {
            const int id = XML::getProperty(node, "id", 0);
            if (id <= 0 || id >= maxAttr)
                continue;
            const std::string tag = XML::getProperty(node, "tag", "");
            const std::string name = XML::getProperty(node, "name", "");
            mBasicStats.push_back(BasicStat(static_cast<AttributesT>(id),
                tag,
                name));
        }
    }
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
        if (skipError == SkipError_false)
            addBasicStats();
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
        else if (xmlNameEqual(node, "basic"))
        {
            loadBasicStats(node);
        }
    }
}

void StatDb::unload()
{
    logger->log1("Unloading stat database...");

    mLoaded = false;
}
