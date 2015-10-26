/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "resources/db/npcdialogdb.h"

#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"

#include "utils/dtor.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    NpcDialogDB::Dialogs mDialogs;
}

void NpcDialogDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Loading npcdialog database...");
    loadXmlFile(paths.getStringValue("npcDialogsFile"));
    loadXmlFile(paths.getStringValue("npcDialogsPatchFile"));
    loadXmlDir("npcDialogsPatchDir", loadXmlFile);

    mLoaded = true;
}

static void loadNpcDialog(NpcDialogInfo *const dialog,
                          const XmlNodePtrConst node)
{
    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "button"))
        {
            const std::string name = XML::getProperty(childNode, "name", "");
            if (name.empty())
                continue;

            const std::string value = XML::getProperty(childNode, "value", "");
            if (value.empty())
                continue;

            NpcButtonInfo *const button = new NpcButtonInfo;
            button->x = XML::getIntProperty(
                childNode, "x", 0, 0, 10000);
            button->y = XML::getIntProperty(
                childNode, "y", 0, 0, 10000);
            button->name = name;
            button->value = value;
            dialog->buttons.push_back(button);
        }
    }
}

void NpcDialogDB::loadXmlFile(const std::string &fileName)
{
    XML::Document *const doc = new XML::Document(fileName,
        UseResman_true,
        SkipError_false);

    const XmlNodePtrConst root = doc->rootNode();
    if (!root)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "dialog"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            deleteDialog(name);
            NpcDialogInfo *const dialog = new NpcDialogInfo;
            dialog->name = name;
            mDialogs[name] = dialog;
            loadNpcDialog(dialog, node);
        }
        else if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
    }

    delete doc;
}

void NpcDialogDB::deleteDialog(const std::string &name)
{
    DialogsIter it = mDialogs.find(name);
    if (it == mDialogs.end())
        return;

    NpcDialogInfo *dialog = (*it).second;
    delete_all(dialog->buttons);
    mDialogs.erase(it);
    delete dialog;
}

void NpcDialogDB::unload()
{
    logger->log1("Unloading npcdialog database...");

    FOR_EACH(DialogsIter, it, mDialogs)
    {
        NpcDialogInfo *dialog = (*it).second;
        delete_all(dialog->buttons);
        delete dialog;
    }
    mDialogs.clear();

    mLoaded = false;
}
