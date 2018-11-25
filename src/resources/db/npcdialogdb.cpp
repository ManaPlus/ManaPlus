/*
 *  The ManaPlus Client
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

#include "resources/db/npcdialogdb.h"

#include "configuration.h"

#include "resources/beingcommon.h"
#include "resources/npcdialoginfo.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    NpcDialogDB::Dialogs mDialogs;
}  // namespace

void NpcDialogDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Loading npc dialog database...");
    loadXmlFile(paths.getStringValue("npcDialogsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("npcDialogsPatchFile"), SkipError_true);
    loadXmlDir("npcDialogsPatchDir", loadXmlFile)

    mLoaded = true;
}

static void loadNpcDialogMenu(NpcDialogInfo *const dialog,
                              XmlNodeConstPtrConst node)
{
    for_each_xml_child_node(childNode, node)
    {
        if (!xmlTypeEqual(childNode, XML_ELEMENT_NODE))
            continue;

        if (xmlNameEqual(childNode, "button"))
        {
            const std::string name = XML::getProperty(childNode, "name", "");
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
            button->image = XML::getProperty(childNode, "image", "");
            if (button->name.empty() && button->image.empty())
            {
                reportAlways("Error: npc button without name or image")
                delete button;
                continue;
            }
            button->imageWidth = XML::getIntProperty(
                childNode, "imageWidth", 16, 1, 1000);
            button->imageHeight = XML::getIntProperty(
                childNode, "imageHeight", 16, 1, 1000);
            dialog->menu.buttons.push_back(button);
        }
        else if (xmlNameEqual(childNode, "image"))
        {
            const std::string image = XML::getProperty(childNode, "image", "");
            if (image.empty())
            {
                reportAlways("Error: no image attribute found in image tag.")
                continue;
            }
            NpcImageInfo *const imageInfo = new NpcImageInfo;
            imageInfo->name = image;
            imageInfo->x = XML::getIntProperty(
                childNode, "x", 0, 0, 10000);
            imageInfo->y = XML::getIntProperty(
                childNode, "y", 0, 0, 10000);
            dialog->menu.images.push_back(imageInfo);
        }
        else if (xmlNameEqual(childNode, "text"))
        {
            const std::string text = XML::getProperty(childNode, "text", "");
            if (text.empty())
            {
                reportAlways("Error: no text attribute found in text tag.")
                continue;
            }
            NpcTextInfo *const textInfo = new NpcTextInfo;
            textInfo->text = text;
            textInfo->x = XML::getIntProperty(
                childNode, "x", 0, 0, 10000);
            textInfo->y = XML::getIntProperty(
                childNode, "y", 0, 0, 10000);
            textInfo->width = XML::getIntProperty(
                childNode, "width", 20, 10, 10000);
            textInfo->height = XML::getIntProperty(
                childNode, "height", 20, 10, 10000);
            dialog->menu.texts.push_back(textInfo);
        }
    }
}

static void loadNpcDialogInventory(NpcDialogInfo *const dialog,
                                   XmlNodePtrConst node)
{
    dialog->inventory.cell = XML::getProperty(node, "cell", "");
    dialog->inventory.columns = XML::getIntProperty(
        node, "columns", 10000, 1, 10000);
}

static void loadNpcDialog(NpcDialogInfo *const dialog,
                          XmlNodeConstPtrConst node)
{
    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "menu"))
        {
            loadNpcDialogMenu(dialog, childNode);
        }
        else if (xmlNameEqual(childNode, "inventory"))
        {
            loadNpcDialogInventory(dialog, childNode);
        }
    }
}

void NpcDialogDB::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document *const doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);

    XmlNodeConstPtrConst root = doc->rootNode();
    if (root == nullptr)
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
            dialog->hideText = XML::getBoolProperty(
                node, "hideText", false);
            mDialogs[name] = dialog;
            loadNpcDialog(dialog, node);
        }
        else if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
    }

    delete doc;
}

void NpcDialogDB::deleteDialog(const std::string &name)
{
    const DialogsIter it = mDialogs.find(name);
    if (it == mDialogs.end())
        return;

    NpcDialogInfo *dialog = (*it).second;
    delete_all(dialog->menu.buttons);
    delete_all(dialog->menu.images);
    delete_all(dialog->menu.texts);
    mDialogs.erase(it);
    delete dialog;
}

void NpcDialogDB::unload()
{
    logger->log1("Unloading npc dialog database...");

    FOR_EACH (DialogsIter, it, mDialogs)
    {
        NpcDialogInfo *dialog = (*it).second;
        delete_all(dialog->menu.buttons);
        delete_all(dialog->menu.images);
        delete_all(dialog->menu.texts);
        delete dialog;
    }
    mDialogs.clear();

    mLoaded = false;
}

NpcDialogInfo *NpcDialogDB::getDialog(const std::string &name)
{
    const DialogsIter it = mDialogs.find(name);
    if (it == mDialogs.end())
        return nullptr;
    return (*it).second;
}
