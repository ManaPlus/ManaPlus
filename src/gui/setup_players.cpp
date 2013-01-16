/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/setup_players.h"

#include "gui/chatwindow.h"
#include "gui/editdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "configuration.h"
#include "localplayer.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Players::Setup_Players(const Widget2 *const widget) :
    SetupTabScroll(widget)
{
    setName(_("Players"));

    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemCheckBox(_("Show gender"), "",
        "showgender", this, "showgenderEvent");

    new SetupItemCheckBox(_("Show level"), "",
        "showlevel", this, "showlevelEvent");

    new SetupItemCheckBox(_("Show own name"), "",
        "showownname", this, "showownnameEvent");

    new SetupItemCheckBox(_("Enable extended mouse targeting"), "",
        "extMouseTargeting", this, "extMouseTargetingEvent");

    new SetupItemCheckBox(_("Target dead players"), "",
        "targetDeadPlayers", this, "targetDeadPlayersEvent");

    new SetupItemCheckBox(_("Visible names"), "",
        "visiblenames", this, "visiblenamesEvent");

    new SetupItemCheckBox(_("Secure trades"), "",
        "securetrades", this, "securetradesEvent");

    new SetupItemTextField(_("Unsecure chars in names"), "",
        "unsecureChars", this, "unsecureCharsEvent");

    new SetupItemCheckBox(_("Show statuses"), "",
        "showPlayersStatus", this, "showPlayersStatusEvent");

    new SetupItemCheckBox(_("Show ip addresses on screenshots"), "",
        "showip", this, "showipEvent");

    new SetupItemCheckBox(_("Allow self heal with mouse click"), "",
        "selfMouseHeal", this, "selfMouseHealEvent");

    new SetupItemCheckBox(_("Group friends in who is online window"), "",
        "groupFriends", this, "groupFriendsEvent");

    new SetupItemCheckBox(_("Hide erased players nicks"), "",
        "hideErased", this, "hideErasedEvent");

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

void Setup_Players::apply()
{
    SetupTabScroll::apply();
}
