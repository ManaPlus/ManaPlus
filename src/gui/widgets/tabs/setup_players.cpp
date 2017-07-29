/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "configuration.h"
#include "settings.h"

#include "gui/models/namesmodel.h"

#include "gui/widgets/containerplacer.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

static const int badgesListSize = 4;

static const char *const badgesList[] =
{
    // TRANSLATORS: screen badges type
    N_("hide"),
    // TRANSLATORS: screen badges type
    N_("show at top"),
    // TRANSLATORS: screen badges type
    N_("show at right"),
    // TRANSLATORS: screen badges type
    N_("show at bottom"),
};

static const int visibleNamesListSize = 3;

static const char *const visibleNamesList[] =
{
    // TRANSLATORS: visible name type
    N_("hide"),
    // TRANSLATORS: visible name type
    N_("show"),
    // TRANSLATORS: visible name type
    N_("show on selection")
};

static const int topDownListSize = 2;

static const char *const topDownList[] =
{
    // TRANSLATORS: show on top or down
    N_("top"),
    // TRANSLATORS: show on top or down
    N_("buttom")
};

Setup_Players::Setup_Players(const Widget2 *const widget) :
    SetupTabScroll(widget),
    mBadgesList(new NamesModel),
    mVisibleNamesList(new NamesModel),
    mVisibleNamesPosList(new NamesModel)
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

    mVisibleNamesList->fillFromArray(&visibleNamesList[0],
        visibleNamesListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Show player names"), "",
        "visiblenames", this, "visiblenamesEvent",
        mVisibleNamesList, 150);

    mVisibleNamesPosList->fillFromArray(&topDownList[0],
        topDownListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Show player names at"), "",
        "visiblenamespos", this, "visiblenamesposEvent",
        mVisibleNamesPosList, 150);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Auto move names"), "",
        "moveNames", this, "moveNamesEvent");

    mBadgesList->fillFromArray(&badgesList[0], badgesListSize);
    // TRANSLATORS: settings option
    new SetupItemDropDown(_("Badges"), "",
        "showBadges", this, "showBadgesEvent", mBadgesList, 150);

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
    new SetupItemCheckBox(_("Collect players id and seen log"),
        "", "enableIdCollecting", this, "enableIdCollectingEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Use special diagonal speed in players moving"),
        "", "useDiagonalSpeed", this, "useDiagonalSpeedEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Log players actions (for GM)"),
        "", "logPlayerActions", this, "logPlayerActionsEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Create screenshots for each complete trades"),
        "", "tradescreenshot", this, "tradescreenshotEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Emulate right mouse button by long mouse click"
        " (useful for touch interfaces)"),
        "", "longmouseclick", this, "longmouseclickEvent");

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Enable remote commands"),
        "", "enableRemoteCommands", this, "enableRemoteCommandsEvent",
        MainConfig_false);

    // TRANSLATORS: settings option
    new SetupItemCheckBox(_("Allow move character by mouse"),
        "", "allowMoveByMouse", this, "allowMoveByMouseEvent");

    setDimension(Rect(0, 0, 550, 350));
}

Setup_Players::~Setup_Players()
{
    delete2(mBadgesList);
    delete2(mVisibleNamesList);
    delete2(mVisibleNamesPosList);
}

void Setup_Players::externalUpdated()
{
    reread("enableRemoteCommands");
}

void Setup_Players::apply()
{
    SetupTabScroll::apply();
    settings.enableRemoteCommands = (serverConfig.getValue(
        "enableRemoteCommands", 1) != 0);
}
