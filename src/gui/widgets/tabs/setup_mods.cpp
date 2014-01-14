/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/widgets/tabs/setup_mods.h"

#include "configuration.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/setupitem.h"
#include "gui/widgets/scrollarea.h"

#include "resources/db/moddb.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Mods::Setup_Mods(const Widget2 *const widget) :
    SetupTabScroll(widget)
{
    // TRANSLATORS: mods tab in settings
    setName(_("Mods"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

Setup_Mods::~Setup_Mods()
{
}

void Setup_Mods::apply()
{
    SetupTabScroll::apply();
    saveMods();
}

void Setup_Mods::externalUpdated()
{
    clear();
    loadMods();
}

void Setup_Mods::loadMods()
{
    std::string modsString = serverConfig.getValue("mods", "");
    std::set<std::string> modsList;
    splitToStringSet(modsList, modsString, '|');

    const ModInfos &mods = ModDB::getAll();
    if (mods.empty())
    {
        // TRANSLATORS: settings label
        new SetupItemLabel(_("No mods present"), "", this, false);
        return;
    }

    FOR_EACH (ModInfoCIterator, it, mods)
    {
        const ModInfo *const info = (*it).second;
        if (!info)
            continue;

        std::string name = info->getName();
        replaceAll(name, "|", "");
        SetupItem *const item = new SetupItemCheckBox(
            info->getDescription(), "", "", this, name);
        if (modsList.find(name) != modsList.end())
            item->setValue("1");
        else
            item->setValue("0");
        item->toWidget();
    }
}

void Setup_Mods::saveMods() const
{
    const ModInfos &mods = ModDB::getAll();
    if (mods.empty())
        return;

    std::string modsString;
    const std::set<SetupItem*> &modsList = getAllItems();
    FOR_EACH (std::set<SetupItem*>::const_iterator, it, modsList)
    {
        const SetupItem *const item = *it;
        if (!item)
            continue;
        const std::string val = item->getValue();
        if (val == "1")
        {
            const std::string key = item->getEventName();
            if (!modsString.empty())
                modsString.append("|");
            modsString.append(key);
        }
    }
    serverConfig.setValue("mods", modsString);
}

void Setup_Mods::externalUnloaded()
{
    clear();
}
