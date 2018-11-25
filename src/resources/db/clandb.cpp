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

#include "resources/db/clandb.h"

#include "configuration.h"

#include "resources/beingcommon.h"
#include "resources/claninfo.h"

#include "resources/db/itemfielddb.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/itemxmlutils.h"

#include "debug.h"

namespace
{
    std::map<int, ClanInfo *> mClansInfos;
    bool mLoaded = false;
}  // namespace

void ClanDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing clans database...");
    loadXmlFile(paths.getStringValue("clansFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("clansPatchFile"), SkipError_true);
    loadXmlDir("clansPatchDir", loadXmlFile)

    mLoaded = true;
}

void ClanDb::loadXmlFile(const std::string &fileName,
                         const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtr rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "clans"))
    {
        logger->log("Clans database: Error while loading %s!",
            paths.getStringValue("clansFile").c_str());
        mLoaded = true;
        return;
    }

    const ItemFieldInfos &addFields =
        ItemFieldDb::getAddFields();

    // iterate <clan>s
    for_each_xml_child_node(clanNode, rootNode)
    {
        if (xmlNameEqual(clanNode, "include"))
        {
            const std::string name = XML::getProperty(
                clanNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        if (!xmlNameEqual(clanNode, "clan"))
            continue;

        const int id = XML::getProperty(clanNode, "id", 0);
        ClanInfo *clanInfo = nullptr;
        if (mClansInfos.find(id) != mClansInfos.end())
        {
            reportAlways("ClanDb: Redefinition of clan ID %d", id)
            clanInfo = mClansInfos[id];
        }
        if (clanInfo == nullptr)
            clanInfo = new ClanInfo;

        clanInfo->id = id;
        clanInfo->name = XML::langProperty(
            // TRANSLATORS: unknown clan name
            clanNode, "name", _("unnamed"));

        readItemStatsVector(clanInfo->stats,
            clanNode,
            addFields);

        mClansInfos[id] = clanInfo;
    }
}

void ClanDb::unload()
{
    logger->log1("Unloading clans database...");
    delete_all(mClansInfos);
    mClansInfos.clear();

    mLoaded = false;
}

const ClanInfo *ClanDb::get(const int clanId)
{
    std::map<int, ClanInfo *>::const_iterator i =
        mClansInfos.find(clanId);

    if (i == mClansInfos.end())
    {
        i = mClansInfos.find(clanId);
        if (i == mClansInfos.end())
        {
            reportAlways("ClanDb: Warning, unknown clan ID "
                "%d requested",
                clanId)
            return nullptr;
        }
    }
    return i->second;
}
