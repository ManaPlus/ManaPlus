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

#include "gui/widgets/tabs/netdebugtab.h"

#include "being/localplayer.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"

#include "net/packetcounters.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

NetDebugTab::NetDebugTab(const Widget2 *const widget) :
    DebugTab(widget),
    mPingLabel(new Label(this, "                ")),
    mInPackets1Label(new Label(this, "                ")),
    mOutPackets1Label(new Label(this, "                "))
{
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);

    place(0, 0, mPingLabel, 2, 1);
    place(0, 1, mInPackets1Label, 2, 1);
    place(0, 2, mOutPackets1Label, 2, 1);

    place.getCell().matchColWidth(0, 0);
    place = h.getPlacer(0, 1);
    setDimension(Rect(0, 0, 600, 300));
}

void NetDebugTab::logic()
{
    BLOCK_START("NetDebugTab::logic")
    if (localPlayer != nullptr)
    {
        // TRANSLATORS: debug window label
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"),
            localPlayer->getPingTime().c_str()));
    }
    else
    {
        // TRANSLATORS: debug window label
        mPingLabel->setCaption(strprintf(_("Ping: %s ms"), "0"));
    }
    // TRANSLATORS: debug window label
    mInPackets1Label->setCaption(strprintf(_("In: %d bytes/s"),
        PacketCounters::getInBytes()));
    // TRANSLATORS: debug window label
    mOutPackets1Label->setCaption(strprintf(_("Out: %d bytes/s"),
        PacketCounters::getOutBytes()));
    BLOCK_END("NetDebugTab::logic")
}
