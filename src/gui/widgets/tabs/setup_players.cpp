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

#include "gui/widgets/tabs/setup_players.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Players::Setup_Players(const Widget2 *const widget) :
    SetupTabScroll(widget)
{
    // TRANSLATORS: settings tab name
    setName(_("Players"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show gender"), "",
        "showgender", this, "showgenderEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show level"), "",
        "showlevel", this, "showlevelEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show own name"), "",
        "showownname", this, "showownnameEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable extended mouse targeting"), "",
        "extMouseTargeting", this, "extMouseTargetingEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Target dead players"), "",
        "targetDeadPlayers", this, "targetDeadPlayersEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Visible names"), "",
        "visiblenames", this, "visiblenamesEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto move names"), "",
        "moveNames", this, "moveNamesEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Secure trades"), "",
        "securetrades", this, "securetradesEvent");

    // TRANSLATORS: settings option
    new SetupItemTextField(_("Unsecure chars in names"), "",
        "unsecureChars", this, "unsecureCharsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show statuses"), "",
        "showPlayersStatus", this, "showPlayersStatusEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Show ip addresses on screenshots"), "",
        "showip", this, "showipEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Allow self heal with mouse click"), "",
        "selfMouseHeal", this, "selfMouseHealEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Group friends in who is online window"), "",
        "groupFriends", this, "groupFriendsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Hide erased players nicks"), "",
        "hideErased", this, "hideErasedEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use special diagonal speed in players moving"),
        "", "useDiagonalSpeed", this, "useDiagonalSpeedEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Emulate right mouse button by long mouse click"
        " (usefull for touch interfaces)"),
        "", "longmouseclick", this, "longmouseclickEvent");

    setDimension(Rect(0, 0, 550, 350));
}
