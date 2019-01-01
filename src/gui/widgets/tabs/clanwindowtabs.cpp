/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "gui/widgets/tabs/clanwindowtabs.h"

#include "being/localclan.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

InfoClanTab::InfoClanTab(const Widget2 *const widget) :
    Container(widget),
    mNameLabel(new Label(this, "                ")),
    mMasterLabel(new Label(this, "                ")),
    mMapLabel(new Label(this, "                "))
{
    setSelectable(false);

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mNameLabel, 2, 1);
    place(0, 1, mMasterLabel, 2, 1);
    place(0, 2, mMapLabel, 2, 1);

    place.getCell().matchColWidth(0, 0);
    setDimension(Rect(0, 0, 600, 300));
}

void InfoClanTab::resetClan()
{
    // TRANSLATORS: not in clan label
    mNameLabel->setCaption(strprintf(_("Not in clan")));
    mMasterLabel->setCaption(std::string());
    mMapLabel->setCaption(std::string());
}

void InfoClanTab::updateClan()
{
    mNameLabel->setCaption(strprintf("%s: %s",
        // TRANSLATORS: clan name label
        _("Clan name"),
        localClan.name.c_str()));
    mMasterLabel->setCaption(strprintf("%s: %s",
        // TRANSLATORS: clan master name label
        _("Master name"),
        localClan.masterName.c_str()));
    mMapLabel->setCaption(strprintf("%s: %s",
        // TRANSLATORS: clan map name label
        _("Map name"),
        localClan.mapName.c_str()));
}

StatsClanTab::StatsClanTab(const Widget2 *const widget) :
    Container(widget),
    mLabels()
{
    setSelectable(false);
}

void StatsClanTab::clearLabels()
{
    FOR_EACH (STD_VECTOR<Label*>::iterator, it, mLabels)
    {
        remove(*it);
        delete *it;
    }
    mLabels.clear();
}

void StatsClanTab::resetClan()
{
    clearLabels();
}


void StatsClanTab::updateClan()
{
    clearLabels();

    LayoutHelper h(this);

    const int hPadding = h.getLayout().getHPadding();
    const int vPadding = h.getLayout().getVPadding();
    int y = vPadding;
    FOR_EACH (STD_VECTOR<std::string>::const_iterator, it, localClan.stats)
    {
        Label *const label = new Label(this, *it);
        add(label);
        label->setPosition(hPadding, y);
        label->adjustSize();
        y += label->getHeight() + vPadding;
        mLabels.push_back(label);
    }
}

RelationClanTab::RelationClanTab(const Widget2 *const widget) :
    Container(widget),
    mLabels()
{
    setSelectable(false);
}

void RelationClanTab::clearLabels()
{
    FOR_EACH (STD_VECTOR<Label*>::iterator, it, mLabels)
    {
        remove(*it);
        delete *it;
    }
    mLabels.clear();
}

void RelationClanTab::resetClan()
{
    clearLabels();
}

void RelationClanTab::updateClan(const STD_VECTOR<std::string> &restrict names)
{
    clearLabels();

    LayoutHelper h(this);

    const int hPadding = h.getLayout().getHPadding();
    const int vPadding = h.getLayout().getVPadding();
    int y = vPadding;
    FOR_EACH (STD_VECTOR<std::string>::const_iterator, it, names)
    {
        Label *const label = new Label(this, *it);
        add(label);
        label->setPosition(hPadding, y);
        label->adjustSize();
        y += label->getHeight() + vPadding;
        mLabels.push_back(label);
    }
}
