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

#include "resources/db/itemoptiondb.h"

#include "configuration.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"

#include "resources/db/itemfielddb.h"

#include "debug.h"

namespace
{
    ItemOptionDb::OptionInfos mOptions;
    const STD_VECTOR<ItemFieldType*> mEmptyOption;
    bool mLoaded = false;
}  // namespace

void ItemOptionDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing item options database...");
    loadXmlFile(paths.getStringValue("itemOptionsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("itemOptionsPatchFile"), SkipError_true);
    loadXmlDir("itemOptionsPatchDir", loadXmlFile);
    mLoaded = true;
}

static void addFieldByName(STD_VECTOR<ItemFieldType*> &options,
                           XmlNodeConstPtr node,
                           const ItemFieldDb::FieldInfos &fields,
                           const char *const name)
{
    std::string value = XML::getProperty(node, name, "");
    if (value.empty())
        return;

    FOR_EACH (ItemFieldDb::FieldInfos::const_iterator, it, fields)
    {
        const std::string fieldName = (*it).first;
        if (fieldName == value)
        {
            options.push_back((*it).second);
            return;
        }
    }
}

static void readOptionFields(STD_VECTOR<ItemFieldType*> &options,
                             XmlNodeConstPtr node,
                             const ItemFieldDb::FieldInfos &fields)
{
    addFieldByName(options, node, fields, "field");
    for (int f = 0; f < 15; f ++)
    {
        const std::string field = strprintf("field%d", f);
        addFieldByName(options, node, fields, field.c_str());
    }
}

void ItemOptionDb::loadXmlFile(const std::string &fileName,
                               const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "itemoptions"))
    {
        if (skipError == SkipError_true)
        {
            logger->log("ItemFieldDb: Error while loading %s!",
                fileName.c_str());
        }
        else
        {
            reportAlways("ItemFieldDb: Error while loading %s!",
                fileName.c_str());
        }
        return;
    }

    const ItemFieldDb::FieldInfos &requiredFields =
        ItemFieldDb::getRequiredFields();
    const ItemFieldDb::FieldInfos &addFields =
        ItemFieldDb::getAddFields();

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        if (xmlNameEqual(node, "option"))
        {
            const int id = XML::getProperty(node,
                "id",
                0);
            if (id <= 0)
            {
                reportAlways("Empty id field in ItemOptionDb");
                continue;
            }
            STD_VECTOR<ItemFieldType*> &options = mOptions[id];
            readOptionFields(options, node, requiredFields);
            readOptionFields(options, node, addFields);
        }
    }
}

void ItemOptionDb::unload()
{
    logger->log1("Unloading item options database...");
    mOptions.clear();
    mLoaded = false;
}

const STD_VECTOR<ItemFieldType*> &ItemOptionDb::getFields(const int id)
{
    OptionInfos::const_iterator it = mOptions.find(id);
    if (it == mOptions.end())
        return mEmptyOption;
    return (*it).second;
}
