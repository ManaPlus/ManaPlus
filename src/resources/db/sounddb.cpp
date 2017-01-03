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

#include "resources/db/sounddb.h"

#include "configuration.h"
#include "notifymanager.h"

#include "enums/resources/notifytypes.h"

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
    loadXmlFile(paths.getStringValue("soundsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("soundsPatchFile"), SkipError_true);
    loadXmlDir("soundsPatchDir", loadXmlFile);
}

void SoundDB::loadXmlFile(const std::string &fileName,
                          const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseResman_true,
        skipError);
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
                loadXmlFile(name, skipError);
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
    mSounds.resize(NotifyTypes::TYPE_END);
    for (int f = 0; f < NotifyTypes::TYPE_END; f ++)
        mSounds[f].clear();
}

std::string &SoundDB::getSound(const int id)
{
    if (id < 0 || id >= NotifyTypes::TYPE_END)
        return mDefault;
    return mSounds[id];
}
