/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "resources/db/deaddb.h"

#include "configuration.h"
#include "logger.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    std::vector<std::string> mMessages;
}  // namespace

void DeadDB::load()
{
    if (mLoaded)
        unload();

    XML::Document *doc = new XML::Document(
        paths.getStringValue("deadMessagesFile"));
    const XmlNodePtr root = doc->rootNode();

    if (!root || !xmlNameEqual(root, "messages"))
    {
        logger->log("DeadDB: Failed to parse %s.",
            paths.getStringValue("deadMessagesFile").c_str());
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "message"))
        {
            const char *const data = reinterpret_cast<const char*>(
                xmlNodeGetContent(node));
            if (!data || !*data)
                continue;
            mMessages.push_back(data);
        }
    }

    delete doc;
    mLoaded = true;
}

void DeadDB::unload()
{
    mMessages.clear();
    mLoaded = false;
}

std::string DeadDB::getRandomString()
{
    const int sz = mMessages.size();
    if (!sz)
        return std::string();
    return translator->getStr(mMessages[rand() % sz]);
}
