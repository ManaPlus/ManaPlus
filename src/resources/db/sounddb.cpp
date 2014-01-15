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

#include "resources/db/sounddb.h"

#include "configuration.h"
#include "notifications.h"
#include "notifymanager.h"

#include "utils/xml.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    std::string mDefault;
    std::vector<std::string> mSounds;
}

void SoundDB::load()
{
    unload();
    loadXmlFile(paths.getStringValue("soundsFile"));
    loadXmlFile(paths.getStringValue("soundsPatchFile"));
    loadXmlDir("soundsPatchDir", loadXmlFile);
}

void SoundDB::loadXmlFile(const std::string &fileName)
{
    XML::Document *doc = new XML::Document(fileName);
    const XmlNodePtrConst root = doc->rootNode();

    if (!root || !xmlNameEqual(root, "sounds"))
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        else if (xmlNameEqual(node, "sound"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            const int id = NotifyManager::getIndexBySound(name);
            if (id)
            {
                const std::string value = XML::getProperty(node, "value", "");
                mSounds[id] = value;
            }
        }
    }

    delete doc;
}

void SoundDB::unload()
{
    mSounds.resize(NotifyManager::TYPE_END);
    for (int f = 0; f < NotifyManager::TYPE_END; f ++)
        mSounds[f] = "";
}

std::string &SoundDB::getSound(const int id)
{
    if (id < 0 || id >= NotifyManager::TYPE_END)
        return mDefault;
    return mSounds[id];
}
