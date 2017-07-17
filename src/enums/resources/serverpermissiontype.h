/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef ENUMS_RESOURCES_SERVERPERMISSIONTYPE_H
#define ENUMS_RESOURCES_SERVERPERMISSIONTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(ServerPermissionType)
{
// Hercules permissions
    can_trade = 0,
    can_party,
    all_skill,
    all_equipment,
    skill_unconditional,
    join_chat,
    kick_chat,
    hide_session,
    who_display_aid,
    hack_info,
    any_warp,
    view_hpmeter,
    view_equipment,
    use_check,
    use_changemaptype,
    all_commands,
    receive_requests,
    show_bossmobs,
    disable_pvm,
    disable_pvp,
    disable_commands_when_dead,
    hchsys_admin,
    can_trade_bound,
    disable_pickup,
    disable_store,
    disable_exp,
    disable_skill_usage,

// Evol permissions
    send_gm,
    show_client_version,

    Max
}
enumEnd(ServerPermissionType);

#endif  // ENUMS_RESOURCES_SERVERPERMISSIONTYPE_H

